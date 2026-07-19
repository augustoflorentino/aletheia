#ifndef ALETHEIA_UART_H
#define ALETHEIA_UART_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t shift[10];
    d_flip_flop_t count[4];

    bit load;
    bit data_in[8];

    bit serial_out;
    bit ready;
} uart_tx_t;

typedef struct {
    d_flip_flop_t shift[8];
    d_flip_flop_t count[4];
    d_flip_flop_t valid;

    bit serial_in;

    bit data_out[8];
    bit valid_out;
} uart_rx_t;

void uart_tx_init(uart_tx_t *tx);
void uart_tx_set_data(uart_tx_t *tx, bit *data);
void uart_tx_set_load(uart_tx_t *tx, bit load);
void uart_tx_clock_fall(uart_tx_t *tx);
void uart_tx_clock_rise(uart_tx_t *tx);
void uart_tx_propagate(uart_tx_t *tx);

void uart_rx_init(uart_rx_t *rx);
void uart_rx_set_serial(uart_rx_t *rx, bit s);
void uart_rx_clock_fall(uart_rx_t *rx);
void uart_rx_clock_rise(uart_rx_t *rx);
void uart_rx_propagate(uart_rx_t *rx);

#endif
