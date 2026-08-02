#include "timer.h"
#include "../../core/gates/and.h"
#include "../../core/gates/or.h"
#include "../../core/gates/not.h"
#include "../../core/mux/mux2.h"

void timer_init(hw_timer_t *tmr) {
    int i;
    i = 0; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 1; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 2; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 3; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 4; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 5; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 6; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 7; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 8; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 9; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 10; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 11; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 12; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 13; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 14; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);
    i = 15; dff_reset(&tmr->counter[i]); dff_reset(&tmr->reload[i]);

    dff_reset(&tmr->control);
    dff_reset(&tmr->control_reload);
    dff_reset(&tmr->irq_pending);

    i = 0; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 1; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 2; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 3; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 4; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 5; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 6; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 7; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 8; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 9; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 10; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 11; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 12; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 13; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 14; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;
    i = 15; tmr->counter_in[i] = 0; tmr->reload_in[i] = 0; tmr->counter_out[i] = 0; tmr->reload_out[i] = 0;

    tmr->control_in = 0;
    tmr->control_reload_in = 0;
    tmr->chip_select = 0;
    tmr->read = 0;
    tmr->write = 0;
    tmr->addr[0] = 0;
    tmr->addr[1] = 0;
    tmr->tick = 1;
    tmr->enabled = 0;
    tmr->auto_reload = 0;
    tmr->irq = 0;
    tmr->zero = 0;
}

void timer_reset(hw_timer_t *tmr) {
    timer_init(tmr);
}

void timer_set_cs(hw_timer_t *tmr, bit cs) {
    tmr->chip_select = cs;
}

void timer_set_read(hw_timer_t *tmr, bit rd) {
    tmr->read = rd;
}

void timer_set_write(hw_timer_t *tmr, bit wr) {
    tmr->write = wr;
}

void timer_set_addr(hw_timer_t *tmr, bit a0, bit a1) {
    tmr->addr[0] = a0;
    tmr->addr[1] = a1;
}

void timer_set_data(hw_timer_t *tmr, bit *data) {
    tmr->counter_in[0] = data[0]; tmr->counter_in[1] = data[1];
    tmr->counter_in[2] = data[2]; tmr->counter_in[3] = data[3];
    tmr->counter_in[4] = data[4]; tmr->counter_in[5] = data[5];
    tmr->counter_in[6] = data[6]; tmr->counter_in[7] = data[7];
    tmr->counter_in[8] = data[8]; tmr->counter_in[9] = data[9];
    tmr->counter_in[10] = data[10]; tmr->counter_in[11] = data[11];
    tmr->counter_in[12] = data[12]; tmr->counter_in[13] = data[13];
    tmr->counter_in[14] = data[14]; tmr->counter_in[15] = data[15];

    tmr->reload_in[0] = data[0]; tmr->reload_in[1] = data[1];
    tmr->reload_in[2] = data[2]; tmr->reload_in[3] = data[3];
    tmr->reload_in[4] = data[4]; tmr->reload_in[5] = data[5];
    tmr->reload_in[6] = data[6]; tmr->reload_in[7] = data[7];
    tmr->reload_in[8] = data[8]; tmr->reload_in[9] = data[9];
    tmr->reload_in[10] = data[10]; tmr->reload_in[11] = data[11];
    tmr->reload_in[12] = data[12]; tmr->reload_in[13] = data[13];
    tmr->reload_in[14] = data[14]; tmr->reload_in[15] = data[15];

    tmr->control_in = data[0];
    tmr->control_reload_in = data[1];
}

void timer_clock_rise(hw_timer_t *tmr) {
    int i;
    i = 0; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 1; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 2; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 3; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 4; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 5; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 6; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 7; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 8; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 9; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 10; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 11; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 12; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 13; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 14; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);
    i = 15; dff_clock_rise(&tmr->counter[i]); dff_clock_rise(&tmr->reload[i]);

    dff_clock_rise(&tmr->control);
    dff_clock_rise(&tmr->control_reload);
    dff_clock_rise(&tmr->irq_pending);
}

