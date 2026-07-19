#include "arbiter.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/mux/mux2.h"

void arbiter_init(arbiter_t *arb) {
    dff_reset(&arb->grant[0]); dff_reset(&arb->grant[1]);
    dff_reset(&arb->grant[2]); dff_reset(&arb->grant[3]);
    dff_reset(&arb->busy);

    arb->request[0] = 0; arb->request[1] = 0;
    arb->request[2] = 0; arb->request[3] = 0;

    arb->grant_out[0] = 0; arb->grant_out[1] = 0;
    arb->grant_out[2] = 0; arb->grant_out[3] = 0;

    arb->busy_out = 0;
    arb->any_grant = 0;

    arb->priority[0][0] = 0; arb->priority[0][1] = 0;
    arb->priority[1][0] = 1; arb->priority[1][1] = 0;
    arb->priority[2][0] = 0; arb->priority[2][1] = 1;
    arb->priority[3][0] = 1; arb->priority[3][1] = 1;
}

void arbiter_reset(arbiter_t *arb) {
    arbiter_init(arb);
}

void arbiter_set_request(arbiter_t *arb, bit r0, bit r1, bit r2, bit r3) {
    arb->request[0] = r0;
    arb->request[1] = r1;
    arb->request[2] = r2;
    arb->request[3] = r3;
}

void arbiter_set_priority(arbiter_t *arb, bit *p0, bit *p1, bit *p2, bit *p3) {
    arb->priority[0][0] = p0[0]; arb->priority[0][1] = p0[1];
    arb->priority[1][0] = p1[0]; arb->priority[1][1] = p1[1];
    arb->priority[2][0] = p2[0]; arb->priority[2][1] = p2[1];
    arb->priority[3][0] = p3[0]; arb->priority[3][1] = p3[1];
}

void arbiter_clock_rise(arbiter_t *arb) {
    dff_clock_rise(&arb->grant[0]); dff_clock_rise(&arb->grant[1]);
    dff_clock_rise(&arb->grant[2]); dff_clock_rise(&arb->grant[3]);
    dff_clock_rise(&arb->busy);
}

