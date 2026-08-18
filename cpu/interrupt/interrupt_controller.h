#ifndef ALETHEIA_INTERRUPT_CONTROLLER_H
#define ALETHEIA_INTERRUPT_CONTROLLER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define NUM_INTERRUPTS 8

typedef struct {
    d_flip_flop_t pending[NUM_INTERRUPTS];
    d_flip_flop_t mask[NUM_INTERRUPTS];
    d_flip_flop_t in_service[NUM_INTERRUPTS];
    d_flip_flop_t global_enable;
    d_flip_flop_t irq_prev[NUM_INTERRUPTS];

    bit irq_in[NUM_INTERRUPTS];
    bit ack;
    bit ack_num[3];
    bit service;
    bit service_num[3];

    bit irq_out;
    bit vector[3];
    bit has_pending;
} interrupt_controller_t;

void intc_init(interrupt_controller_t *ic);
void intc_reset(interrupt_controller_t *ic);

void intc_set_irq(interrupt_controller_t *ic, bit *irq);
void intc_set_ack(interrupt_controller_t *ic, bit ack, bit *ack_num);
void intc_set_service(interrupt_controller_t *ic, bit service, bit *service_num);
void intc_set_mask(interrupt_controller_t *ic, bit *mask);
void intc_set_global_enable(interrupt_controller_t *ic, bit enable);

void intc_clock_rise(interrupt_controller_t *ic);
void intc_clock_fall(interrupt_controller_t *ic);
void intc_propagate(interrupt_controller_t *ic);

bit intc_get_irq_out(interrupt_controller_t *ic);
void intc_get_vector(interrupt_controller_t *ic, bit *out);
bit intc_has_pending(interrupt_controller_t *ic);
void intc_get_top_service(interrupt_controller_t *ic, bit *num, bit *any);

#endif
