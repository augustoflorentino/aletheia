#include "test.h"

#include "../io/format/number_formatter.h"
#include "../io/devices/uart.h"

static uint32_t uart_send_byte(uart_tx_t *tx, uart_rx_t *rx, uint32_t byte) {
    bit data[8];
    to_bits_n(byte, data, 8);
    uart_tx_set_data(tx, data);
    uart_tx_set_load(tx, 1);
    uart_tx_clock_fall(tx);
    uart_tx_clock_rise(tx);
    uart_tx_set_load(tx, 0);

    uint32_t got = 0xFFFF;
    int cycle;
    for (cycle = 0; cycle < 20; cycle++) {
        uart_tx_propagate(tx);
        uart_rx_set_serial(rx, tx->serial_out);
        uart_rx_propagate(rx);
        if (rx->valid_out && got == 0xFFFF) got = from_bits_n(rx->data_out, 8);
        uart_tx_clock_fall(tx);
        uart_rx_clock_fall(rx);
        uart_tx_clock_rise(tx);
        uart_rx_clock_rise(rx);
    }
    return got;
}

static void print_number(uint32_t v, char *out) {
    bit in[32], ascii[NF_DIGITS][8];
    to_bits_32(v, in);
    number_format(in, ascii);

    uart_tx_t tx;
    uart_rx_t rx;

    int len = 0;
    int i;
    for (i = NF_DIGITS - 1; i >= 0; i--) {
        uint32_t ch = from_bits_n(ascii[i], 8);
        if (ch == (uint32_t)' ') continue;
        uart_tx_init(&tx);
        uart_rx_init(&rx);
        uint32_t rcv = uart_send_byte(&tx, &rx, ch);
        out[len++] = (char)rcv;
    }
    out[len] = '\0';
}

static void ref_decimal(uint32_t v, char *out) {
    char tmp[16];
    int n = 0;
    do { tmp[n++] = (char)('0' + (v % 10u)); v /= 10u; } while (v != 0);
    int i;
    for (i = 0; i < n; i++) out[i] = tmp[n - 1 - i];
    out[n] = '\0';
}

static int streq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

static void check(uint32_t v) {
    char got[16], exp[16];
    print_number(v, got);
    ref_decimal(v, exp);
    char label[48];
    snprintf(label, sizeof(label), "print %u -> \"%s\"", v, exp);
    CHECK(streq(got, exp), "%s (got \"%s\")", label, got);
}

static void test_print_path(void) {
    test_suite("print path: number -> formatter -> UART -> RX");

    uint32_t vals[] = {0u, 7u, 42u, 100u, 243u, 65535u, 1000000u, 4294967295u};
    int i;
    for (i = 0; i < (int)(sizeof(vals) / sizeof(vals[0])); i++) check(vals[i]);

    rng_seed(0x9E3Du);
    int k;
    for (k = 0; k < 300; k++) check(rng_next());
}

int main(void) {
    test_print_path();
    return test_report();
}