void arbiter_clock_fall(arbiter_t *arb) {
    bit r0 = arb->request[0];
    bit r1 = arb->request[1];
    bit r2 = arb->request[2];
    bit r3 = arb->request[3];

    bit np0_0 = NOT(arb->priority[0][0]);
    bit np0_1 = NOT(arb->priority[0][1]);
    bit np1_0 = NOT(arb->priority[1][0]);
    bit np1_1 = NOT(arb->priority[1][1]);
    bit np2_0 = NOT(arb->priority[2][0]);
    bit np2_1 = NOT(arb->priority[2][1]);
    bit np3_0 = NOT(arb->priority[3][0]);
    bit np3_1 = NOT(arb->priority[3][1]);

    bit is_pri0_0 = AND(np0_0, np0_1);
    bit is_pri0_1 = AND(arb->priority[0][0], np0_1);
    bit is_pri0_2 = AND(np0_0, arb->priority[0][1]);
    bit is_pri0_3 = AND(arb->priority[0][0], arb->priority[0][1]);

    bit is_pri1_0 = AND(np1_0, np1_1);
    bit is_pri1_1 = AND(arb->priority[1][0], np1_1);
    bit is_pri1_2 = AND(np1_0, arb->priority[1][1]);
    bit is_pri1_3 = AND(arb->priority[1][0], arb->priority[1][1]);

    bit is_pri2_0 = AND(np2_0, np2_1);
    bit is_pri2_1 = AND(arb->priority[2][0], np2_1);
    bit is_pri2_2 = AND(np2_0, arb->priority[2][1]);
    bit is_pri2_3 = AND(arb->priority[2][0], arb->priority[2][1]);

    bit is_pri3_0 = AND(np3_0, np3_1);
    bit is_pri3_1 = AND(arb->priority[3][0], np3_1);
    bit is_pri3_2 = AND(np3_0, arb->priority[3][1]);
    bit is_pri3_3 = AND(arb->priority[3][0], arb->priority[3][1]);

    bit req_at_p0 = OR(OR(AND(r0, is_pri0_0), AND(r1, is_pri1_0)),
                       OR(AND(r2, is_pri2_0), AND(r3, is_pri3_0)));
    bit req_at_p1 = OR(OR(AND(r0, is_pri0_1), AND(r1, is_pri1_1)),
                       OR(AND(r2, is_pri2_1), AND(r3, is_pri3_1)));
    bit req_at_p2 = OR(OR(AND(r0, is_pri0_2), AND(r1, is_pri1_2)),
                       OR(AND(r2, is_pri2_2), AND(r3, is_pri3_2)));
    bit req_at_p3 = OR(OR(AND(r0, is_pri0_3), AND(r1, is_pri1_3)),
                       OR(AND(r2, is_pri2_3), AND(r3, is_pri3_3)));

    bit grant_p0 = req_at_p0;
    bit grant_p1 = AND(req_at_p1, NOT(req_at_p0));
    bit grant_p2 = AND(req_at_p2, AND(NOT(req_at_p0), NOT(req_at_p1)));
    bit grant_p3 = AND(req_at_p3, AND(AND(NOT(req_at_p0), NOT(req_at_p1)), NOT(req_at_p2)));

    bit g0_p0 = AND(r0, AND(is_pri0_0, grant_p0));
    bit g0_p1 = AND(r0, AND(is_pri0_1, grant_p1));
    bit g0_p2 = AND(r0, AND(is_pri0_2, grant_p2));
    bit g0_p3 = AND(r0, AND(is_pri0_3, grant_p3));
    bit g0 = OR(OR(g0_p0, g0_p1), OR(g0_p2, g0_p3));

    bit g1_p0 = AND(r1, AND(is_pri1_0, grant_p0));
    bit g1_p1 = AND(r1, AND(is_pri1_1, grant_p1));
    bit g1_p2 = AND(r1, AND(is_pri1_2, grant_p2));
    bit g1_p3 = AND(r1, AND(is_pri1_3, grant_p3));
    bit g1 = OR(OR(g1_p0, g1_p1), OR(g1_p2, g1_p3));

    bit g2_p0 = AND(r2, AND(is_pri2_0, grant_p0));
    bit g2_p1 = AND(r2, AND(is_pri2_1, grant_p1));
    bit g2_p2 = AND(r2, AND(is_pri2_2, grant_p2));
    bit g2_p3 = AND(r2, AND(is_pri2_3, grant_p3));
    bit g2 = OR(OR(g2_p0, g2_p1), OR(g2_p2, g2_p3));

    bit g3_p0 = AND(r3, AND(is_pri3_0, grant_p0));
    bit g3_p1 = AND(r3, AND(is_pri3_1, grant_p1));
    bit g3_p2 = AND(r3, AND(is_pri3_2, grant_p2));
    bit g3_p3 = AND(r3, AND(is_pri3_3, grant_p3));
    bit g3 = OR(OR(g3_p0, g3_p1), OR(g3_p2, g3_p3));

    bit any_req = OR(OR(r0, r1), OR(r2, r3));

    arb->grant[0].d = g0;
    arb->grant[1].d = g1;
    arb->grant[2].d = g2;
    arb->grant[3].d = g3;
    arb->busy.d = any_req;

    dff_clock_fall(&arb->grant[0]); dff_clock_fall(&arb->grant[1]);
    dff_clock_fall(&arb->grant[2]); dff_clock_fall(&arb->grant[3]);
    dff_clock_fall(&arb->busy);
}

void arbiter_propagate(arbiter_t *arb) {
    arb->grant_out[0] = arb->grant[0].q;
    arb->grant_out[1] = arb->grant[1].q;
    arb->grant_out[2] = arb->grant[2].q;
    arb->grant_out[3] = arb->grant[3].q;
    arb->busy_out = arb->busy.q;
    arb->any_grant = OR(OR(arb->grant_out[0], arb->grant_out[1]),
                        OR(arb->grant_out[2], arb->grant_out[3]));
}

bit arbiter_get_grant(arbiter_t *arb, bit s0, bit s1) {
    bit ns0 = NOT(s0);
    bit ns1 = NOT(s1);
    bit sel0 = AND(ns0, ns1);
    bit sel1 = AND(s0, ns1);
    bit sel2 = AND(ns0, s1);
    bit sel3 = AND(s0, s1);

    return OR(OR(AND(arb->grant_out[0], sel0), AND(arb->grant_out[1], sel1)),
              OR(AND(arb->grant_out[2], sel2), AND(arb->grant_out[3], sel3)));
}

bit arbiter_is_busy(arbiter_t *arb) {
    return arb->busy_out;
}
