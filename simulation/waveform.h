#ifndef ALETHEIA_WAVEFORM_H
#define ALETHEIA_WAVEFORM_H

#include <stdio.h>
#include <stdint.h>
#include "../core/types.h"
#include "scheduler.h"

#define MAX_VCD_SIGNALS     256
#define MAX_SIGNAL_NAME_LEN 64

#define VCD_ID_LEN          4

typedef struct {
    char name[MAX_SIGNAL_NAME_LEN];
    int width;
    char id[VCD_ID_LEN];
    bit *value_ptr;
    uint32_t last_value;
    int dumped;
} vcd_signal_t;

typedef struct {
    FILE *file;
    vcd_signal_t signals[MAX_VCD_SIGNALS];
    int signal_count;
    sim_time_t last_time;
    int time_written;
    int enabled;
} vcd_writer_t;

void vcd_init(vcd_writer_t *vcd);
int vcd_open(vcd_writer_t *vcd, const char *filename);
void vcd_close(vcd_writer_t *vcd);
void vcd_set_timescale(vcd_writer_t *vcd, const char *scale);
void vcd_begin_module(vcd_writer_t *vcd, const char *name);
void vcd_end_module(vcd_writer_t *vcd);
int vcd_add_signal(vcd_writer_t *vcd, const char *name, int width, bit *value_ptr);
void vcd_write_header(vcd_writer_t *vcd);
void vcd_write_initial(vcd_writer_t *vcd);
void vcd_write_time(vcd_writer_t *vcd, sim_time_t time);
void vcd_write_value(vcd_writer_t *vcd, int signal_idx);
void vcd_update(vcd_writer_t *vcd, sim_time_t time);
void vcd_force_dump(vcd_writer_t *vcd, sim_time_t time);

#endif
