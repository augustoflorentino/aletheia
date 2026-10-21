#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include "../include/aletheia/aletheia.h"
#include "panel.h"

#define BOOT_ROM        "firmware/boot.asm"
#define KEY_POLL_CYCLES 256
#define MAX_FAULTS      16
#define MAX_BREAKPOINTS 32

typedef struct {
    long max_cycles;
    int cycles_given;
    int verbose;
    int boot;
    int panel;
    alt_arch_t arch;
    long hz;
    unsigned straps;
    unsigned timer_div;
    long dram_leak;
    unsigned long long power_off_at;
    long power_off_cycles;
    const char *vcd_file;
    const char *asm_file;
    const char *input;
    const char *faults[MAX_FAULTS];
    int fault_count;
    uint32_t breakpoints[MAX_BREAKPOINTS];
    int breakpoint_count;
} options_t;

typedef struct {
    const options_t *opt;
} hook_ctx_t;

static struct termios saved_tty;
static int tty_raw;

static void restore_tty(void) {
    if (tty_raw) {
        tcsetattr(STDIN_FILENO, TCSANOW, &saved_tty);
        if (write(STDOUT_FILENO, "\x1b[?25h", 6) < 0) tty_raw = 0;
    }
    tty_raw = 0;
}

static void on_signal(int sig) {
    restore_tty();
    signal(sig, SIG_DFL);
    raise(sig);
}

static void open_keyboard(void) {
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &saved_tty) != 0) return;
    struct termios raw = saved_tty;
    raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    raw.c_iflag &= ~(tcflag_t)(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    tty_raw = 1;
    atexit(restore_tty);
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);
}

static void poll_keyboard(alt_machine_t *m) {
    unsigned char buf[64];
    ssize_t n = read(STDIN_FILENO, buf, sizeof buf);
    for (ssize_t i = 0; i < n; i++) alt_send_key(m, buf[i]);
}

static double seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

static void panel_key(alt_machine_t *m, panel_t *panel, unsigned char key) {
    long hz = alt_hz(m);
    if (key == 16) panel->paused = !panel->paused;
    else if (key == 14 && panel->paused) alt_step(m);
    else if (key == 6) alt_set_hz(m, hz == 0 || hz >= 65536 ? 0 : hz * 2);
    else if (key == 2) alt_set_hz(m, hz == 0 ? 4096 : (hz > 1 ? hz / 2 : 1));
    else alt_send_key(m, key == '\r' ? '\n' : key);
}

static void idle(void) {
    struct timespec t = {0, 20000000L};
    nanosleep(&t, NULL);
}

static int run_panel(alt_machine_t *m, const uint32_t *rom, long max_cycles) {
    static panel_t panel;
    panel_init(&panel, rom);
    alt_set_output(m, panel_output, &panel);
    open_keyboard();
    panel_open();
    alt_power_on(m);
    double last_draw = seconds();
    uint64_t last_cycles = 0;
    while (max_cycles == 0 || (long)alt_cycles(m) < max_cycles) {
        unsigned char keys[64];
        ssize_t n = read(STDIN_FILENO, keys, sizeof keys);
        for (ssize_t i = 0; i < n; i++) panel_key(m, &panel, keys[i]);
        int running = !panel.paused && !alt_halted(m);
        long hz = alt_hz(m);
        long chunk = hz > 0 ? (hz / 20 > 0 ? hz / 20 : 1) : KEY_POLL_CYCLES;
        for (long c = 0; running && c < chunk && !alt_halted(m); c++) alt_step(m);
        if (!running) idle();
        double now = seconds();
        if (now - last_draw < 0.05) continue;
        panel.rate = (double)(alt_cycles(m) - last_cycles) / (now - last_draw);
        last_draw = now;
        last_cycles = alt_cycles(m);
        panel_draw(&panel, m);
    }
    panel_draw(&panel, m);
    panel_close();
    restore_tty();
    return alt_halted(m) ? 0 : 2;
}

static int run_boot(alt_machine_t *m, long max_cycles) {
    open_keyboard();
    alt_power_on(m);
    while (!alt_halted(m) && (max_cycles == 0 || (long)alt_cycles(m) < max_cycles)) {
        if (alt_cycles(m) % KEY_POLL_CYCLES == 0) poll_keyboard(m);
        alt_step(m);
    }
    for (int i = 0; i < 12; i++) alt_step(m);
    restore_tty();
    printf("\n");
    return alt_halted(m) ? 0 : 2;
}

