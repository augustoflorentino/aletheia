#include "../include/aletheia/aletheia.h"
#include "../simulation/simulation.h"
#include "../isa/isa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALT_STR2(x) #x
#define ALT_STR(x)  ALT_STR2(x)

struct alt_machine {
    simulator_t sim;
    alt_allocator_t allocator;
    bit straps[STRAP_COUNT];
    char out[ALT_OUTPUT_MAX];
    size_t out_len;
    alt_output_fn on_output;
    void *output_ctx;
    alt_cycle_fn on_cycle;
    void *cycle_ctx;
    int at_break;
};

static void *default_alloc(void *ctx, size_t size) {
    (void)ctx;
    return malloc(size);
}

static void default_free(void *ctx, void *ptr) {
    (void)ctx;
    free(ptr);
}

static const alt_allocator_t default_allocator = {default_alloc, default_free, NULL};

static void machine_free(const alt_machine_t *m, void *ptr) {
    if (ptr) m->allocator.free(m->allocator.ctx, ptr);
}

static uint32_t word_of(const bit *bits, int width) {
    uint32_t v = 0;
    for (int i = 0; i < width; i++) v |= (uint32_t)(bits[i] & 1) << i;
    return v;
}

static uint32_t word_of_dffs(const d_flip_flop_t *f, int width) {
    uint32_t v = 0;
    for (int i = 0; i < width; i++) v |= (uint32_t)(f[i].q & 1) << i;
    return v;
}

static void bits_of(uint32_t v, bit *bits, int width) {
    for (int i = 0; i < width; i++) bits[i] = (bit)((v >> i) & 1u);
}

static void output_tap(void *ctx, int byte) {
    alt_machine_t *m = ctx;
    if (m->out_len < ALT_OUTPUT_MAX) m->out[m->out_len++] = (char)byte;
    if (m->on_output) m->on_output(m->output_ctx, byte);
}

static void apply_straps(alt_machine_t *m) {
    memcpy(m->sim.board.config.pin, m->straps, sizeof m->sim.board.config.pin);
    config_reset(&m->sim.board.config);
}

const char *alt_version(void) {
    return ALT_STR(ALT_VERSION_MAJOR) "." ALT_STR(ALT_VERSION_MINOR) "." ALT_STR(ALT_VERSION_PATCH);
}

const char *alt_status_name(alt_status_t status) {
    static const char *names[] = {"ok", "out of memory", "invalid argument", "input/output error",
                                  "invalid format", "full"};
    if ((unsigned)status >= sizeof names / sizeof names[0]) return "unknown status";
    return names[status];
}

alt_status_t alt_machine_create(const alt_allocator_t *allocator, alt_machine_t **out_machine) {
    if (!out_machine) return ALT_ERR_ARG;
    *out_machine = NULL;
    if (allocator && (!allocator->alloc || !allocator->free)) return ALT_ERR_ARG;
    const alt_allocator_t *a = allocator ? allocator : &default_allocator;
    alt_machine_t *m = a->alloc(a->ctx, sizeof *m);
    if (!m) return ALT_ERR_NOMEM;
    memset(m, 0, sizeof *m);
    m->allocator = *a;
    simulator_init(&m->sim);
    simulator_enable_console(&m->sim);
    simulator_set_arch(&m->sim, SIM_ARCH_SINGLE);
    m->sim.on_output = output_tap;
    m->sim.output_ctx = m;
    alt_set_straps(m, ALT_STRAP_DEFAULT);
    *out_machine = m;
    return ALT_OK;
}

void alt_machine_destroy(alt_machine_t *machine) {
    if (!machine) return;
    alt_vcd_close(machine);
    machine_free(machine, machine);
}

alt_status_t alt_set_arch(alt_machine_t *machine, alt_arch_t arch) {
    if ((unsigned)arch > ALT_ARCH_PIPE) return ALT_ERR_ARG;
    simulator_set_arch(&machine->sim, (sim_arch_t)arch);
    return ALT_OK;
}