void timer_clock_fall(hw_timer_t *tmr) {
    bit active = tmr->chip_select;
    bit wr = AND(tmr->write, active);
    bit na0 = NOT(tmr->addr[0]);
    bit na1 = NOT(tmr->addr[1]);

    bit sel_cnt = AND(na1, na0);
    bit sel_rld = AND(na1, tmr->addr[0]);
    bit sel_ctrl = AND(tmr->addr[1], na0);

    bit wr_cnt = AND(wr, sel_cnt);
    bit wr_rld = AND(wr, sel_rld);
    bit wr_ctrl = AND(wr, sel_ctrl);

    bit en = tmr->control.q;
    bit cnt_zero = AND(AND(AND(AND(NOT(tmr->counter[0].q), NOT(tmr->counter[1].q)),
                               AND(NOT(tmr->counter[2].q), NOT(tmr->counter[3].q))),
                           AND(AND(NOT(tmr->counter[4].q), NOT(tmr->counter[5].q)),
                               AND(NOT(tmr->counter[6].q), NOT(tmr->counter[7].q)))),
                       AND(AND(AND(NOT(tmr->counter[8].q), NOT(tmr->counter[9].q)),
                               AND(NOT(tmr->counter[10].q), NOT(tmr->counter[11].q))),
                           AND(AND(NOT(tmr->counter[12].q), NOT(tmr->counter[13].q)),
                               AND(NOT(tmr->counter[14].q), NOT(tmr->counter[15].q)))));

    bit b0 = NOT(tmr->counter[0].q);
    bit b1 = AND(b0, NOT(tmr->counter[1].q));
    bit b2 = AND(b1, NOT(tmr->counter[2].q));
    bit b3 = AND(b2, NOT(tmr->counter[3].q));
    bit b4 = AND(b3, NOT(tmr->counter[4].q));
    bit b5 = AND(b4, NOT(tmr->counter[5].q));
    bit b6 = AND(b5, NOT(tmr->counter[6].q));
    bit b7 = AND(b6, NOT(tmr->counter[7].q));
    bit b8 = AND(b7, NOT(tmr->counter[8].q));
    bit b9 = AND(b8, NOT(tmr->counter[9].q));
    bit b10 = AND(b9, NOT(tmr->counter[10].q));
    bit b11 = AND(b10, NOT(tmr->counter[11].q));
    bit b12 = AND(b11, NOT(tmr->counter[12].q));
    bit b13 = AND(b12, NOT(tmr->counter[13].q));
    bit b14 = AND(b13, NOT(tmr->counter[14].q));
    (void)b14;

    bit dec_en = AND(AND(en, tmr->tick), NOT(cnt_zero));

    bit new_c0 = NOT(tmr->counter[0].q);
    bit new_c1 = mux2(tmr->counter[1].q, NOT(tmr->counter[1].q), b0);
    bit new_c2 = mux2(tmr->counter[2].q, NOT(tmr->counter[2].q), b1);
    bit new_c3 = mux2(tmr->counter[3].q, NOT(tmr->counter[3].q), b2);
    bit new_c4 = mux2(tmr->counter[4].q, NOT(tmr->counter[4].q), b3);
    bit new_c5 = mux2(tmr->counter[5].q, NOT(tmr->counter[5].q), b4);
    bit new_c6 = mux2(tmr->counter[6].q, NOT(tmr->counter[6].q), b5);
    bit new_c7 = mux2(tmr->counter[7].q, NOT(tmr->counter[7].q), b6);
    bit new_c8 = mux2(tmr->counter[8].q, NOT(tmr->counter[8].q), b7);
    bit new_c9 = mux2(tmr->counter[9].q, NOT(tmr->counter[9].q), b8);
    bit new_c10 = mux2(tmr->counter[10].q, NOT(tmr->counter[10].q), b9);
    bit new_c11 = mux2(tmr->counter[11].q, NOT(tmr->counter[11].q), b10);
    bit new_c12 = mux2(tmr->counter[12].q, NOT(tmr->counter[12].q), b11);
    bit new_c13 = mux2(tmr->counter[13].q, NOT(tmr->counter[13].q), b12);
    bit new_c14 = mux2(tmr->counter[14].q, NOT(tmr->counter[14].q), b13);
    bit new_c15 = mux2(tmr->counter[15].q, NOT(tmr->counter[15].q), b14);

    bit dec_c0 = mux2(tmr->counter[0].q, new_c0, dec_en);
    bit dec_c1 = mux2(tmr->counter[1].q, new_c1, dec_en);
    bit dec_c2 = mux2(tmr->counter[2].q, new_c2, dec_en);
    bit dec_c3 = mux2(tmr->counter[3].q, new_c3, dec_en);
    bit dec_c4 = mux2(tmr->counter[4].q, new_c4, dec_en);
    bit dec_c5 = mux2(tmr->counter[5].q, new_c5, dec_en);
    bit dec_c6 = mux2(tmr->counter[6].q, new_c6, dec_en);
    bit dec_c7 = mux2(tmr->counter[7].q, new_c7, dec_en);
    bit dec_c8 = mux2(tmr->counter[8].q, new_c8, dec_en);
    bit dec_c9 = mux2(tmr->counter[9].q, new_c9, dec_en);
    bit dec_c10 = mux2(tmr->counter[10].q, new_c10, dec_en);
    bit dec_c11 = mux2(tmr->counter[11].q, new_c11, dec_en);
    bit dec_c12 = mux2(tmr->counter[12].q, new_c12, dec_en);
    bit dec_c13 = mux2(tmr->counter[13].q, new_c13, dec_en);
    bit dec_c14 = mux2(tmr->counter[14].q, new_c14, dec_en);
    bit dec_c15 = mux2(tmr->counter[15].q, new_c15, dec_en);

    bit upper_zero = AND(AND(AND(NOT(tmr->counter[1].q), NOT(tmr->counter[2].q)),
                             AND(NOT(tmr->counter[3].q), NOT(tmr->counter[4].q))),
                     AND(AND(AND(NOT(tmr->counter[5].q), NOT(tmr->counter[6].q)),
                             AND(NOT(tmr->counter[7].q), NOT(tmr->counter[8].q))),
                         AND(AND(NOT(tmr->counter[9].q), NOT(tmr->counter[10].q)),
                             AND(AND(NOT(tmr->counter[11].q), NOT(tmr->counter[12].q)),
                                 AND(NOT(tmr->counter[13].q),
                                     AND(NOT(tmr->counter[14].q), NOT(tmr->counter[15].q)))))));
    bit is_one = AND(tmr->counter[0].q, upper_zero);
    bit fire = AND(dec_en, is_one);
    bit reload_now = AND(fire, tmr->control_reload.q);
    tmr->counter[0].d = mux2(mux2(dec_c0, tmr->reload[0].q, reload_now), tmr->counter_in[0], wr_cnt);
    tmr->counter[1].d = mux2(mux2(dec_c1, tmr->reload[1].q, reload_now), tmr->counter_in[1], wr_cnt);
    tmr->counter[2].d = mux2(mux2(dec_c2, tmr->reload[2].q, reload_now), tmr->counter_in[2], wr_cnt);
    tmr->counter[3].d = mux2(mux2(dec_c3, tmr->reload[3].q, reload_now), tmr->counter_in[3], wr_cnt);
    tmr->counter[4].d = mux2(mux2(dec_c4, tmr->reload[4].q, reload_now), tmr->counter_in[4], wr_cnt);
    tmr->counter[5].d = mux2(mux2(dec_c5, tmr->reload[5].q, reload_now), tmr->counter_in[5], wr_cnt);
    tmr->counter[6].d = mux2(mux2(dec_c6, tmr->reload[6].q, reload_now), tmr->counter_in[6], wr_cnt);
    tmr->counter[7].d = mux2(mux2(dec_c7, tmr->reload[7].q, reload_now), tmr->counter_in[7], wr_cnt);
    tmr->counter[8].d = mux2(mux2(dec_c8, tmr->reload[8].q, reload_now), tmr->counter_in[8], wr_cnt);
    tmr->counter[9].d = mux2(mux2(dec_c9, tmr->reload[9].q, reload_now), tmr->counter_in[9], wr_cnt);
    tmr->counter[10].d = mux2(mux2(dec_c10, tmr->reload[10].q, reload_now), tmr->counter_in[10], wr_cnt);
    tmr->counter[11].d = mux2(mux2(dec_c11, tmr->reload[11].q, reload_now), tmr->counter_in[11], wr_cnt);
    tmr->counter[12].d = mux2(mux2(dec_c12, tmr->reload[12].q, reload_now), tmr->counter_in[12], wr_cnt);
    tmr->counter[13].d = mux2(mux2(dec_c13, tmr->reload[13].q, reload_now), tmr->counter_in[13], wr_cnt);
    tmr->counter[14].d = mux2(mux2(dec_c14, tmr->reload[14].q, reload_now), tmr->counter_in[14], wr_cnt);
    tmr->counter[15].d = mux2(mux2(dec_c15, tmr->reload[15].q, reload_now), tmr->counter_in[15], wr_cnt);
    tmr->reload[0].d = mux2(tmr->reload[0].q, tmr->reload_in[0], wr_rld);
    tmr->reload[1].d = mux2(tmr->reload[1].q, tmr->reload_in[1], wr_rld);
    tmr->reload[2].d = mux2(tmr->reload[2].q, tmr->reload_in[2], wr_rld);
    tmr->reload[3].d = mux2(tmr->reload[3].q, tmr->reload_in[3], wr_rld);
    tmr->reload[4].d = mux2(tmr->reload[4].q, tmr->reload_in[4], wr_rld);
    tmr->reload[5].d = mux2(tmr->reload[5].q, tmr->reload_in[5], wr_rld);
    tmr->reload[6].d = mux2(tmr->reload[6].q, tmr->reload_in[6], wr_rld);
    tmr->reload[7].d = mux2(tmr->reload[7].q, tmr->reload_in[7], wr_rld);
    tmr->reload[8].d = mux2(tmr->reload[8].q, tmr->reload_in[8], wr_rld);
    tmr->reload[9].d = mux2(tmr->reload[9].q, tmr->reload_in[9], wr_rld);
    tmr->reload[10].d = mux2(tmr->reload[10].q, tmr->reload_in[10], wr_rld);
    tmr->reload[11].d = mux2(tmr->reload[11].q, tmr->reload_in[11], wr_rld);
    tmr->reload[12].d = mux2(tmr->reload[12].q, tmr->reload_in[12], wr_rld);
    tmr->reload[13].d = mux2(tmr->reload[13].q, tmr->reload_in[13], wr_rld);
    tmr->reload[14].d = mux2(tmr->reload[14].q, tmr->reload_in[14], wr_rld);
    tmr->reload[15].d = mux2(tmr->reload[15].q, tmr->reload_in[15], wr_rld);

    tmr->control.d = mux2(tmr->control.q, tmr->control_in, wr_ctrl);
    tmr->control_reload.d = mux2(tmr->control_reload.q, tmr->control_reload_in, wr_ctrl);

    bit rd_status = AND(AND(tmr->read, active), AND(tmr->addr[1], tmr->addr[0]));
    bit pend = mux2(tmr->irq_pending.q, 0, rd_status);
    tmr->irq_pending.d = OR(pend, fire);

    int i;
    i = 0; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 1; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 2; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 3; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 4; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 5; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 6; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 7; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 8; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 9; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 10; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 11; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 12; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 13; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 14; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);
    i = 15; dff_clock_fall(&tmr->counter[i]); dff_clock_fall(&tmr->reload[i]);

    dff_clock_fall(&tmr->control);
    dff_clock_fall(&tmr->control_reload);
    dff_clock_fall(&tmr->irq_pending);
}