static void dump_state(alt_machine_t *m) {
    static const unsigned regs[] = {8, 9, 10, 11, 12, 13};
    static const char *names[] = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5"};
    printf("\nRegisters:\n");
    for (int i = 0; i < 6; i++) {
        unsigned v = alt_reg(m, regs[i]);
        printf("  %s = %u (0x%08x)\n", names[i], v, v);
    }
    printf("Display:\n  [");
    for (unsigned i = 0; i < ALT_DISPLAY_CHARS; i++) {
        unsigned ch = alt_display_char(m, i);
        putchar(ch >= 32 && ch < 127 ? (int)ch : ' ');
    }
    printf("]\n");
    printf("Memory (RAM):\n");
    for (unsigned i = 0; i < 8; i++) {
        uint32_t v = 0;
        alt_ram_read(m, i, &v);
        printf("  ram[%u] = %u (0x%08x)\n", i, (unsigned)v, (unsigned)v);
    }
}

static void print_usage(const char *prog) {
    printf("Aletheia - a gate-level hardware simulator\n\n");
    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n");
    printf("  -h, --help          Show this help\n");
    printf("      --version       Show the version\n");
    printf("  -m, --arch ARCH     single | multi | pipe (default: single)\n");
    printf("      --hz N          Limit the clock to N cycles per second\n");
    printf("  -i, --input TEXT    Type TEXT into the UART (\\n for Enter)\n");
    printf("      --power-off C:D Cut the power after cycle C for D cycles, then\n");
    printf("                      power on again (DRAM decays, SRAM is lost)\n");
    printf("      --dram-leak N   DRAM leak period in cycles (default 64, 0 = never)\n");
    printf("Jumpers (sampled at reset):\n");
    printf("      --l2            Enable the L2 cache\n");
    printf("      --no-l1         Disable the L1 cache\n");
    printf("      --no-mmu        Tie the MMU off (writes to its enable are ignored)\n");
    printf("      --timer-div N   Timer prescaler: 1, 2, 4 or 8\n");
    printf("  -f, --fault SPEC    Flip a flip-flop before cycle C (repeatable):\n");
    printf("                      C:pc:BIT  C:rN:BIT  C:ramADDR:BIT (ADDR in hex)\n");
    printf("  -c, --cycles N      Run N cycles (default: 100)\n");
    printf("  -v, --verbose       Verbose mode\n");
    printf("  -w, --vcd FILE      Write a VCD waveform file\n");
    printf("  -a, --asm FILE      Load an assembly program\n");
    printf("  -d, --disasm WORD   Disassemble one instruction (hex)\n");
    printf("  -b, --break ADDR    Stop when the PC reaches ADDR (hex, repeatable)\n");
    printf("      --boot          Power on into the boot ROM and monitor (%s);\n", BOOT_ROM);
    printf("                      the keyboard goes to the UART, Ctrl-C quits\n");
    printf("      --panel         Front panel: registers, display, UART, pipeline;\n");
    printf("                      boots the ROM unless -a is given\n");
    printf("\n");
    printf("Exit status: 0 halted, 1 error, 2 cycle limit reached, 3 breakpoint\n");
}

static int is_opt(const char *arg, const char *short_name, const char *long_name) {
    return strcmp(arg, short_name) == 0 || strcmp(arg, long_name) == 0;
}

static int fail(const char *what, const char *val, const char *hint) {
    fprintf(stderr, "invalid %s: %s%s\n", what, val, hint);
    return -1;
}

static int parse_flag(options_t *o, const char *opt) {
    if (strcmp(opt, "--panel") == 0) o->panel = 1;
    else if (strcmp(opt, "--boot") == 0) o->boot = 1;
    else if (strcmp(opt, "--l2") == 0) o->straps |= ALT_STRAP_L2;
    else if (strcmp(opt, "--no-l1") == 0) o->straps &= ~ALT_STRAP_L1;
    else if (strcmp(opt, "--no-mmu") == 0) o->straps &= ~ALT_STRAP_MMU;
    else if (is_opt(opt, "-v", "--verbose")) o->verbose = 1;
    else return 0;
    return 1;
}

static int parse_cycles(options_t *o, const char *val) {
    char *end;
    long n = strtol(val, &end, 10);
    if (*end != '\0' || n <= 0 || n > 2000000000L) return fail("cycle count", val, "");
    o->max_cycles = n;
    o->cycles_given = 1;
    return 0;
}

static int parse_power_off(options_t *o, const char *val) {
    char *end;
    o->power_off_at = strtoull(val, &end, 10);
    o->power_off_cycles = *end == ':' ? strtol(end + 1, &end, 10) : 0;
    if (*end != '\0' || o->power_off_at == 0 || o->power_off_cycles <= 0) return fail("power cut", val, " (CYCLE:DURATION)");
    return 0;
}

static int parse_timer_div(options_t *o, const char *val) {
    int d = atoi(val);
    if (d != 1 && d != 2 && d != 4 && d != 8) return fail("timer divider", val, " (1, 2, 4 or 8)");
    o->timer_div = (unsigned)d;
    return 0;
}

