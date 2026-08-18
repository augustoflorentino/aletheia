#include "interrupt_controller.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void intc_init(interrupt_controller_t *ic) {
    dff_reset(&ic->pending[0]); dff_reset(&ic->pending[1]);
    dff_reset(&ic->pending[2]); dff_reset(&ic->pending[3]);
    dff_reset(&ic->pending[4]); dff_reset(&ic->pending[5]);
    dff_reset(&ic->pending[6]); dff_reset(&ic->pending[7]);

    dff_reset(&ic->mask[0]); dff_reset(&ic->mask[1]);
    dff_reset(&ic->mask[2]); dff_reset(&ic->mask[3]);
    dff_reset(&ic->mask[4]); dff_reset(&ic->mask[5]);
    dff_reset(&ic->mask[6]); dff_reset(&ic->mask[7]);

    dff_reset(&ic->in_service[0]); dff_reset(&ic->in_service[1]);
    dff_reset(&ic->in_service[2]); dff_reset(&ic->in_service[3]);
    dff_reset(&ic->in_service[4]); dff_reset(&ic->in_service[5]);
    dff_reset(&ic->in_service[6]); dff_reset(&ic->in_service[7]);

    dff_reset(&ic->global_enable);
    dff_reset(&ic->irq_prev[0]);
    dff_reset(&ic->irq_prev[1]);
    dff_reset(&ic->irq_prev[2]);
    dff_reset(&ic->irq_prev[3]);
    dff_reset(&ic->irq_prev[4]);
    dff_reset(&ic->irq_prev[5]);
    dff_reset(&ic->irq_prev[6]);
    dff_reset(&ic->irq_prev[7]);
    ic->global_enable.q = 1;

    ic->irq_in[0] = 0; ic->irq_in[1] = 0; ic->irq_in[2] = 0; ic->irq_in[3] = 0;
    ic->irq_in[4] = 0; ic->irq_in[5] = 0; ic->irq_in[6] = 0; ic->irq_in[7] = 0;

    ic->ack = 0;
    ic->ack_num[0] = 0; ic->ack_num[1] = 0; ic->ack_num[2] = 0;
    ic->service = 0;
    ic->service_num[0] = 0; ic->service_num[1] = 0; ic->service_num[2] = 0;

    ic->irq_out = 0;
    ic->vector[0] = 0; ic->vector[1] = 0; ic->vector[2] = 0;
    ic->has_pending = 0;
}

void intc_reset(interrupt_controller_t *ic) {
    intc_init(ic);
}

void intc_set_irq(interrupt_controller_t *ic, bit *irq) {
    ic->irq_in[0] = irq[0]; ic->irq_in[1] = irq[1];
    ic->irq_in[2] = irq[2]; ic->irq_in[3] = irq[3];
    ic->irq_in[4] = irq[4]; ic->irq_in[5] = irq[5];
    ic->irq_in[6] = irq[6]; ic->irq_in[7] = irq[7];
}

void intc_set_ack(interrupt_controller_t *ic, bit ack, bit *ack_num) {
    ic->ack = ack;
    ic->ack_num[0] = ack_num[0];
    ic->ack_num[1] = ack_num[1];
    ic->ack_num[2] = ack_num[2];
}

void intc_set_service(interrupt_controller_t *ic, bit service, bit *service_num) {
    ic->service = service;
    ic->service_num[0] = service_num[0];
    ic->service_num[1] = service_num[1];
    ic->service_num[2] = service_num[2];
}

void intc_set_mask(interrupt_controller_t *ic, bit *mask) {
    ic->mask[0].d = mask[0]; ic->mask[1].d = mask[1];
    ic->mask[2].d = mask[2]; ic->mask[3].d = mask[3];
    ic->mask[4].d = mask[4]; ic->mask[5].d = mask[5];
    ic->mask[6].d = mask[6]; ic->mask[7].d = mask[7];
}

void intc_set_global_enable(interrupt_controller_t *ic, bit enable) {
    ic->global_enable.d = enable;
}

