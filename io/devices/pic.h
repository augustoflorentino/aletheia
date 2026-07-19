#ifndef ALETHEIA_PIC_H
#define ALETHEIA_PIC_H

#include "../../core/types.h"
#include "../../cpu/interrupt/interrupt_controller.h"

typedef struct {
    interrupt_controller_t ic;

    bit int_request;
    bit int_vector[3];
} pic_t;

void pic_init(pic_t *p);
void pic_reset(pic_t *p);
void pic_raise(pic_t *p, bit *irq_lines);
void pic_write_mask(pic_t *p, bit *mask);
void pic_set_enable(pic_t *p, bit en);
void pic_ack(pic_t *p, bit *vector);
void pic_clock_fall(pic_t *p);
void pic_clock_rise(pic_t *p);
void pic_propagate(pic_t *p);

#endif
