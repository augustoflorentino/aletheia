#ifndef ALETHEIA_CACHE_LINE_H
#define ALETHEIA_CACHE_LINE_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t valid;
    d_flip_flop_t dirty;
    d_flip_flop_t tag[20];
    d_flip_flop_t data[32];

    bit write_enable;
    bit set_valid;
    bit set_dirty;
    bit clear_dirty;
    bit tag_in[20];
    bit data_in[32];
} cache_line_t;

void cache_line_init(cache_line_t *line);
void cache_line_reset(cache_line_t *line);
void cache_line_set_write(cache_line_t *line, bit we);
void cache_line_set_valid(cache_line_t *line, bit valid);
void cache_line_set_dirty(cache_line_t *line, bit dirty, bit clear);
void cache_line_set_tag(cache_line_t *line, bit *tag);
void cache_line_set_data(cache_line_t *line, bit *data);
void cache_line_clock_rise(cache_line_t *line);
void cache_line_clock_fall(cache_line_t *line);

bit cache_line_get_valid(cache_line_t *line);
bit cache_line_get_dirty(cache_line_t *line);
void cache_line_get_tag(cache_line_t *line, bit *out);
void cache_line_get_data(cache_line_t *line, bit *out);

bit cache_line_tag_match(cache_line_t *line, bit *tag);

#endif
