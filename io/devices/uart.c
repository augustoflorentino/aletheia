#include "uart.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"
#include "../../cpu/alu/adder.h"

static bit count_or(d_flip_flop_t *c) {
    return OR(OR(c[0].q, c[1].q), OR(c[2].q, c[3].q));
}

static bit count_eq_one(d_flip_flop_t *c) {
    return AND(c[0].q, AND(NOT(c[1].q), AND(NOT(c[2].q), NOT(c[3].q))));
}

void uart_tx_init(uart_tx_t *tx) {
    dff_reset(&tx->shift[0]);
    dff_reset(&tx->shift[1]);
    dff_reset(&tx->shift[2]);
    dff_reset(&tx->shift[3]);
    dff_reset(&tx->shift[4]);
    dff_reset(&tx->shift[5]);
    dff_reset(&tx->shift[6]);
    dff_reset(&tx->shift[7]);
    dff_reset(&tx->shift[8]);
    dff_reset(&tx->shift[9]);
    dff_reset(&tx->count[0]);
    dff_reset(&tx->count[1]);
    dff_reset(&tx->count[2]);
    dff_reset(&tx->count[3]);
    tx->load = 0;
    tx->data_in[0] = 0;
    tx->data_in[1] = 0;
    tx->data_in[2] = 0;
    tx->data_in[3] = 0;
    tx->data_in[4] = 0;
    tx->data_in[5] = 0;
    tx->data_in[6] = 0;
    tx->data_in[7] = 0;
    tx->serial_out = 1;
    tx->ready = 1;
}

void uart_tx_set_data(uart_tx_t *tx, bit *data) {
    tx->data_in[0] = data[0];
    tx->data_in[1] = data[1];
    tx->data_in[2] = data[2];
    tx->data_in[3] = data[3];
    tx->data_in[4] = data[4];
    tx->data_in[5] = data[5];
    tx->data_in[6] = data[6];
    tx->data_in[7] = data[7];
}

void uart_tx_set_load(uart_tx_t *tx, bit load) {
    tx->load = load;
}

void uart_tx_clock_fall(uart_tx_t *tx) {
    bit busy = count_or(tx->count);
    bit cnt[4] = {tx->count[0].q, tx->count[1].q, tx->count[2].q, tx->count[3].q};
    bit ones[4] = {1, 1, 1, 1};
    bit dec[4], cout;
    add_4bit(cnt, ones, 0, dec, &cout);
    bit ten[4] = {0, 1, 0, 1};
    bit frame[10];
    frame[0] = 0;
    frame[1] = tx->data_in[0];
    frame[2] = tx->data_in[1];
    frame[3] = tx->data_in[2];
    frame[4] = tx->data_in[3];
    frame[5] = tx->data_in[4];
    frame[6] = tx->data_in[5];
    frame[7] = tx->data_in[6];
    frame[8] = tx->data_in[7];
    frame[9] = 1;
    bit bv0 = mux2(tx->shift[0].q, tx->shift[1].q, busy);
    tx->shift[0].d = mux2(bv0, frame[0], tx->load);
    bit bv1 = mux2(tx->shift[1].q, tx->shift[2].q, busy);
    tx->shift[1].d = mux2(bv1, frame[1], tx->load);
    bit bv2 = mux2(tx->shift[2].q, tx->shift[3].q, busy);
    tx->shift[2].d = mux2(bv2, frame[2], tx->load);
    bit bv3 = mux2(tx->shift[3].q, tx->shift[4].q, busy);
    tx->shift[3].d = mux2(bv3, frame[3], tx->load);
    bit bv4 = mux2(tx->shift[4].q, tx->shift[5].q, busy);
    tx->shift[4].d = mux2(bv4, frame[4], tx->load);
    bit bv5 = mux2(tx->shift[5].q, tx->shift[6].q, busy);
    tx->shift[5].d = mux2(bv5, frame[5], tx->load);
    bit bv6 = mux2(tx->shift[6].q, tx->shift[7].q, busy);
    tx->shift[6].d = mux2(bv6, frame[6], tx->load);
    bit bv7 = mux2(tx->shift[7].q, tx->shift[8].q, busy);
    tx->shift[7].d = mux2(bv7, frame[7], tx->load);
    bit bv8 = mux2(tx->shift[8].q, tx->shift[9].q, busy);
    tx->shift[8].d = mux2(bv8, frame[8], tx->load);
    bit bv9 = mux2(tx->shift[9].q, 1, busy);
    tx->shift[9].d = mux2(bv9, frame[9], tx->load);
    tx->count[0].d = mux2(mux2(0, dec[0], busy), ten[0], tx->load);
    tx->count[1].d = mux2(mux2(0, dec[1], busy), ten[1], tx->load);
    tx->count[2].d = mux2(mux2(0, dec[2], busy), ten[2], tx->load);
    tx->count[3].d = mux2(mux2(0, dec[3], busy), ten[3], tx->load);
    dff_clock_fall(&tx->shift[0]);
    dff_clock_fall(&tx->shift[1]);
    dff_clock_fall(&tx->shift[2]);
    dff_clock_fall(&tx->shift[3]);
    dff_clock_fall(&tx->shift[4]);
    dff_clock_fall(&tx->shift[5]);
    dff_clock_fall(&tx->shift[6]);
    dff_clock_fall(&tx->shift[7]);
    dff_clock_fall(&tx->shift[8]);
    dff_clock_fall(&tx->shift[9]);
    dff_clock_fall(&tx->count[0]);
    dff_clock_fall(&tx->count[1]);
    dff_clock_fall(&tx->count[2]);
    dff_clock_fall(&tx->count[3]);
}

