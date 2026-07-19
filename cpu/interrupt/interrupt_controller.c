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

    ic->pending[0].d = AND(OR(ic->pending[0].q, ic->irq_in[0]), NOT(clear0));
    ic->pending[1].d = AND(OR(ic->pending[1].q, ic->irq_in[1]), NOT(clear1));
    ic->pending[2].d = AND(OR(ic->pending[2].q, ic->irq_in[2]), NOT(clear2));
    ic->pending[3].d = AND(OR(ic->pending[3].q, ic->irq_in[3]), NOT(clear3));
    ic->pending[4].d = AND(OR(ic->pending[4].q, ic->irq_in[4]), NOT(clear4));
    ic->pending[5].d = AND(OR(ic->pending[5].q, ic->irq_in[5]), NOT(clear5));
    ic->pending[6].d = AND(OR(ic->pending[6].q, ic->irq_in[6]), NOT(clear6));
    ic->pending[7].d = AND(OR(ic->pending[7].q, ic->irq_in[7]), NOT(clear7));

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
}

void intc_propagate(interrupt_controller_t *ic) {
    bit active[8];
    active[0] = AND(AND(ic->pending[0].q, ic->mask[0].q), NOT(ic->in_service[0].q));
    active[1] = AND(AND(ic->pending[1].q, ic->mask[1].q), NOT(ic->in_service[1].q));
    active[2] = AND(AND(ic->pending[2].q, ic->mask[2].q), NOT(ic->in_service[2].q));
    active[3] = AND(AND(ic->pending[3].q, ic->mask[3].q), NOT(ic->in_service[3].q));
    active[4] = AND(AND(ic->pending[4].q, ic->mask[4].q), NOT(ic->in_service[4].q));
    active[5] = AND(AND(ic->pending[5].q, ic->mask[5].q), NOT(ic->in_service[5].q));
    active[6] = AND(AND(ic->pending[6].q, ic->mask[6].q), NOT(ic->in_service[6].q));
    active[7] = AND(AND(ic->pending[7].q, ic->mask[7].q), NOT(ic->in_service[7].q));

    bit any_active = OR(OR(OR(active[0], active[1]), OR(active[2], active[3])),
                        OR(OR(active[4], active[5]), OR(active[6], active[7])));

    bit v0_from_1 = AND(active[1], NOT(active[0]));
    bit v0_from_3 = AND(active[3], AND(NOT(active[0]), AND(NOT(active[1]), NOT(active[2]))));
    bit v0_from_5 = AND(active[5], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]), NOT(active[4]))))));
    bit v0_from_7 = AND(active[7], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]),
                        AND(NOT(active[4]), AND(NOT(active[5]), NOT(active[6]))))))));

    bit v1_from_2 = AND(active[2], AND(NOT(active[0]), NOT(active[1])));
    bit v1_from_3 = AND(active[3], AND(NOT(active[0]), AND(NOT(active[1]), NOT(active[2]))));
    bit v1_from_6 = AND(active[6], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]),
                        AND(NOT(active[4]), NOT(active[5])))))));
    bit v1_from_7 = AND(active[7], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]),
                        AND(NOT(active[4]), AND(NOT(active[5]), NOT(active[6]))))))));

    bit v2_from_4 = AND(active[4], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), NOT(active[3])))));
    bit v2_from_5 = AND(active[5], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]), NOT(active[4]))))));
    bit v2_from_6 = AND(active[6], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]),
                        AND(NOT(active[4]), NOT(active[5])))))));
    bit v2_from_7 = AND(active[7], AND(NOT(active[0]), AND(NOT(active[1]),
                        AND(NOT(active[2]), AND(NOT(active[3]),
                        AND(NOT(active[4]), AND(NOT(active[5]), NOT(active[6]))))))));

    ic->vector[0] = OR(OR(v0_from_1, v0_from_3), OR(v0_from_5, v0_from_7));
    ic->vector[1] = OR(OR(v1_from_2, v1_from_3), OR(v1_from_6, v1_from_7));
    ic->vector[2] = OR(OR(v2_from_4, v2_from_5), OR(v2_from_6, v2_from_7));

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
