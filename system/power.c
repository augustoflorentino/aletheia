#include "power.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/mux/mux2.h"

void power_init(power_controller_t *pwr) {
    dff_reset(&pwr->reset_counter[0]);
    dff_reset(&pwr->reset_counter[1]);
    dff_reset(&pwr->reset_counter[2]);
    dff_reset(&pwr->power_good);
    dff_reset(&pwr->reset_out);

    pwr->power_in = 0;
    pwr->external_reset = 0;
    pwr->reset = 1;
    pwr->ready = 0;
    pwr->power_ok = 0;
}

void power_reset(power_controller_t *pwr) {
    power_init(pwr);
}

void power_set_power(power_controller_t *pwr, bit pwr_in) {
    pwr->power_in = pwr_in;
}

void power_set_external_reset(power_controller_t *pwr, bit rst) {
    pwr->external_reset = rst;
}

void power_clock_rise(power_controller_t *pwr) {
    dff_clock_rise(&pwr->reset_counter[0]);
    dff_clock_rise(&pwr->reset_counter[1]);
    dff_clock_rise(&pwr->reset_counter[2]);
    dff_clock_rise(&pwr->power_good);
    dff_clock_rise(&pwr->reset_out);
}

void power_clock_fall(power_controller_t *pwr) {
    bit pwr_ok = pwr->power_in;
    bit ext_rst = pwr->external_reset;

    bit cnt0 = pwr->reset_counter[0].q;
    bit cnt1 = pwr->reset_counter[1].q;
    bit cnt2 = pwr->reset_counter[2].q;

    bit count_done = AND(AND(cnt0, cnt1), cnt2);
    bit counting = AND(pwr_ok, NOT(count_done));

    bit b0 = NOT(cnt0);
    bit b1 = AND(b0, NOT(cnt1));

    bit new_c0 = NOT(cnt0);
    bit new_c1 = mux2(cnt1, NOT(cnt1), b0);
    bit new_c2 = mux2(cnt2, NOT(cnt2), b1);

    bit rst_cnt0 = mux2(cnt0, new_c0, counting);
    bit rst_cnt1 = mux2(cnt1, new_c1, counting);
    bit rst_cnt2 = mux2(cnt2, new_c2, counting);

    bit clear = OR(NOT(pwr_ok), ext_rst);
    pwr->reset_counter[0].d = mux2(rst_cnt0, 0, clear);
    pwr->reset_counter[1].d = mux2(rst_cnt1, 0, clear);
    pwr->reset_counter[2].d = mux2(rst_cnt2, 0, clear);

    pwr->power_good.d = pwr_ok;

    bit reset_active = OR(OR(NOT(pwr_ok), ext_rst), NOT(count_done));
    pwr->reset_out.d = reset_active;

    dff_clock_fall(&pwr->reset_counter[0]);
    dff_clock_fall(&pwr->reset_counter[1]);
    dff_clock_fall(&pwr->reset_counter[2]);
    dff_clock_fall(&pwr->power_good);
    dff_clock_fall(&pwr->reset_out);
}

void power_propagate(power_controller_t *pwr) {
    pwr->reset = pwr->reset_out.q;
    pwr->power_ok = pwr->power_good.q;

    bit cnt0 = pwr->reset_counter[0].q;
    bit cnt1 = pwr->reset_counter[1].q;
    bit cnt2 = pwr->reset_counter[2].q;
    pwr->ready = AND(AND(AND(cnt0, cnt1), cnt2), pwr->power_ok);
}

bit power_get_reset(power_controller_t *pwr) {
    return pwr->reset;
}

bit power_is_ready(power_controller_t *pwr) {
    return pwr->ready;
}