void uart_tx_clock_rise(uart_tx_t *tx) {
    dff_clock_rise(&tx->shift[0]);
    dff_clock_rise(&tx->shift[1]);
    dff_clock_rise(&tx->shift[2]);
    dff_clock_rise(&tx->shift[3]);
    dff_clock_rise(&tx->shift[4]);
    dff_clock_rise(&tx->shift[5]);
    dff_clock_rise(&tx->shift[6]);
    dff_clock_rise(&tx->shift[7]);
    dff_clock_rise(&tx->shift[8]);
    dff_clock_rise(&tx->shift[9]);
    dff_clock_rise(&tx->count[0]);
    dff_clock_rise(&tx->count[1]);
    dff_clock_rise(&tx->count[2]);
    dff_clock_rise(&tx->count[3]);
}

void uart_tx_propagate(uart_tx_t *tx) {
    bit busy = count_or(tx->count);
    tx->serial_out = mux2(1, tx->shift[0].q, busy);
    tx->ready = NOT(busy);
}

void uart_rx_init(uart_rx_t *rx) {
    dff_reset(&rx->shift[0]);
    dff_reset(&rx->shift[1]);
    dff_reset(&rx->shift[2]);
    dff_reset(&rx->shift[3]);
    dff_reset(&rx->shift[4]);
    dff_reset(&rx->shift[5]);
    dff_reset(&rx->shift[6]);
    dff_reset(&rx->shift[7]);
    dff_reset(&rx->count[0]);
    dff_reset(&rx->count[1]);
    dff_reset(&rx->count[2]);
    dff_reset(&rx->count[3]);
    dff_reset(&rx->valid);
    rx->serial_in = 1;
    rx->data_out[0] = 0;
    rx->data_out[1] = 0;
    rx->data_out[2] = 0;
    rx->data_out[3] = 0;
    rx->data_out[4] = 0;
    rx->data_out[5] = 0;
    rx->data_out[6] = 0;
    rx->data_out[7] = 0;
    rx->valid_out = 0;
}

void uart_rx_set_serial(uart_rx_t *rx, bit s) {
    rx->serial_in = s;
}

