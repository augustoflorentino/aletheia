#ifndef ALETHEIA_MEMORY_CELL_H
#define ALETHEIA_MEMORY_CELL_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t data;
    bit write_enable;
    bit data_in;
} memory_cell_t;

typedef struct {
    d_flip_flop_t bits[8];
    bit write_enable;
    bit data_in[8];
} memory_cell_8_t;

typedef struct {
    d_flip_flop_t bits[32];
    bit write_enable;
    bit data_in[32];
} memory_cell_32_t;

void cell_init(memory_cell_t *cell);
void cell_reset(memory_cell_t *cell);
void cell_set_write(memory_cell_t *cell, bit we, bit data);
void cell_clock_rise(memory_cell_t *cell);
void cell_clock_fall(memory_cell_t *cell);
bit cell_read(memory_cell_t *cell);

void cell8_init(memory_cell_8_t *cell);
void cell8_reset(memory_cell_8_t *cell);
void cell8_set_write(memory_cell_8_t *cell, bit we, bit *data);
void cell8_clock_rise(memory_cell_8_t *cell);
void cell8_clock_fall(memory_cell_8_t *cell);
void cell8_read(memory_cell_8_t *cell, bit *out);

void cell32_init(memory_cell_32_t *cell);
void cell32_reset(memory_cell_32_t *cell);
void cell32_set_write(memory_cell_32_t *cell, bit we, bit *data);
void cell32_clock_rise(memory_cell_32_t *cell);
void cell32_clock_fall(memory_cell_32_t *cell);
void cell32_read(memory_cell_32_t *cell, bit *out);

#endif