void intc_clock_rise(interrupt_controller_t *ic) {
    dff_clock_rise(&ic->pending[0]); dff_clock_rise(&ic->pending[1]);
    dff_clock_rise(&ic->pending[2]); dff_clock_rise(&ic->pending[3]);
    dff_clock_rise(&ic->pending[4]); dff_clock_rise(&ic->pending[5]);
    dff_clock_rise(&ic->pending[6]); dff_clock_rise(&ic->pending[7]);

    dff_clock_rise(&ic->mask[0]); dff_clock_rise(&ic->mask[1]);
    dff_clock_rise(&ic->mask[2]); dff_clock_rise(&ic->mask[3]);
    dff_clock_rise(&ic->mask[4]); dff_clock_rise(&ic->mask[5]);
    dff_clock_rise(&ic->mask[6]); dff_clock_rise(&ic->mask[7]);

    dff_clock_rise(&ic->in_service[0]); dff_clock_rise(&ic->in_service[1]);
    dff_clock_rise(&ic->in_service[2]); dff_clock_rise(&ic->in_service[3]);
    dff_clock_rise(&ic->in_service[4]); dff_clock_rise(&ic->in_service[5]);
    dff_clock_rise(&ic->in_service[6]); dff_clock_rise(&ic->in_service[7]);

    dff_clock_rise(&ic->global_enable);
    dff_clock_rise(&ic->irq_prev[0]);
    dff_clock_rise(&ic->irq_prev[1]);
    dff_clock_rise(&ic->irq_prev[2]);
    dff_clock_rise(&ic->irq_prev[3]);
    dff_clock_rise(&ic->irq_prev[4]);
    dff_clock_rise(&ic->irq_prev[5]);
    dff_clock_rise(&ic->irq_prev[6]);
    dff_clock_rise(&ic->irq_prev[7]);
}

