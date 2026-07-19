#include "cache_controller.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

static void low4(bit *addr, bit *out) {
    out[0] = addr[0]; out[1] = addr[1]; out[2] = addr[2]; out[3] = addr[3];
}

void cc_init(cache_controller_t *cc) {
    l1_init(&cc->l1);
    sram_init(&cc->mem);
}

void cc_reset(cache_controller_t *cc) {
    l1_reset(&cc->l1);
    sram_reset(&cc->mem);
}

void cc_mem_write(cache_controller_t *cc, bit *addr, bit *data) {
    bit a4[4];
    low4(addr, a4);
    sram_set_addr(&cc->mem, a4);
    sram_set_data(&cc->mem, data);
    sram_set_write(&cc->mem, 1);
    sram_set_cs(&cc->mem, 1);
    sram_clock_fall(&cc->mem);
    sram_clock_rise(&cc->mem);
    sram_set_write(&cc->mem, 0);
}

void cc_access(cache_controller_t *cc, bit *addr) {
    l1_set_addr(&cc->l1, addr);
    l1_set_write(&cc->l1, 0);
    l1_propagate(&cc->l1);
    bit hit = cc->l1.hit;

    bit a4[4];
    low4(addr, a4);
    sram_set_addr(&cc->mem, a4);
    sram_set_write(&cc->mem, 0);
    sram_set_cs(&cc->mem, 1);
    sram_propagate(&cc->mem);
    bit mem_data[32];
    sram_read(&cc->mem, mem_data);

    mux2_32(mem_data, cc->l1.data_out, hit, cc->data_out);

    l1_set_addr(&cc->l1, addr);
    l1_set_data(&cc->l1, mem_data);
    l1_set_write(&cc->l1, NOT(hit));
    l1_clock_fall(&cc->l1);
    l1_clock_rise(&cc->l1);
    l1_set_write(&cc->l1, 0);

    cc->hit_out = hit;
}