static int parse_dram_leak(options_t *o, const char *val) {
    char *end;
    o->dram_leak = strtol(val, &end, 10);
    if (*end != '\0' || o->dram_leak < 0) return fail("leak period", val, "");
    return 0;
}

static int parse_fault(options_t *o, const char *val) {
    if (o->fault_count >= MAX_FAULTS) return fail("fault", val, " (too many)");
    o->faults[o->fault_count++] = val;
    return 0;
}

static int parse_hz(options_t *o, const char *val) {
    char *end;
    o->hz = strtol(val, &end, 10);
    if (*end != '\0' || o->hz <= 0 || o->hz > 100000000L) return fail("clock rate", val, "");
    return 0;
}

static int parse_arch(options_t *o, const char *val) {
    if (strcmp(val, "single") == 0) o->arch = ALT_ARCH_SINGLE;
    else if (strcmp(val, "multi") == 0) o->arch = ALT_ARCH_MULTI;
    else if (strcmp(val, "pipe") == 0) o->arch = ALT_ARCH_PIPE;
    else return fail("arch", val, " (single, multi or pipe)");
    return 0;
}

static int parse_break(options_t *o, const char *val) {
    char *end;
    unsigned long addr = strtoul(val, &end, 16);
    if (*end != '\0' || *val == '\0' || addr > 0xFFFFFFFFUL || o->breakpoint_count >= MAX_BREAKPOINTS) return fail("breakpoint", val, "");
    o->breakpoints[o->breakpoint_count++] = (uint32_t)addr;
    return 0;
}

static int disasm_word(const char *val) {
    char *end;
    unsigned long word = strtoul(val, &end, 16);
    if (*end != '\0' || *val == '\0' || word > 0xFFFFFFFFUL) return fail("instruction word", val, "");
    char text[64];
    alt_disassemble((uint32_t)word, text, sizeof text);
    printf("0x%08lx: %s\n", word, text);
    return 1;
}

static int set_path(const char **slot, const char *val) {
    *slot = val;
    return 0;
}

static int parse_value(options_t *o, const char *opt, const char *val) {
    if (is_opt(opt, "-c", "--cycles")) return parse_cycles(o, val);
    if (is_opt(opt, "-w", "--vcd")) return set_path(&o->vcd_file, val);
    if (is_opt(opt, "-a", "--asm")) return set_path(&o->asm_file, val);
    if (is_opt(opt, "-i", "--input")) return set_path(&o->input, val);
    if (strcmp(opt, "--power-off") == 0) return parse_power_off(o, val);
    if (strcmp(opt, "--timer-div") == 0) return parse_timer_div(o, val);
    if (strcmp(opt, "--dram-leak") == 0) return parse_dram_leak(o, val);
    if (is_opt(opt, "-f", "--fault")) return parse_fault(o, val);
    if (strcmp(opt, "--hz") == 0) return parse_hz(o, val);
    if (is_opt(opt, "-m", "--arch")) return parse_arch(o, val);
    if (is_opt(opt, "-b", "--break")) return parse_break(o, val);
    if (is_opt(opt, "-d", "--disasm")) return disasm_word(val);
    fprintf(stderr, "unknown option: %s\n", opt);
    return -1;
}

static int parse_args(options_t *o, int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        const char *opt = argv[i];
        if (is_opt(opt, "-h", "--help")) {
            print_usage(argv[0]);
            return 1;
        }
        if (strcmp(opt, "--version") == 0) {
            printf("aletheia %s\n", alt_version());
            return 1;
        }
        if (parse_flag(o, opt)) continue;
        if (opt[0] != '-' || i + 1 >= argc) {
            fprintf(stderr, opt[0] == '-' ? "option %s needs a value\n" : "unknown option: %s\n", opt);
            return -1;
        }
        int r = parse_value(o, opt, argv[++i]);
        if (r != 0) return r;
    }
    return 0;
}

static void type_input(alt_machine_t *m, const char *text) {
    for (const char *c = text; c && *c; c++) {
        int escaped_newline = c[0] == '\\' && c[1] == 'n';
        alt_send_key(m, escaped_newline ? '\n' : (unsigned char)*c);
        c += escaped_newline;
    }
}

static void print_output(void *ctx, int byte) {
    (void)ctx;
    putchar(byte);
    fflush(stdout);
}

static void on_cycle(void *ctx, alt_machine_t *m) {
    const options_t *o = ctx;
    uint64_t c = alt_cycles(m);
    if (o->verbose && c % 1000 == 0 && !alt_halted(m)) printf("Cycle: %llu\n", (unsigned long long)c);
}

static int arm_faults(alt_machine_t *m, const options_t *o, int quiet) {
    for (int i = 0; i < o->fault_count; i++) {
        if (alt_add_fault_spec(m, o->faults[i]) != ALT_OK) return fail("fault", o->faults[i], " (C:pc:BIT, C:rN:BIT or C:ramADDR:BIT)");
        if (!quiet) printf("Fault armed: %s\n", o->faults[i]);
    }
    return 0;
}