void intc_clock_fall(interrupt_controller_t *ic) {
    bit ack_sel[8];
    bit n0 = NOT(ic->ack_num[0]);
    bit n1 = NOT(ic->ack_num[1]);
    bit n2 = NOT(ic->ack_num[2]);

    ack_sel[0] = AND3(n2, n1, n0);
    ack_sel[1] = AND3(n2, n1, ic->ack_num[0]);
    ack_sel[2] = AND3(n2, ic->ack_num[1], n0);
    ack_sel[3] = AND3(n2, ic->ack_num[1], ic->ack_num[0]);
    ack_sel[4] = AND3(ic->ack_num[2], n1, n0);
    ack_sel[5] = AND3(ic->ack_num[2], n1, ic->ack_num[0]);
    ack_sel[6] = AND3(ic->ack_num[2], ic->ack_num[1], n0);
    ack_sel[7] = AND3(ic->ack_num[2], ic->ack_num[1], ic->ack_num[0]);

    bit clear0 = AND(ic->ack, ack_sel[0]);
    bit clear1 = AND(ic->ack, ack_sel[1]);
    bit clear2 = AND(ic->ack, ack_sel[2]);
    bit clear3 = AND(ic->ack, ack_sel[3]);
    bit clear4 = AND(ic->ack, ack_sel[4]);
    bit clear5 = AND(ic->ack, ack_sel[5]);
    bit clear6 = AND(ic->ack, ack_sel[6]);
    bit clear7 = AND(ic->ack, ack_sel[7]);

    bit svc_sel[8];
    bit sn0 = NOT(ic->service_num[0]);
    bit sn1 = NOT(ic->service_num[1]);
    bit sn2 = NOT(ic->service_num[2]);

    svc_sel[0] = AND3(sn2, sn1, sn0);
    svc_sel[1] = AND3(sn2, sn1, ic->service_num[0]);
    svc_sel[2] = AND3(sn2, ic->service_num[1], sn0);
    svc_sel[3] = AND3(sn2, ic->service_num[1], ic->service_num[0]);
    svc_sel[4] = AND3(ic->service_num[2], sn1, sn0);
    svc_sel[5] = AND3(ic->service_num[2], sn1, ic->service_num[0]);
    svc_sel[6] = AND3(ic->service_num[2], ic->service_num[1], sn0);
    svc_sel[7] = AND3(ic->service_num[2], ic->service_num[1], ic->service_num[0]);

    bit set0 = AND(ic->service, svc_sel[0]);
    bit set1 = AND(ic->service, svc_sel[1]);
    bit set2 = AND(ic->service, svc_sel[2]);
    bit set3 = AND(ic->service, svc_sel[3]);
    bit set4 = AND(ic->service, svc_sel[4]);
    bit set5 = AND(ic->service, svc_sel[5]);
    bit set6 = AND(ic->service, svc_sel[6]);
    bit set7 = AND(ic->service, svc_sel[7]);

    ic->in_service[0].d = mux2(mux2(ic->in_service[0].q, 0, clear0), 1, set0);
    ic->in_service[1].d = mux2(mux2(ic->in_service[1].q, 0, clear1), 1, set1);
    ic->in_service[2].d = mux2(mux2(ic->in_service[2].q, 0, clear2), 1, set2);
    ic->in_service[3].d = mux2(mux2(ic->in_service[3].q, 0, clear3), 1, set3);
    ic->in_service[4].d = mux2(mux2(ic->in_service[4].q, 0, clear4), 1, set4);
    ic->in_service[5].d = mux2(mux2(ic->in_service[5].q, 0, clear5), 1, set5);
    ic->in_service[6].d = mux2(mux2(ic->in_service[6].q, 0, clear6), 1, set6);
    ic->in_service[7].d = mux2(mux2(ic->in_service[7].q, 0, clear7), 1, set7);
    ic->pending[0].d = OR(AND(ic->pending[0].q, NOT(set0)), AND(ic->irq_in[0], NOT(ic->irq_prev[0].q)));
    ic->pending[1].d = OR(AND(ic->pending[1].q, NOT(set1)), AND(ic->irq_in[1], NOT(ic->irq_prev[1].q)));
    ic->pending[2].d = OR(AND(ic->pending[2].q, NOT(set2)), AND(ic->irq_in[2], NOT(ic->irq_prev[2].q)));
    ic->pending[3].d = OR(AND(ic->pending[3].q, NOT(set3)), AND(ic->irq_in[3], NOT(ic->irq_prev[3].q)));
    ic->pending[4].d = OR(AND(ic->pending[4].q, NOT(set4)), AND(ic->irq_in[4], NOT(ic->irq_prev[4].q)));
    ic->pending[5].d = OR(AND(ic->pending[5].q, NOT(set5)), AND(ic->irq_in[5], NOT(ic->irq_prev[5].q)));
    ic->pending[6].d = OR(AND(ic->pending[6].q, NOT(set6)), AND(ic->irq_in[6], NOT(ic->irq_prev[6].q)));
    ic->pending[7].d = OR(AND(ic->pending[7].q, NOT(set7)), AND(ic->irq_in[7], NOT(ic->irq_prev[7].q)));
    ic->irq_prev[0].d = ic->irq_in[0];
    ic->irq_prev[1].d = ic->irq_in[1];
    ic->irq_prev[2].d = ic->irq_in[2];
    ic->irq_prev[3].d = ic->irq_in[3];
    ic->irq_prev[4].d = ic->irq_in[4];
    ic->irq_prev[5].d = ic->irq_in[5];
    ic->irq_prev[6].d = ic->irq_in[6];
    ic->irq_prev[7].d = ic->irq_in[7];
    dff_clock_fall(&ic->pending[0]); dff_clock_fall(&ic->pending[1]);
    dff_clock_fall(&ic->pending[2]); dff_clock_fall(&ic->pending[3]);
    dff_clock_fall(&ic->pending[4]); dff_clock_fall(&ic->pending[5]);
    dff_clock_fall(&ic->pending[6]); dff_clock_fall(&ic->pending[7]);

    dff_clock_fall(&ic->mask[0]); dff_clock_fall(&ic->mask[1]);
    dff_clock_fall(&ic->mask[2]); dff_clock_fall(&ic->mask[3]);
    dff_clock_fall(&ic->mask[4]); dff_clock_fall(&ic->mask[5]);
    dff_clock_fall(&ic->mask[6]); dff_clock_fall(&ic->mask[7]);

    dff_clock_fall(&ic->in_service[0]); dff_clock_fall(&ic->in_service[1]);
    dff_clock_fall(&ic->in_service[2]); dff_clock_fall(&ic->in_service[3]);
    dff_clock_fall(&ic->in_service[4]); dff_clock_fall(&ic->in_service[5]);
    dff_clock_fall(&ic->in_service[6]); dff_clock_fall(&ic->in_service[7]);

    dff_clock_fall(&ic->global_enable);
    dff_clock_fall(&ic->irq_prev[0]);
    dff_clock_fall(&ic->irq_prev[1]);
    dff_clock_fall(&ic->irq_prev[2]);
    dff_clock_fall(&ic->irq_prev[3]);
    dff_clock_fall(&ic->irq_prev[4]);
    dff_clock_fall(&ic->irq_prev[5]);
    dff_clock_fall(&ic->irq_prev[6]);
    dff_clock_fall(&ic->irq_prev[7]);
}

