#ifndef ALETHEIA_UART_H
#define ALETHEIA_UART_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t shift[10];
    d_flip_flop_t count[4];
    d_flip_flop_t overrun;

    bit load;
    bit clear_status;
    bit data_in[8];

    bit serial_out;
    bit ready;
    bit overrun_out;
} uart_tx_t;

typedef struct {
    d_flip_flop_t shift[8];
    d_flip_flop_t count[4];
    d_flip_flop_t valid;
    d_flip_flop_t holding[8];
    d_flip_flop_t avail;
    d_flip_flop_t overrun;
    d_flip_flop_t framing;

    bit serial_in;
    bit read_data;
    bit clear_status;

    bit data_out[8];
    bit valid_out;
    bit holding_out[8];
    bit avail_out;
    bit overrun_out;
    bit framing_out;
} uart_rx_t;

void uart_tx_init(uart_tx_t *tx);
void uart_tx_set_data(uart_tx_t *tx, bit *data);
void uart_tx_set_load(uart_tx_t *tx, bit load);
void uart_tx_set_clear_status(uart_tx_t *tx, bit clear);
void uart_tx_clock_fall(uart_tx_t *tx);
void uart_tx_clock_rise(uart_tx_t *tx);
void uart_tx_propagate(uart_tx_t *tx);

void uart_rx_init(uart_rx_t *rx);
void uart_rx_set_serial(uart_rx_t *rx, bit s);
void uart_rx_set_read(uart_rx_t *rx, bit read_data, bit clear_status);
void uart_rx_clock_fall(uart_rx_t *rx);
void uart_rx_clock_rise(uart_rx_t *rx);
void uart_rx_propagate(uart_rx_t *rx);

#endif
