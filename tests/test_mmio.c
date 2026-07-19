#include "test.h"

#include "../system/board.h"
#include "../io/devices/uart.h"
#include "../isa/isa.h"

static void load(board_t *b, uint32_t *words, int n) {
    bit prog[16][32];
    int i, j;
    for (i = 0; i < 16; i++) {
        uint32_t w = (i < n) ? words[i] : 0u;
        for (j = 0; j < 32; j++) prog[i][j] = (w >> j) & 1u;
    }
    for (i = 0; i < 16; i++) rom256_load_word(&b->rom, i, prog[i]);
}

static uint32_t disp_read(board_t *b, uint32_t pos) {
    bit a[3];
    to_bits_n(pos, a, 3);
    display_set_addr(&b->disp, a);
    display_propagate(&b->disp);
    return from_bits_n(b->disp.data_out, 8);
}

static uint32_t ram_read(board_t *b, uint32_t addr) {
    bit a[8], out[32];
    to_bits_n(addr, a, 8);
    sram256_set_addr(&b->ram, a);
    sram256_set_write(&b->ram, 0);
    sram256_set_cs(&b->ram, 1);
    sram256_propagate(&b->ram);
    sram256_read(&b->ram, out);
    return from_bits_32(out);
}

static void test_cpu_writes_uart(void) {
    test_suite("mmio: program on the CPU transmits a byte through the UART");

    board_t b;
    board_init(&b);
    uart_rx_t rx;
    uart_rx_init(&rx);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, (uint32_t)'H'),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 256),
    };
    load(&b, prog, 2);

    uint32_t got = 0xFFFFu;
    int c;
    for (c = 0; c < 20; c++) {
        board_step(&b);
        uart_rx_set_serial(&rx, b.uart.serial_out);
        uart_rx_propagate(&rx);
        if (rx.valid_out && got == 0xFFFFu) got = from_bits_n(rx.data_out, 8);
        uart_rx_clock_fall(&rx);
        uart_rx_clock_rise(&rx);
    }

    CHECK_EQ_U32(got, (uint32_t)'H', "byte 'H' came out serialized through the UART");
}

static void test_cpu_writes_display(void) {
    test_suite("mmio: program on the CPU writes to the display");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, (uint32_t)'A'),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 272),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, (uint32_t)'B'),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 273),
    };
    load(&b, prog, 4);

    int c;
    for (c = 0; c < 6; c++) board_step(&b);

    CHECK_EQ_U32(disp_read(&b, 0), (uint32_t)'A', "display[0] = 'A'");
    CHECK_EQ_U32(disp_read(&b, 1), (uint32_t)'B', "display[1] = 'B'");
    CHECK_EQ_U32(disp_read(&b, 2), 0u, "display[2] untouched");
}

static void test_memory_still_works(void) {
    test_suite("mmio: addresses < 0x100 still go to the SRAM");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 99),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
    };
    load(&b, prog, 2);

    int c;
    for (c = 0; c < 4; c++) board_step(&b);

    CHECK_EQ_U32(ram_read(&b, 3), 99u, "SW at 3 wrote to the SRAM");
    CHECK_EQ_U32(disp_read(&b, 3), 0u, "display was not touched");
}

static void test_io_read_status(void) {
    test_suite("mmio: LW in the I/O space reads UART status");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_LW, REG_ZERO, REG_T0, 256),
    };
    load(&b, prog, 1);

    board_step(&b);
    regfile_propagate(&b.regs);
    uint32_t status = from_bits_32(b.regs.regs[REG_T0].data_out);
    CHECK_EQ_U32(status & 1u, 1u, "idle UART reports ready");
}

static void test_cpu_drives_gpio(void) {
    test_suite("mmio: program on the CPU drives GPIO pins");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 0xFF),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x124),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 0x55),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x120),
    };
    load(&b, prog, 4);

    int c;
    for (c = 0; c < 6; c++) board_step(&b);

    gpio_propagate(&b.gpio);
    CHECK_EQ_U32(from_bits_n(b.gpio.pin_out, 8), 0x55u, "GPIO pins reflect 0x55");
}

int main(void) {
    test_cpu_writes_uart();
    test_cpu_writes_display();
    test_memory_still_works();
    test_io_read_status();
    test_cpu_drives_gpio();
    return test_report();
}
