#include "pic.h"

void pic_init(pic_t *p) {
    intc_init(&p->ic);
    p->int_request = 0;
    p->int_vector[0] = 0;
    p->int_vector[1] = 0;
    p->int_vector[2] = 0;
}

void pic_reset(pic_t *p) {
    intc_reset(&p->ic);
    p->int_request = 0;
    p->int_vector[0] = 0;
    p->int_vector[1] = 0;
    p->int_vector[2] = 0;
}

void pic_raise(pic_t *p, bit *irq_lines) {
    intc_set_irq(&p->ic, irq_lines);
}

void pic_write_mask(pic_t *p, bit *mask) {
    intc_set_mask(&p->ic, mask);
}

void pic_set_enable(pic_t *p, bit en) {
    intc_set_global_enable(&p->ic, en);
}

void pic_ack(pic_t *p, bit *vector) {
    intc_set_ack(&p->ic, 1, vector);
}

void pic_clock_fall(pic_t *p) {
    intc_clock_fall(&p->ic);
}

void pic_clock_rise(pic_t *p) {
    intc_clock_rise(&p->ic);
}

void pic_propagate(pic_t *p) {
    intc_propagate(&p->ic);
    p->int_request = intc_get_irq_out(&p->ic);
    intc_get_vector(&p->ic, p->int_vector);
}