void alt_set_straps(alt_machine_t *machine, unsigned straps) {
    machine->straps[STRAP_L1] = (bit)((straps & ALT_STRAP_L1) != 0);
    machine->straps[STRAP_L2] = (bit)((straps & ALT_STRAP_L2) != 0);
    machine->straps[STRAP_MMU] = (bit)((straps & ALT_STRAP_MMU) != 0);
    apply_straps(machine);
}

alt_status_t alt_set_timer_div(alt_machine_t *machine, unsigned div) {
    static const int codes[9] = {-1, 0, 1, -1, 2, -1, -1, -1, 3};
    if (div > 8 || codes[div] < 0) return ALT_ERR_ARG;
    machine->straps[STRAP_TDIV0] = (bit)(codes[div] & 1);
    machine->straps[STRAP_TDIV1] = (bit)(codes[div] >> 1);
    apply_straps(machine);
    return ALT_OK;
}

void alt_set_dram_leak(alt_machine_t *machine, unsigned long period) {
    machine->sim.dram_leak_period = (long)period;
}

void alt_set_hz(alt_machine_t *machine, long hz) {
    simulator_set_hz(&machine->sim, hz > 0 ? hz : 0);
}

void alt_set_output(alt_machine_t *machine, alt_output_fn fn, void *ctx) {
    machine->on_output = fn;
    machine->output_ctx = ctx;
}

void alt_set_cycle_hook(alt_machine_t *machine, alt_cycle_fn fn, void *ctx) {
    machine->on_cycle = fn;
    machine->cycle_ctx = ctx;
}

alt_status_t alt_assemble_file(const char *path, uint32_t *words, size_t words_len,
                               size_t *out_count, char *out_error, size_t out_error_len) {
    if (!path || !words || !out_count || words_len > ALT_ROM_WORDS) return ALT_ERR_ARG;
    assembler_t as;
    assembler_init(&as);
    unsigned int buf[ALT_ROM_WORDS];
    int count = assembler_assemble_file(&as, path, buf, (int)words_len);
    if (count < 0 && out_error && out_error_len > 0) snprintf(out_error, out_error_len, "%s", as.error);
    if (count < 0) return ALT_ERR_FORMAT;
    for (int i = 0; i < count; i++) words[i] = buf[i];
    *out_count = (size_t)count;
    return ALT_OK;
}

alt_status_t alt_disassemble(uint32_t word, char *out_text, size_t out_text_len) {
    if (!out_text || out_text_len == 0) return ALT_ERR_ARG;
    char buf[MAX_DISASM_LEN];
    disassemble_instruction(word, buf, MAX_DISASM_LEN);
    snprintf(out_text, out_text_len, "%s", buf);
    return ALT_OK;
}

alt_status_t alt_load_rom(alt_machine_t *machine, const uint32_t *words, size_t words_len) {
    if ((!words && words_len) || words_len > ALT_ROM_WORDS) return ALT_ERR_ARG;
    for (size_t i = 0; i < words_len; i++) {
        bit bits[32];
        bits_of(words[i], bits, 32);
        rom_load_at(&machine->sim.board.rom, (unsigned)i, bits);
    }
    return ALT_OK;
}

alt_status_t alt_send_key(alt_machine_t *machine, unsigned char key) {
    if (!simulator_send_key(&machine->sim, key)) return ALT_ERR_FULL;
    return ALT_OK;
}

alt_status_t alt_add_fault(alt_machine_t *machine, uint64_t cycle, alt_fault_target_t target,
                           unsigned index, unsigned bit) {
    if ((unsigned)target > ALT_FAULT_RAM || bit > 31) return ALT_ERR_ARG;
    fault_t f = {cycle, (fault_target_t)target, index, bit};
    return simulator_add_fault(&machine->sim, &f) ? ALT_OK : ALT_ERR_FULL;
}

