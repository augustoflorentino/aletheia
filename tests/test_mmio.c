#include "test.h"
#include "../simulation/simulator.h"

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
    for (i = 0; i < 16; i++) rom_load_at(&b->rom, (unsigned)(i), prog[i]);
}

static uint32_t disp_read(board_t *b, uint32_t pos) {
    bit a[3];
    to_bits_n(pos, a, 3);
    display_set_addr(&b->disp, a);
    display_propagate(&b->disp);
    return from_bits_n(b->disp.data_out, 8);
}

static uint32_t ram_peek(board_t *b, uint32_t addr) {
    bit a[32], out[32];
    to_bits_32(addr, a);
    ram_set_addr(&b->ram, a);
    ram_set_write(&b->ram, 0);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_read(&b->ram, out);
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
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF00),
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
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF10),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, (uint32_t)'B'),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0xFF11),
    };
    load(&b, prog, 4);

    int c;
    for (c = 0; c < 6; c++) board_step(&b);

    CHECK_EQ_U32(disp_read(&b, 0), (uint32_t)'A', "display[0] = 'A'");
    CHECK_EQ_U32(disp_read(&b, 1), (uint32_t)'B', "display[1] = 'B'");
    CHECK_EQ_U32(disp_read(&b, 2), 0u, "display[2] untouched");
}

static void test_memory_still_works(void) {
    test_suite("mmio: addresses below the I/O page go to the SRAM");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 99),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
    };
    load(&b, prog, 2);

    int c;
    for (c = 0; c < 4; c++) board_step(&b);

    CHECK_EQ_U32(ram_peek(&b, 3), 99u, "SW at 3 wrote to the SRAM");
    CHECK_EQ_U32(disp_read(&b, 3), 0u, "display was not touched");
}

static void test_io_read_status(void) {
    test_suite("mmio: LW in the I/O space reads UART status");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_LW, REG_ZERO, REG_T0, 0xFF00),
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
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF21),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 0x55),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0xFF20),
    };
    load(&b, prog, 4);

    int c;
    for (c = 0; c < 6; c++) board_step(&b);

    gpio_propagate(&b.gpio);
    CHECK_EQ_U32(from_bits_n(b.gpio.pin_out, 8), 0x55u, "GPIO pins reflect 0x55");
}

static void load_run(board_t *b, const uint32_t *words, int n) {
    board_init(b);
    board_pipe_init(b);
    bit w[32];
    for (int i = 0; i < n; i++) {
        to_bits_32(words[i], w);
        rom_load_at(&b->rom, (unsigned)(i), w);
    }
}

static uint32_t reg(board_t *b, int r) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[r].data_out);
}

static void (*step)(board_t *) = board_step;

static uint32_t run_to_halt(board_t *b, int max) {
    int c = 0;
    max *= 6;
    while (c < max && !b->halt) {
        step(b);
        c++;
    }
    return (uint32_t)c;
}

#define HALT ((uint32_t)OP_HALT << 26)

static void test_io_readback(void) {
    test_suite("mmio: every readable register reads back what was written");

    static board_t b;
    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 0x0F),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF21),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 0x5A),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF20),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 0xA5),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF34),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 0x1234),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF31),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 2),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF32),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 'Z'),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF10),
        encode_i_type(OP_LW, REG_ZERO, REG_S0, 0xFF20),
        encode_i_type(OP_LW, REG_ZERO, REG_S1, 0xFF21),
        encode_i_type(OP_LW, REG_ZERO, REG_S2, 0xFF22),
        encode_i_type(OP_LW, REG_ZERO, REG_S3, 0xFF34),
        encode_i_type(OP_LW, REG_ZERO, REG_S4, 0xFF31),
        encode_i_type(OP_LW, REG_ZERO, REG_S5, 0xFF32),
        encode_i_type(OP_LW, REG_ZERO, REG_S6, 0xFF10),
        encode_i_type(OP_LW, REG_ZERO, REG_S7, 0xFF37),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 0xFF36),
        HALT,
    };
    load_run(&b, prog, (int)(sizeof prog / sizeof prog[0]));
    run_to_halt(&b, 100);
    CHECK_EQ_U32(reg(&b, REG_S0), 0x5Au, "GPIO output register");
    CHECK_EQ_U32(reg(&b, REG_S1), 0x0Fu, "GPIO direction register");
    CHECK_EQ_U32(reg(&b, REG_S2), 0x0Au, "GPIO pins (outputs driven, inputs low)");
    CHECK_EQ_U32(reg(&b, REG_S3), 0xA5u, "PIC mask");
    CHECK_EQ_U32(reg(&b, REG_S4), 0x1234u, "timer reload");
    CHECK_EQ_U32(reg(&b, REG_S5), 2u, "timer control");
    CHECK_EQ_U32(reg(&b, REG_S6), 0u, "display is write-only");
    CHECK_EQ_U32(reg(&b, REG_S7), 0u, "unmapped system register reads 0");
    CHECK_EQ_U32(reg(&b, REG_T1), 0u, "MMU enable");
}