void timer_propagate(hw_timer_t *tmr) {
    tmr->counter_out[0] = tmr->counter[0].q; tmr->counter_out[1] = tmr->counter[1].q;
    tmr->counter_out[2] = tmr->counter[2].q; tmr->counter_out[3] = tmr->counter[3].q;
    tmr->counter_out[4] = tmr->counter[4].q; tmr->counter_out[5] = tmr->counter[5].q;
    tmr->counter_out[6] = tmr->counter[6].q; tmr->counter_out[7] = tmr->counter[7].q;
    tmr->counter_out[8] = tmr->counter[8].q; tmr->counter_out[9] = tmr->counter[9].q;
    tmr->counter_out[10] = tmr->counter[10].q; tmr->counter_out[11] = tmr->counter[11].q;
    tmr->counter_out[12] = tmr->counter[12].q; tmr->counter_out[13] = tmr->counter[13].q;
    tmr->counter_out[14] = tmr->counter[14].q; tmr->counter_out[15] = tmr->counter[15].q;

    tmr->reload_out[0] = tmr->reload[0].q; tmr->reload_out[1] = tmr->reload[1].q;
    tmr->reload_out[2] = tmr->reload[2].q; tmr->reload_out[3] = tmr->reload[3].q;
    tmr->reload_out[4] = tmr->reload[4].q; tmr->reload_out[5] = tmr->reload[5].q;
    tmr->reload_out[6] = tmr->reload[6].q; tmr->reload_out[7] = tmr->reload[7].q;
    tmr->reload_out[8] = tmr->reload[8].q; tmr->reload_out[9] = tmr->reload[9].q;
    tmr->reload_out[10] = tmr->reload[10].q; tmr->reload_out[11] = tmr->reload[11].q;
    tmr->reload_out[12] = tmr->reload[12].q; tmr->reload_out[13] = tmr->reload[13].q;
    tmr->reload_out[14] = tmr->reload[14].q; tmr->reload_out[15] = tmr->reload[15].q;

    tmr->enabled = tmr->control.q;
    tmr->auto_reload = tmr->control_reload.q;
    tmr->irq = tmr->irq_pending.q;

    tmr->zero = AND(AND(AND(AND(NOT(tmr->counter[0].q), NOT(tmr->counter[1].q)),
                            AND(NOT(tmr->counter[2].q), NOT(tmr->counter[3].q))),
                        AND(AND(NOT(tmr->counter[4].q), NOT(tmr->counter[5].q)),
                            AND(NOT(tmr->counter[6].q), NOT(tmr->counter[7].q)))),
                    AND(AND(AND(NOT(tmr->counter[8].q), NOT(tmr->counter[9].q)),
                            AND(NOT(tmr->counter[10].q), NOT(tmr->counter[11].q))),
                        AND(AND(NOT(tmr->counter[12].q), NOT(tmr->counter[13].q)),
                            AND(NOT(tmr->counter[14].q), NOT(tmr->counter[15].q)))));
}