alt_status_t alt_add_fault_spec(alt_machine_t *machine, const char *spec) {
    fault_t f;
    if (!spec || !fault_parse(spec, &f)) return ALT_ERR_FORMAT;
    return simulator_add_fault(&machine->sim, &f) ? ALT_OK : ALT_ERR_FULL;
}

void alt_power_cut(alt_machine_t *machine, uint64_t at_cycle, unsigned long off_cycles) {
    machine->sim.power_off_at = at_cycle;
    machine->sim.power_off_cycles = (long)off_cycles;
}

void alt_slot_dma(alt_machine_t *machine, uint64_t at_cycle, uint32_t src, uint32_t dst, uint32_t count) {
    machine->sim.slot_at = at_cycle;
    machine->sim.slot_regs[0] = src;
    machine->sim.slot_regs[1] = dst;
    machine->sim.slot_regs[2] = count;
    machine->sim.slot_regs[3] = 1;
}

alt_status_t alt_break_add(alt_machine_t *machine, uint32_t pc) {
    return debugger_add_breakpoint(&machine->sim.debugger, pc) >= 0 ? ALT_OK : ALT_ERR_FULL;
}

alt_status_t alt_vcd_open(alt_machine_t *machine, const char *path) {
    if (!path) return ALT_ERR_ARG;
    alt_vcd_close(machine);
    vcd_init(&machine->sim.vcd);
    return simulator_open_vcd(&machine->sim, path) ? ALT_OK : ALT_ERR_IO;
}

void alt_vcd_close(alt_machine_t *machine) {
    simulator_close_vcd(&machine->sim);
}

void alt_power_on(alt_machine_t *machine) {
    simulator_power_up(&machine->sim);
}

void alt_step(alt_machine_t *machine) {
    simulator_run_cycle(&machine->sim);
    if (machine->on_cycle) machine->on_cycle(machine->cycle_ctx, machine);
}

static void drain_uart(alt_machine_t *machine) {
    for (int i = 0; i < 12; i++) alt_step(machine);
}

alt_stop_t alt_run(alt_machine_t *machine, uint64_t max_cycles) {
    simulator_t *sim = &machine->sim;
    uint64_t end = sim->cycles + max_cycles;
    if (machine->at_break) debugger_continue(&sim->debugger);
    machine->at_break = 0;
    while (sim->cycles < end) {
        machine->at_break = debugger_check(&sim->debugger);
        if (machine->at_break) return ALT_STOP_BREAK;
        alt_step(machine);
        if (!sim->board.halt) continue;
        drain_uart(machine);
        return ALT_STOP_HALT;
    }
    return sim->board.halt ? ALT_STOP_HALT : ALT_STOP_LIMIT;
}

uint64_t alt_cycles(const alt_machine_t *machine) {
    return machine->sim.cycles;
}

int alt_halted(const alt_machine_t *machine) {
    return machine->sim.board.halt != 0;
}

alt_arch_t alt_arch(const alt_machine_t *machine) {
    return (alt_arch_t)machine->sim.arch;
}

long alt_hz(const alt_machine_t *machine) {
    return machine->sim.hz;
}

uint32_t alt_pc(alt_machine_t *machine) {
    return word_of(machine->sim.board.pc.reg.data_out, 32);
}

uint32_t alt_reg(alt_machine_t *machine, unsigned index) {
    if (index > 31) return 0;
    regfile_propagate(&machine->sim.board.regs);
    return word_of(machine->sim.board.regs.regs[index].data_out, 32);
}

alt_status_t alt_ram_read(alt_machine_t *machine, uint32_t addr, uint32_t *out_word) {
    if (!out_word || addr >= RAM_WORDS) return ALT_ERR_ARG;
    bit a[32], out[32];
    bits_of(addr, a, 32);
    ram_set_addr(&machine->sim.board.ram, a);
    ram_set_cs(&machine->sim.board.ram, 1);
    ram_propagate(&machine->sim.board.ram);
    ram_read(&machine->sim.board.ram, out);
    *out_word = word_of(out, 32);
    return ALT_OK;
}

