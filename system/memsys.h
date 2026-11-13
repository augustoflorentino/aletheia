#ifndef ALETHEIA_MEMSYS_H
#define ALETHEIA_MEMSYS_H

#include "board.h"

typedef struct {
    bit vaddr[32];
    bit wdata[32];
    bit read;
    bit write;
    bit io_sel;
    bit translate;
    bit fault;
    bit is_uart;
    bit is_gpio;
    bit is_disp;
    bit sel_timer;
    bit sel_picmask;
    bit sel_ptfill;
    bit sel_mmuen;
    bit sel_mode;
    bit sel_dma;
    bit is_dram;
    bit l1_hit;
    bit l2_hit;
    bit sram_data[32];
    bit rdata[32];
} mem_port_t;

typedef struct {
    bit take;
    bit ok;
    bit reti;
} exc_outcome_t;

void memsys_irq(board_t *b);
void memsys_probe(board_t *b, bit *vaddr, bit *wdata, bit read, bit write, mem_port_t *p);
void memsys_watch_instr(board_t *b, bit *instr);
void memsys_commit(board_t *b, mem_port_t *p, bit enable);
void board_exceptions(board_t *b, bit *epc, bit fault, bit undefined, bit irq_allowed,
                      bit reti, bit run, exc_outcome_t *out);
bit memsys_bus_hold(board_t *b);
void memsys_dma_cycle(board_t *b);
void memsys_clock_fall(board_t *b);
void memsys_clock_rise(board_t *b);

#endif