void timer_read_data(hw_timer_t *tmr, bit *out) {
    bit a0 = tmr->addr[0];
    bit a1 = tmr->addr[1];
    bit ctrl[16] = {tmr->control.q, tmr->control_reload.q, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bit status[16] = {tmr->irq_pending.q, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    out[0] = mux2(mux2(tmr->counter_out[0], tmr->reload_out[0], a0), mux2(ctrl[0], status[0], a0), a1);
    out[1] = mux2(mux2(tmr->counter_out[1], tmr->reload_out[1], a0), mux2(ctrl[1], status[1], a0), a1);
    out[2] = mux2(mux2(tmr->counter_out[2], tmr->reload_out[2], a0), mux2(ctrl[2], status[2], a0), a1);
    out[3] = mux2(mux2(tmr->counter_out[3], tmr->reload_out[3], a0), mux2(ctrl[3], status[3], a0), a1);
    out[4] = mux2(mux2(tmr->counter_out[4], tmr->reload_out[4], a0), mux2(ctrl[4], status[4], a0), a1);
    out[5] = mux2(mux2(tmr->counter_out[5], tmr->reload_out[5], a0), mux2(ctrl[5], status[5], a0), a1);
    out[6] = mux2(mux2(tmr->counter_out[6], tmr->reload_out[6], a0), mux2(ctrl[6], status[6], a0), a1);
    out[7] = mux2(mux2(tmr->counter_out[7], tmr->reload_out[7], a0), mux2(ctrl[7], status[7], a0), a1);
    out[8] = mux2(mux2(tmr->counter_out[8], tmr->reload_out[8], a0), mux2(ctrl[8], status[8], a0), a1);
    out[9] = mux2(mux2(tmr->counter_out[9], tmr->reload_out[9], a0), mux2(ctrl[9], status[9], a0), a1);
    out[10] = mux2(mux2(tmr->counter_out[10], tmr->reload_out[10], a0), mux2(ctrl[10], status[10], a0), a1);
    out[11] = mux2(mux2(tmr->counter_out[11], tmr->reload_out[11], a0), mux2(ctrl[11], status[11], a0), a1);
    out[12] = mux2(mux2(tmr->counter_out[12], tmr->reload_out[12], a0), mux2(ctrl[12], status[12], a0), a1);
    out[13] = mux2(mux2(tmr->counter_out[13], tmr->reload_out[13], a0), mux2(ctrl[13], status[13], a0), a1);
    out[14] = mux2(mux2(tmr->counter_out[14], tmr->reload_out[14], a0), mux2(ctrl[14], status[14], a0), a1);
    out[15] = mux2(mux2(tmr->counter_out[15], tmr->reload_out[15], a0), mux2(ctrl[15], status[15], a0), a1);
}

void timer_set_tick(hw_timer_t *tmr, bit tick) {
    tmr->tick = tick;
}

bit timer_get_irq(hw_timer_t *tmr) {
    return tmr->irq;
}

bit timer_get_zero(hw_timer_t *tmr) {
    return tmr->zero;
}