static void write_sram(board_t *b, bit *a, bit *d) {
    ram_set_addr(&b->ram, a);
    ram_set_data(&b->ram, d);
    ram_set_cs(&b->ram, 1);
    ram_set_write(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_clock_fall(&b->ram);
    ram_clock_rise(&b->ram);
    ram_set_write(&b->ram, 0);
    ram_propagate(&b->ram);
}

static void update_l1(l1_cache_t *c, bit *a, bit *d) {
    l1_set_addr(c, a);
    l1_propagate(c);
    if (!(c->hit & 1)) return;
    l1_set_data(c, d);
    l1_set_write(c, 1);
    l1_clock_fall(c);
    l1_clock_rise(c);
    l1_set_write(c, 0);
    l1_propagate(c);
}

static void update_l2(l2_cache_t *c, bit *a, bit *d) {
    l2_set_addr(c, a);
    l2_propagate(c);
    if (!(c->hit & 1)) return;
    l2_set_data(c, d);
    l2_set_write(c, 1);
    l2_clock_fall(c);
    l2_clock_rise(c);
    l2_set_write(c, 0);
    l2_propagate(c);
}

alt_status_t alt_ram_write(alt_machine_t *machine, uint32_t addr, uint32_t word) {
    if (addr >= RAM_WORDS) return ALT_ERR_ARG;
    board_t *b = &machine->sim.board;
    bit a[32], d[32];
    bits_of(addr, a, 32);
    bits_of(word, d, 32);
    write_sram(b, a, d);
    update_l1(&b->l1, a, d);
    update_l2(&b->l2, a, d);
    return ALT_OK;
}

size_t alt_ram_words(void) {
    return RAM_WORDS;
}

unsigned char alt_display_char(alt_machine_t *machine, unsigned index) {
    if (index >= ALT_DISPLAY_CHARS) return 0;
    bit a[3];
    bits_of(index, a, 3);
    display_set_addr(&machine->sim.board.disp, a);
    display_propagate(&machine->sim.board.disp);
    return (unsigned char)word_of(machine->sim.board.disp.data_out, 8);
}

unsigned alt_signals(const alt_machine_t *machine) {
    const board_t *b = &machine->sim.board;
    unsigned s = 0;
    s |= (b->pic.int_request & 1) ? ALT_SIGNAL_IRQ : 0;
    s |= (b->exc.in_exception.q & 1) ? ALT_SIGNAL_EXCEPTION : 0;
    s |= (b->mmu_en.q & 1) ? ALT_SIGNAL_MMU : 0;
    s |= (b->umode.q & 1) ? ALT_SIGNAL_USER : 0;
    return s;
}

alt_status_t alt_stage(const alt_machine_t *machine, alt_stage_t stage, uint32_t *out_value, int *out_valid) {
    const board_t *b = &machine->sim.board;
    if (!out_value || !out_valid || (unsigned)stage > ALT_STAGE_WB) return ALT_ERR_ARG;
    const uint32_t values[] = {word_of_dffs(b->if_id.instruction, 32), word_of_dffs(b->id_ex.pc, 32),
                               word_of_dffs(b->ex_mem.pc, 32), word_of_dffs(b->mem_wb.write_reg, 5)};
    const bit valid[] = {b->if_id.valid.q, b->id_ex.valid.q, b->ex_mem.valid.q, b->mem_wb.valid.q};
    *out_value = values[stage];
    *out_valid = (valid[stage] & 1) != 0;
    return ALT_OK;
}

size_t alt_output(const alt_machine_t *machine, const char **out_bytes) {
    if (out_bytes) *out_bytes = machine->out;
    return machine->out_len;
}
