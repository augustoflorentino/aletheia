#include "panel.h"
#include <stdio.h>
#include <string.h>

static const char *reg_names[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

static const char *arch_names[] = {"single", "multi", "pipe"};

void panel_init(panel_t *p, const uint32_t *rom) {
    memset(p, 0, sizeof *p);
    p->rom = rom;
}

static void scroll(panel_t *p) {
    memmove(p->term[0], p->term[1], sizeof p->term[0] * (PANEL_TERM_ROWS - 1));
    memset(p->term[PANEL_TERM_ROWS - 1], 0, sizeof p->term[0]);
    p->row = PANEL_TERM_ROWS - 1;
}

void panel_output(void *ctx, int byte) {
    panel_t *p = ctx;
    if (byte == '\n') {
        p->col = 0;
        p->row++;
        if (p->row >= PANEL_TERM_ROWS) scroll(p);
        return;
    }
    if (byte == 8) {
        if (p->col > 0) p->col--;
        return;
    }
    if (byte < 32 || byte > 126) return;
    if (p->col >= PANEL_TERM_COLS) panel_output(ctx, '\n');
    p->term[p->row][p->col++] = (char)byte;
}

void panel_open(void) {
    printf("\x1b[?25l\x1b[2J");
}

void panel_close(void) {
    printf("\x1b[?25h\x1b[%d;1H\n", 26);
    fflush(stdout);
}

static void draw_display(alt_machine_t *m) {
    printf("DISPLAY [");
    for (unsigned i = 0; i < ALT_DISPLAY_CHARS; i++) {
        unsigned ch = alt_display_char(m, i);
        putchar(ch >= 32 && ch < 127 ? (int)ch : ' ');
    }
    unsigned s = alt_signals(m);
    printf("]   IRQ %d  EXC %d  MMU %d\x1b[K\n", (s & ALT_SIGNAL_IRQ) != 0, (s & ALT_SIGNAL_EXCEPTION) != 0,
           (s & ALT_SIGNAL_MMU) != 0);
}

static const char *mark(const alt_machine_t *m, alt_stage_t stage, uint32_t *value) {
    int valid;
    alt_stage(m, stage, value, &valid);
    return valid ? "*" : "-";
}

static void draw_pipeline(alt_machine_t *m) {
    uint32_t id, ex, mem, wb;
    const char *id_mark = mark(m, ALT_STAGE_ID, &id);
    const char *ex_mark = mark(m, ALT_STAGE_EX, &ex);
    const char *mem_mark = mark(m, ALT_STAGE_MEM, &mem);
    const char *wb_mark = mark(m, ALT_STAGE_WB, &wb);
    char dis[64];
    alt_disassemble(id, dis, sizeof dis);
    printf("PIPE  ID %s %-22s EX %s %04X  MEM %s %04X  WB %s r%-2u\x1b[K\n", id_mark, dis, ex_mark,
           (unsigned)(ex & 0xFFFF), mem_mark, (unsigned)((mem - 1) & 0xFFFF), wb_mark, (unsigned)wb);
}

void panel_draw(panel_t *p, alt_machine_t *m) {
    uint32_t pc = alt_pc(m);
    uint32_t instr = p->rom[pc & (ALT_ROM_WORDS - 1)];
    char dis[64];
    alt_disassemble(instr, dis, sizeof dis);

    printf("\x1b[H\x1b[7m ALETHEIA  %-6s  cycle %-10llu %8.0f Hz", arch_names[alt_arch(m)],
           (unsigned long long)alt_cycles(m), p->rate);
    printf("  limit %-8ld %-6s\x1b[0m\x1b[K\n", alt_hz(m), p->paused ? "PAUSED" : (alt_halted(m) ? "HALT" : "RUN"));
    printf("PC %08X  %08X  %s\x1b[K\n\n", (unsigned)pc, (unsigned)instr, dis);
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 4; c++) {
            unsigned i = (unsigned)(c * 8 + r);
            printf(" %4s %08X ", reg_names[i], (unsigned)alt_reg(m, i));
        }
        printf("\x1b[K\n");
    }
    printf("\n");
    draw_display(m);
    if (alt_arch(m) == ALT_ARCH_PIPE) draw_pipeline(m);
    else printf("\x1b[K\n");
    printf("UART %.*s\x1b[K\n", PANEL_TERM_COLS - 5,
           "------------------------------------------------------------------------------");
    for (int r = 0; r < PANEL_TERM_ROWS; r++) printf(" %s\x1b[K\n", p->term[r]);
    printf("\x1b[7m ^P run/pause  ^N step  ^F faster  ^B slower  ^C quit   other keys -> UART \x1b[0m\x1b[K");
    fflush(stdout);
}
