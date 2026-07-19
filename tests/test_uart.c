#include "test.h"

#include "../io/devices/uart.h"

static int loopback(uint32_t byte, uint32_t *received) {
    uart_tx_t tx;
    uart_rx_t rx;
    uart_tx_init(&tx);
    uart_rx_init(&rx);

    bit data[8];
    to_bits_n(byte, data, 8);
    uart_tx_set_data(&tx, data);
    uart_tx_set_load(&tx, 1);
    uart_tx_clock_fall(&tx);
    uart_tx_clock_rise(&tx);
    uart_tx_set_load(&tx, 0);

    int got = 0;
    int cycle;
    for (cycle = 0; cycle < 20; cycle++) {
        uart_tx_propagate(&tx);
        uart_rx_set_serial(&rx, tx.serial_out);
        uart_rx_propagate(&rx);
        if (rx.valid_out && !got) {
            *received = from_bits_n(rx.data_out, 8);
            got = 1;
        }
        uart_tx_clock_fall(&tx);
        uart_rx_clock_fall(&rx);
        uart_tx_clock_rise(&tx);
        uart_rx_clock_rise(&rx);
    }
    return got;
}

static void test_tx_ready(void) {
    test_suite("uart: TX ready/idle");

    uart_tx_t tx;
    uart_tx_init(&tx);
    uart_tx_propagate(&tx);
    CHECK_EQ_BIT(tx.ready, 1, "TX ready when idle");
    CHECK_EQ_BIT(tx.serial_out, 1, "TX line idle high");

    bit data[8];
    to_bits_n(0x55u, data, 8);
    uart_tx_set_data(&tx, data);
    uart_tx_set_load(&tx, 1);
    uart_tx_clock_fall(&tx);
    uart_tx_clock_rise(&tx);
    uart_tx_propagate(&tx);
    CHECK_EQ_BIT(tx.ready, 0, "TX busy after load");
    CHECK_EQ_BIT(tx.serial_out, 0, "TX emits start bit (0)");
}

static void test_loopback(void) {
    test_suite("uart: TX -> RX loopback");

    uint32_t bytes[] = {0x00u, 0xFFu, 0x55u, 0xAAu, 0x41u, 0x7Fu, 0x80u, 0x01u};
    int n = (int)(sizeof(bytes) / sizeof(bytes[0]));
    int i;
    for (i = 0; i < n; i++) {
        uint32_t rcv = 0xDEAD;
        int ok = loopback(bytes[i], &rcv);
        CHECK(ok, "byte 0x%02x received a valid frame", bytes[i]);
        CHECK_EQ_U32(rcv, bytes[i], "loopback byte matches");
    }
}

static void test_loopback_all(void) {
    test_suite("uart: loopback all 256 bytes");

    uint32_t v;
    for (v = 0; v < 256; v++) {
        uint32_t rcv = 0xDEAD;
        int ok = loopback(v, &rcv);
        CHECK(ok && rcv == v, "byte 0x%02x roundtrip", v);
    }
}

int main(void) {
    test_tx_ready();
    test_loopback();
    test_loopback_all();
    return test_report();
}
