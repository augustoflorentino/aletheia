#include "waveform.h"
#include <string.h>

void vcd_init(vcd_writer_t *vcd) {
    vcd->file = 0;
    vcd->signal_count = 0;
    vcd->last_time = 0;
    vcd->enabled = 0;
    vcd->next_id = '!';
}

int vcd_open(vcd_writer_t *vcd, const char *filename) {
    vcd->file = fopen(filename, "w");
    if (vcd->file == 0) {
        return 0;
    }
    vcd->enabled = 1;
    return 1;
}

void vcd_close(vcd_writer_t *vcd) {
    if (vcd->file != 0) {
        fclose(vcd->file);
        vcd->file = 0;
    }
    vcd->enabled = 0;
}

void vcd_set_timescale(vcd_writer_t *vcd, const char *scale) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$timescale %s $end\n", scale);
    }
}

void vcd_begin_module(vcd_writer_t *vcd, const char *name) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$scope module %s $end\n", name);
    }
}

void vcd_end_module(vcd_writer_t *vcd) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$upscope $end\n");
    }
}

int vcd_add_signal(vcd_writer_t *vcd, const char *name, int width, bit *value_ptr) {
    if (vcd->signal_count >= MAX_VCD_SIGNALS) {
        return -1;
    }

    int idx = vcd->signal_count;
    strncpy(vcd->signals[idx].name, name, MAX_SIGNAL_NAME_LEN - 1);
    vcd->signals[idx].name[MAX_SIGNAL_NAME_LEN - 1] = '\0';
    vcd->signals[idx].width = width;
    vcd->signals[idx].id = vcd->next_id;
    vcd->signals[idx].value_ptr = value_ptr;
    vcd->signals[idx].last_value = 2;

    vcd->next_id++;
    if (vcd->next_id > '~') {
        vcd->next_id = '!';
    }

    vcd->signal_count++;

    if (vcd->file != 0) {
        fprintf(vcd->file, "$var wire %d %c %s $end\n",
                vcd->signals[idx].width, vcd->signals[idx].id, vcd->signals[idx].name);
    }

    return idx;
}

void vcd_write_header(vcd_writer_t *vcd) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$date\n   Aletheia Simulation\n$end\n");
        fprintf(vcd->file, "$version\n   Aletheia 1.0\n$end\n");
    }
}

void vcd_write_initial(vcd_writer_t *vcd) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$enddefinitions $end\n");
        fprintf(vcd->file, "$dumpvars\n");
        for (int i = 0; i < vcd->signal_count; i++) {
            if (vcd->signals[i].width == 1) {
                fprintf(vcd->file, "%d%c\n",
                        vcd->signals[i].value_ptr ? *vcd->signals[i].value_ptr : 0,
                        vcd->signals[i].id);
            }
            vcd->signals[i].last_value = vcd->signals[i].value_ptr ? *vcd->signals[i].value_ptr : 0;
        }
        fprintf(vcd->file, "$end\n");
    }
}

void vcd_write_time(vcd_writer_t *vcd, sim_time_t time) {
    if (vcd->file != 0 && time != vcd->last_time) {
        fprintf(vcd->file, "#%llu\n", time);
        vcd->last_time = time;
    }
}

void vcd_write_value(vcd_writer_t *vcd, int signal_idx) {
    if (vcd->file != 0 && signal_idx >= 0 && signal_idx < vcd->signal_count) {
        vcd_signal_t *sig = &vcd->signals[signal_idx];
        if (sig->width == 1) {
            fprintf(vcd->file, "%d%c\n", sig->value_ptr ? *sig->value_ptr : 0, sig->id);
        }
    }
}

void vcd_update(vcd_writer_t *vcd, sim_time_t time) {
    if (!vcd->enabled || vcd->file == 0) {
        return;
    }

    int time_written = 0;

    for (int i = 0; i < vcd->signal_count; i++) {
        vcd_signal_t *sig = &vcd->signals[i];
        bit current = sig->value_ptr ? *sig->value_ptr : 0;
        if (current != sig->last_value) {
            if (!time_written) {
                vcd_write_time(vcd, time);
                time_written = 1;
            }
            vcd_write_value(vcd, i);
            sig->last_value = current;
        }
    }
}

void vcd_force_dump(vcd_writer_t *vcd, sim_time_t time) {
    if (!vcd->enabled || vcd->file == 0) {
        return;
    }

    vcd_write_time(vcd, time);
    for (int i = 0; i < vcd->signal_count; i++) {
        vcd_write_value(vcd, i);
        vcd->signals[i].last_value = vcd->signals[i].value_ptr ? *vcd->signals[i].value_ptr : 0;
    }
}