static void test_io_decode_is_complete(void) {
    test_suite("mmio: addresses outside a device window do not alias it");

    static board_t b;
    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 7),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x300),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF3C),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF08),
        encode_i_type(OP_LW, REG_ZERO, REG_S0, 0xFF3D),
        HALT,
    };
    load_run(&b, prog, (int)(sizeof prog / sizeof prog[0]));
    run_to_halt(&b, 50);
    uart_tx_propagate(&b.uart);
    CHECK_EQ_BIT(b.uart.ready, 1, "0x300 and 0xFFFFFF08 do not start a UART transfer");
    bit cnt[16];
    timer_set_addr(&b.timer, 0, 0);
    timer_propagate(&b.timer);
    timer_read_data(&b.timer, cnt);
    CHECK_EQ_U32(from_bits_n(cnt, 16), 0u, "0xFFFFFF3C does not write the timer counter");
    CHECK_EQ_U32(reg(&b, REG_S0), 0u, "0xFFFFFF3D reads 0");
    CHECK_EQ_U32(ram_peek(&b, 0x300 & (RAM_WORDS - 1)), 7u, "0x300 is RAM (outside I/O space)");
}

static void test_uart_receive(void) {
    test_suite("mmio: CPU polls and reads a byte from the UART receiver");

    static board_t b;
    uint32_t prog[] = {
        encode_i_type(OP_LW, REG_ZERO, REG_T0, 0xFF00),
        encode_i_type(OP_ANDI, REG_T0, REG_T0, 2),
        encode_i_type(OP_BEQ, REG_T0, REG_ZERO, 0xFFFD),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 0xFF01),
        encode_i_type(OP_LW, REG_ZERO, REG_T2, 0xFF00),
        HALT,
    };
    load_run(&b, prog, (int)(sizeof prog / sizeof prog[0]));
    uart_tx_t host;
    uart_tx_init(&host);
    bit k[8];
    to_bits_n('K', k, 8);
    uart_tx_set_data(&host, k);
    uart_tx_set_load(&host, 1);
    for (int c = 0; c < 200 && !b.halt; c++) {
        uart_tx_propagate(&host);
        b.serial_in = host.serial_out;
        board_step(&b);
        uart_tx_clock_fall(&host);
        uart_tx_clock_rise(&host);
        uart_tx_set_load(&host, 0);
    }
    CHECK_EQ_BIT(b.halt, 1, "program saw the byte and halted");
    CHECK_EQ_U32(reg(&b, REG_T1), (uint32_t)'K', "received byte");
    CHECK_EQ_U32(reg(&b, REG_T2) & 2u, 0u, "reading data clears 'available'");
}

static void test_uart_tx_overrun(void) {
    test_suite("mmio: writing to a busy UART is refused and flagged");

    static board_t b;
    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 'A'),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF00),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 'B'),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF00),
        encode_i_type(OP_LW, REG_ZERO, REG_S0, 0xFF00),
        encode_i_type(OP_LW, REG_ZERO, REG_S1, 0xFF00),
        HALT,
    };
    load_run(&b, prog, (int)(sizeof prog / sizeof prog[0]));
    uart_rx_t rx;
    uart_rx_init(&rx);
    uint32_t got = 0xFFFFu;
    for (int c = 0; c < 30; c++) {
        board_step(&b);
        uart_rx_set_serial(&rx, b.uart.serial_out);
        uart_rx_propagate(&rx);
        if (rx.valid_out && got == 0xFFFFu) got = from_bits_n(rx.data_out, 8);
        uart_rx_clock_fall(&rx);
        uart_rx_clock_rise(&rx);
    }
    CHECK_EQ_U32(got, (uint32_t)'A', "byte in flight is not corrupted");
    CHECK_EQ_U32(reg(&b, REG_S0) & 4u, 4u, "overrun flagged");
    CHECK_EQ_U32(reg(&b, REG_S1) & 4u, 0u, "reading status clears overrun");
}

static void test_timer_auto_reload(void) {
    test_suite("mmio: timer with auto-reload fires periodically");

    static board_t b;
    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF31),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF30),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 3),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF32),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 0xFF33),
        encode_r_type(REG_T2, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_j_type(OP_J, 5),
    };
    load_run(&b, prog, (int)(sizeof prog / sizeof prog[0]));
    for (int c = 0; c < 65; c++) board_step(&b);
    uint32_t fired = reg(&b, REG_T2);
    CHECK(fired >= 3, "timer fired %u times in 60 cycles (expected periodic)", fired);
}

static void run_multi(void) {
    test_suite("mmio: readback and decode on the multi-cycle core");
    step = board_step_mc;
    test_io_readback();
    test_io_decode_is_complete();
    test_suite("mmio: readback and decode on the pipeline");
    step = board_step_pipe;
    test_io_readback();
    test_io_decode_is_complete();
    step = board_step;
}

int main(void) {
    test_cpu_writes_uart();
    test_cpu_writes_display();
    test_memory_still_works();
    test_io_read_status();
    test_cpu_drives_gpio();
    test_io_readback();
    test_io_decode_is_complete();
    test_uart_receive();
    test_uart_tx_overrun();
    test_timer_auto_reload();
    run_multi();
    return test_report();
}