static int configure(alt_machine_t *m, const options_t *o, int quiet) {
    alt_set_arch(m, o->arch);
    alt_set_hz(m, o->hz);
    alt_set_output(m, print_output, NULL);
    alt_set_cycle_hook(m, on_cycle, (void *)o);
    type_input(m, o->input);
    if (arm_faults(m, o, quiet)) return -1;
    alt_set_straps(m, o->straps);
    alt_set_timer_div(m, o->timer_div);
    alt_set_dram_leak(m, (unsigned long)o->dram_leak);
    alt_power_cut(m, o->power_off_at, (unsigned long)o->power_off_cycles);
    return 0;
}

static int assemble(const char *path, uint32_t *rom, size_t *out_count) {
    char error[512];
    if (alt_assemble_file(path, rom, ALT_ROM_WORDS, out_count, error, sizeof error) == ALT_OK) return 0;
    fprintf(stderr, "%s\n", error);
    return -1;
}

static int run_interactive(alt_machine_t *m, const options_t *o) {
    static uint32_t rom[ALT_ROM_WORDS];
    size_t count;
    if (configure(m, o, 1) || assemble(o->asm_file ? o->asm_file : BOOT_ROM, rom, &count)) return 1;
    alt_load_rom(m, rom, count);
    long limit = o->cycles_given ? o->max_cycles : 0;
    return o->panel ? run_panel(m, rom, limit) : run_boot(m, limit);
}

static void list_program(const uint32_t *rom, size_t count) {
    for (size_t i = 0; i < count; i++) {
        char text[64];
        alt_disassemble(rom[i], text, sizeof text);
        printf("  [%zu] 0x%08x  %s\n", i, (unsigned)rom[i], text);
    }
    printf("\n");
}

static int load_program(alt_machine_t *m, const options_t *o) {
    static const uint32_t demo[] = {0x20080005, 0x2009000a, 0x01095020, 0xac0a0000, 0x00000000};
    static uint32_t rom[ALT_ROM_WORDS];
    size_t count = sizeof demo / sizeof demo[0];
    memcpy(rom, demo, sizeof demo);
    if (o->asm_file) printf("Loading program: %s\n", o->asm_file);
    if (o->asm_file && assemble(o->asm_file, rom, &count)) return -1;
    if (o->asm_file) printf("Instructions loaded: %zu\n\n", count);
    else printf("Demo program:\n");
    list_program(rom, count);
    alt_load_rom(m, rom, count);
    return 0;
}

static int arm_debug(alt_machine_t *m, const options_t *o) {
    for (int i = 0; i < o->breakpoint_count; i++) alt_break_add(m, o->breakpoints[i]);
    if (!o->vcd_file) return 0;
    printf("Writing waveform: %s\n", o->vcd_file);
    if (alt_vcd_open(m, o->vcd_file) == ALT_OK) return 0;
    printf("Failed to create VCD file\n");
    return -1;
}

static int finish(alt_machine_t *m, const options_t *o, alt_stop_t stop) {
    int status = stop == ALT_STOP_HALT ? 0 : 2;
    if (stop == ALT_STOP_BREAK) printf("\nBreakpoint at PC 0x%02x\n", (unsigned)alt_pc(m));
    if (stop == ALT_STOP_BREAK) status = 3;
    printf("\nSimulation finished.\n");
    printf("Cycles executed: %llu\n", (unsigned long long)alt_cycles(m));
    dump_state(m);
    if (o->vcd_file) printf("Waveform saved to: %s\n", o->vcd_file);
    return status;
}

static int run_batch(alt_machine_t *m, const options_t *o) {
    printf("===========================================\n");
    printf("  ALETHEIA - Gate-Level Hardware Simulator\n");
    printf("===========================================\n\n");
    if (configure(m, o, 0) || load_program(m, o) || arm_debug(m, o)) return 1;
    printf("Starting simulation (%ld cycles)...\n\n", o->max_cycles);
    alt_power_on(m);
    return finish(m, o, alt_run(m, (uint64_t)o->max_cycles));
}

int main(int argc, char **argv) {
    static options_t o;
    o.max_cycles = 100;
    o.straps = ALT_STRAP_DEFAULT;
    o.timer_div = 1;
    o.dram_leak = 64;
    int parsed = parse_args(&o, argc, argv);
    if (parsed != 0) return parsed > 0 ? 0 : 1;
    alt_machine_t *m;
    if (alt_machine_create(NULL, &m) != ALT_OK) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }
    int status = o.boot || o.panel ? run_interactive(m, &o) : run_batch(m, &o);
    alt_machine_destroy(m);
    return status;
}