static void lowest_set8(bit *in, bit *num, bit *any) {
    bit h1 = in[0];
    bit h2 = OR(h1, in[1]);
    bit h3 = OR(h2, in[2]);
    bit h4 = OR(h3, in[3]);
    bit h5 = OR(h4, in[4]);
    bit h6 = OR(h5, in[5]);
    bit h7 = OR(h6, in[6]);
    bit f1 = AND(in[1], NOT(h1));
    bit f2 = AND(in[2], NOT(h2));
    bit f3 = AND(in[3], NOT(h3));
    bit f4 = AND(in[4], NOT(h4));
    bit f5 = AND(in[5], NOT(h5));
    bit f6 = AND(in[6], NOT(h6));
    bit f7 = AND(in[7], NOT(h7));
    num[0] = OR(OR(f1, f3), OR(f5, f7));
    num[1] = OR(OR(f2, f3), OR(f6, f7));
    num[2] = OR(OR(f4, f5), OR(f6, f7));
    *any = OR(h7, in[7]);
}

void intc_propagate(interrupt_controller_t *ic) {
    bit b0 = ic->in_service[0].q;
    bit b1 = OR(b0, ic->in_service[1].q);
    bit b2 = OR(b1, ic->in_service[2].q);
    bit b3 = OR(b2, ic->in_service[3].q);
    bit b4 = OR(b3, ic->in_service[4].q);
    bit b5 = OR(b4, ic->in_service[5].q);
    bit b6 = OR(b5, ic->in_service[6].q);
    bit b7 = OR(b6, ic->in_service[7].q);
    bit active[8];
    active[0] = AND(AND(ic->pending[0].q, ic->mask[0].q), NOT(b0));
    active[1] = AND(AND(ic->pending[1].q, ic->mask[1].q), NOT(b1));
    active[2] = AND(AND(ic->pending[2].q, ic->mask[2].q), NOT(b2));
    active[3] = AND(AND(ic->pending[3].q, ic->mask[3].q), NOT(b3));
    active[4] = AND(AND(ic->pending[4].q, ic->mask[4].q), NOT(b4));
    active[5] = AND(AND(ic->pending[5].q, ic->mask[5].q), NOT(b5));
    active[6] = AND(AND(ic->pending[6].q, ic->mask[6].q), NOT(b6));
    active[7] = AND(AND(ic->pending[7].q, ic->mask[7].q), NOT(b7));
    bit any_active;
    lowest_set8(active, ic->vector, &any_active);
    ic->irq_out = AND(any_active, ic->global_enable.q);
    ic->has_pending = any_active;
}

bit intc_get_irq_out(interrupt_controller_t *ic) {
    return ic->irq_out;
}

void intc_get_vector(interrupt_controller_t *ic, bit *out) {
    out[0] = ic->vector[0];
    out[1] = ic->vector[1];
    out[2] = ic->vector[2];
}

bit intc_has_pending(interrupt_controller_t *ic) {
    return ic->has_pending;
}

void intc_get_top_service(interrupt_controller_t *ic, bit *num, bit *any) {
    bit svc[8];
    svc[0] = ic->in_service[0].q;
    svc[1] = ic->in_service[1].q;
    svc[2] = ic->in_service[2].q;
    svc[3] = ic->in_service[3].q;
    svc[4] = ic->in_service[4].q;
    svc[5] = ic->in_service[5].q;
    svc[6] = ic->in_service[6].q;
    svc[7] = ic->in_service[7].q;
    lowest_set8(svc, num, any);
}
