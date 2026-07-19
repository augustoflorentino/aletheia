#ifndef ALETHEIA_DMA_H
#define ALETHEIA_DMA_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t src_addr[16];
    d_flip_flop_t dst_addr[16];
    d_flip_flop_t count[16];
    d_flip_flop_t control[8];
    d_flip_flop_t status[8];

    bit src_in[16];
    bit dst_in[16];
    bit count_in[16];
    bit control_in[8];

    bit src_out[16];
    bit dst_out[16];
    bit count_out[16];
    bit status_out[8];

    bit data_in[32];
    bit data_out[32];
    bit addr_out[16];

    bit chip_select;
    bit read;
    bit write;
    bit addr[3];

    bit dreq;
    bit dack;
    bit tc;
    bit bus_request;
    bit bus_grant;
    bit mem_read;
    bit mem_write;
} dma_controller_t;

void dma_init(dma_controller_t *dma);
void dma_reset(dma_controller_t *dma);
void dma_set_cs(dma_controller_t *dma, bit cs);
void dma_set_read(dma_controller_t *dma, bit rd);
void dma_set_write(dma_controller_t *dma, bit wr);
void dma_set_addr(dma_controller_t *dma, bit *addr);
void dma_set_cpu_data(dma_controller_t *dma, bit *data);
void dma_set_mem_data(dma_controller_t *dma, bit *data);
void dma_set_dreq(dma_controller_t *dma, bit dreq);
void dma_set_bus_grant(dma_controller_t *dma, bit grant);
void dma_clock_rise(dma_controller_t *dma);
void dma_clock_fall(dma_controller_t *dma);
void dma_propagate(dma_controller_t *dma);
void dma_read_data(dma_controller_t *dma, bit *out);
void dma_read_addr(dma_controller_t *dma, bit *out);
bit dma_get_bus_request(dma_controller_t *dma);
bit dma_get_dack(dma_controller_t *dma);
bit dma_get_tc(dma_controller_t *dma);
bit dma_get_mem_read(dma_controller_t *dma);
bit dma_get_mem_write(dma_controller_t *dma);

#endif
