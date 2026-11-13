#include "waveform.h"
#include <string.h>

void vcd_init(vcd_writer_t *vcd) {
    vcd->file = 0;
    vcd->signal_count = 0;
    vcd->last_time = 0;
    vcd->time_written = 0;
    vcd->enabled = 0;
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

static void make_id(int index, char *id) {
    int n = 0;
    do {
        id[n++] = (char)('!' + index % 94);
        index /= 94;
    } while (index > 0 && n < VCD_ID_LEN - 1);
    id[n] = '\0';
}

static uint32_t sample(const vcd_signal_t *sig) {
    uint32_t v = 0;
    for (int i = 0; i < sig->width; i++) {
        v |= (uint32_t)(*(const bit *)((const char *)sig->value_ptr + (size_t)i * sig->stride) & 1) << i;
    }
    return v;
}

int vcd_add_signal(vcd_writer_t *vcd, const char *name, int width, bit *value_ptr) {
    return vcd_add_signal_stride(vcd, name, width, value_ptr, sizeof(bit));
}

int vcd_add_signal_stride(vcd_writer_t *vcd, const char *name, int width, bit *value_ptr, size_t stride) {
    if (vcd->signal_count >= MAX_VCD_SIGNALS || width < 1 || width > 32 || value_ptr == 0) {
        return -1;
    }

    int idx = vcd->signal_count;
    vcd_signal_t *sig = &vcd->signals[idx];
    strncpy(sig->name, name, MAX_SIGNAL_NAME_LEN - 1);
    sig->name[MAX_SIGNAL_NAME_LEN - 1] = '\0';
    sig->width = width;
    make_id(idx, sig->id);
    sig->value_ptr = value_ptr;
    sig->stride = stride;
    sig->last_value = 0;
    sig->dumped = 0;
    vcd->signal_count++;

    if (vcd->file != 0) {
        fprintf(vcd->file, "$var wire %d %s %s $end\n", sig->width, sig->id, sig->name);
    }
    return idx;
}

void vcd_write_header(vcd_writer_t *vcd) {
    if (vcd->file != 0) {
        fprintf(vcd->file, "$date\n   Aletheia Simulation\n$end\n");
        fprintf(vcd->file, "$version\n   Aletheia 1.0\n$end\n");
    }
}

static void emit(vcd_writer_t *vcd, vcd_signal_t *sig, uint32_t v) {
    if (sig->width == 1) {
        fprintf(vcd->file, "%u%s\n", v & 1u, sig->id);
    } else {
        fputc('b', vcd->file);
        for (int i = sig->width - 1; i >= 0; i--) {
            fputc((v >> i) & 1u ? '1' : '0', vcd->file);
        }
        fprintf(vcd->file, " %s\n", sig->id);
    }
    sig->last_value = v;
    sig->dumped = 1;
}

void vcd_write_initial(vcd_writer_t *vcd) {
    if (vcd->file == 0) {
        return;
    }
    fprintf(vcd->file, "$enddefinitions $end\n#0\n$dumpvars\n");
    for (int i = 0; i < vcd->signal_count; i++) {
        emit(vcd, &vcd->signals[i], sample(&vcd->signals[i]));
    }
    fprintf(vcd->file, "$end\n");
    vcd->last_time = 0;
    vcd->time_written = 1;
}

void vcd_write_time(vcd_writer_t *vcd, sim_time_t time) {
    if (vcd->file != 0 && (time != vcd->last_time || !vcd->time_written)) {
        fprintf(vcd->file, "#%llu\n", time);
        vcd->last_time = time;
        vcd->time_written = 1;
    }
}

void vcd_write_value(vcd_writer_t *vcd, int signal_idx) {
    if (vcd->file != 0 && signal_idx >= 0 && signal_idx < vcd->signal_count) {
        vcd_signal_t *sig = &vcd->signals[signal_idx];
        emit(vcd, sig, sample(sig));
    }
}

void vcd_update(vcd_writer_t *vcd, sim_time_t time) {
    if (!vcd->enabled || vcd->file == 0) {
        return;
    }
    for (int i = 0; i < vcd->signal_count; i++) {
        vcd_signal_t *sig = &vcd->signals[i];
        uint32_t current = sample(sig);
        if (current != sig->last_value || !sig->dumped) {
            vcd_write_time(vcd, time);
            emit(vcd, sig, current);
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
    }
}