void uart_rx_clock_fall(uart_rx_t *rx) {
    bit busy = count_or(rx->count);
    bit idle = NOT(busy);
    bit start_detected = AND(idle, NOT(rx->serial_in));
    bit eq1 = count_eq_one(rx->count);
    bit shift_enable = AND(busy, NOT(eq1));
    bit cnt[4] = {rx->count[0].q, rx->count[1].q, rx->count[2].q, rx->count[3].q};
    bit ones[4] = {1, 1, 1, 1};
    bit dec[4], cout;
    add_4bit(cnt, ones, 0, dec, &cout);
    bit nine[4] = {1, 0, 0, 1};
    rx->count[0].d = mux2(mux2(0, dec[0], busy), nine[0], start_detected);
    rx->count[1].d = mux2(mux2(0, dec[1], busy), nine[1], start_detected);
    rx->count[2].d = mux2(mux2(0, dec[2], busy), nine[2], start_detected);
    rx->count[3].d = mux2(mux2(0, dec[3], busy), nine[3], start_detected);
    rx->shift[0].d = mux2(rx->shift[0].q, rx->shift[1].q, shift_enable);
    rx->shift[1].d = mux2(rx->shift[1].q, rx->shift[2].q, shift_enable);
    rx->shift[2].d = mux2(rx->shift[2].q, rx->shift[3].q, shift_enable);
    rx->shift[3].d = mux2(rx->shift[3].q, rx->shift[4].q, shift_enable);
    rx->shift[4].d = mux2(rx->shift[4].q, rx->shift[5].q, shift_enable);
    rx->shift[5].d = mux2(rx->shift[5].q, rx->shift[6].q, shift_enable);
    rx->shift[6].d = mux2(rx->shift[6].q, rx->shift[7].q, shift_enable);
    rx->shift[7].d = mux2(rx->shift[7].q, rx->serial_in, shift_enable);
    rx->valid.d = AND(busy, eq1);
    dff_clock_fall(&rx->shift[0]);
    dff_clock_fall(&rx->shift[1]);
    dff_clock_fall(&rx->shift[2]);
    dff_clock_fall(&rx->shift[3]);
    dff_clock_fall(&rx->shift[4]);
    dff_clock_fall(&rx->shift[5]);
    dff_clock_fall(&rx->shift[6]);
    dff_clock_fall(&rx->shift[7]);
    dff_clock_fall(&rx->count[0]);
    dff_clock_fall(&rx->count[1]);
    dff_clock_fall(&rx->count[2]);
    dff_clock_fall(&rx->count[3]);
    dff_clock_fall(&rx->valid);
}

void uart_rx_clock_rise(uart_rx_t *rx) {
    dff_clock_rise(&rx->shift[0]);
    dff_clock_rise(&rx->shift[1]);
    dff_clock_rise(&rx->shift[2]);
    dff_clock_rise(&rx->shift[3]);
    dff_clock_rise(&rx->shift[4]);
    dff_clock_rise(&rx->shift[5]);
    dff_clock_rise(&rx->shift[6]);
    dff_clock_rise(&rx->shift[7]);
    dff_clock_rise(&rx->count[0]);
    dff_clock_rise(&rx->count[1]);
    dff_clock_rise(&rx->count[2]);
    dff_clock_rise(&rx->count[3]);
    dff_clock_rise(&rx->valid);
}

void uart_rx_propagate(uart_rx_t *rx) {
    rx->data_out[0] = rx->shift[0].q;
    rx->data_out[1] = rx->shift[1].q;
    rx->data_out[2] = rx->shift[2].q;
    rx->data_out[3] = rx->shift[3].q;
    rx->data_out[4] = rx->shift[4].q;
    rx->data_out[5] = rx->shift[5].q;
    rx->data_out[6] = rx->shift[6].q;
    rx->data_out[7] = rx->shift[7].q;
    rx->valid_out = rx->valid.q;
}
