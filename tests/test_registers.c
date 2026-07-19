#include "test.h"

#include "../core/sequential/d_flip_flop.h"
#include "../cpu/registers/register.h"
#include "../cpu/registers/register_file.h"
#include "../cpu/registers/flags.h"

static void dff_load(d_flip_flop_t *ff, bit d) {
    dff_set_d(ff, d);
    dff_clock_fall(ff);
    dff_clock_rise(ff);
}

static void test_dff(void) {
    test_suite("registers: d flip-flop");

    d_flip_flop_t ff;
    dff_reset(&ff);
    CHECK_EQ_BIT(ff.q, 0, "dff reset q");
    CHECK_EQ_BIT(ff.q_bar, 1, "dff reset q_bar");

    dff_load(&ff, 1);
    CHECK_EQ_BIT(ff.q, 1, "dff load 1");
    CHECK_EQ_BIT(ff.q_bar, 0, "dff q_bar after load 1");

    dff_load(&ff, 0);
    CHECK_EQ_BIT(ff.q, 0, "dff load 0");

    dff_set_d(&ff, 1);
    CHECK_EQ_BIT(ff.q, 0, "dff holds before edge");
    dff_clock_fall(&ff);
    dff_clock_rise(&ff);
    CHECK_EQ_BIT(ff.q, 1, "dff captures on edge");
}

static void reg32_write(register_32_t *reg, uint32_t v) {
    bit d[32];
    to_bits_32(v, d);
    reg32_set_input(reg, d);
    reg32_set_load(reg, 1);
    reg32_clock_fall(reg);
    reg32_clock_rise(reg);
    reg32_propagate(reg);
}

static void test_register32(void) {
    test_suite("registers: register_32 load / hold");

    register_32_t reg;
    reg32_init(&reg);
    reg32_reset(&reg);
    reg32_propagate(&reg);
    CHECK_EQ_U32(from_bits_32(reg.data_out), 0u, "reg32 reset");

    reg32_write(&reg, 0xDEADBEEFu);
    CHECK_EQ_U32(from_bits_32(reg.data_out), 0xDEADBEEFu, "reg32 write");

    reg32_set_load(&reg, 0);
    bit d[32];
    to_bits_32(0x12345678u, d);
    reg32_set_input(&reg, d);
    reg32_clock_fall(&reg);
    reg32_clock_rise(&reg);
    reg32_propagate(&reg);
    CHECK_EQ_U32(from_bits_32(reg.data_out), 0xDEADBEEFu, "reg32 holds when load=0");
}

static void regfile_write(register_file_t *rf, int idx, uint32_t v) {
    bit addr[5], data[32];
    to_bits_n((uint32_t)idx, addr, 5);
    to_bits_32(v, data);
    regfile_set_write_addr(rf, addr);
    regfile_set_write_data(rf, data);
    regfile_set_write_enable(rf, 1);
    regfile_clock_fall(rf);
    regfile_clock_rise(rf);
    regfile_propagate(rf);
}

static uint32_t regfile_read1(register_file_t *rf, int idx) {
    bit addr[5];
    to_bits_n((uint32_t)idx, addr, 5);
    regfile_set_read_addr1(rf, addr);
    regfile_propagate(rf);
    return from_bits_32(rf->read_data1);
}

static void test_register_file(void) {
    test_suite("registers: register_file (32 x 32)");

    register_file_t rf;
    regfile_init(&rf);
    regfile_reset(&rf);

    int i;
    for (i = 1; i < REG_COUNT; i++) {
        regfile_write(&rf, i, 0x1000u + (uint32_t)i);
    }
    for (i = 1; i < REG_COUNT; i++) {
        CHECK_EQ_U32(regfile_read1(&rf, i), 0x1000u + (uint32_t)i, "regfile readback");
    }

    regfile_write(&rf, 0, 0xDEADBEEFu);
    CHECK_EQ_U32(regfile_read1(&rf, 0), 0u, "regfile $zero hardwired to 0");

    regfile_set_write_enable(&rf, 0);
    bit addr[5], data[32];
    to_bits_n(3u, addr, 5);
    to_bits_32(0xFFFFFFFFu, data);
    regfile_set_write_addr(&rf, addr);
    regfile_set_write_data(&rf, data);
    regfile_clock_fall(&rf);
    regfile_clock_rise(&rf);
    regfile_propagate(&rf);
    CHECK_EQ_U32(regfile_read1(&rf, 3), 0x1003u, "regfile no write when disabled");
}

static void test_flags(void) {
    test_suite("registers: flags register");

    flags_register_t f;
    flags_init(&f);
    flags_reset(&f);

    flags_set_input(&f, 1, 0, 1, 0);
    flags_set_load(&f, 1);
    flags_clock_fall(&f);
    flags_clock_rise(&f);
    flags_propagate(&f);
    CHECK_EQ_BIT(flags_get_zero(&f), 1, "flag Z");
    CHECK_EQ_BIT(flags_get_negative(&f), 0, "flag N");
    CHECK_EQ_BIT(flags_get_carry(&f), 1, "flag C");
    CHECK_EQ_BIT(flags_get_overflow(&f), 0, "flag V");

    flags_set_load(&f, 0);
    flags_set_input(&f, 0, 1, 0, 1);
    flags_clock_fall(&f);
    flags_clock_rise(&f);
    flags_propagate(&f);
    CHECK_EQ_BIT(flags_get_zero(&f), 1, "flag Z held");
    CHECK_EQ_BIT(flags_get_carry(&f), 1, "flag C held");
}

int main(void) {
    test_dff();
    test_register32();
    test_register_file();
    test_flags();
    return test_report();
}
