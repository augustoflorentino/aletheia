#include "test.h"

#include "../synth/netlist.h"
#include "../simulation/scheduler.h"
#include "../cpu/alu/adder.h"
#include "../cpu/alu/alu.h"
#include "../cpu/registers/register_file.h"
#include "../cpu/control/decoder.h"

static void inputs(bit *nodes, int n) {
    for (int i = 0; i < n; i++) nodes[i] = nl_input();
}

static int adder32_depth(void) {
    nl_reset();
    bit a[32], b[32], r[32], c[33];
    inputs(a, 32);
    inputs(b, 32);
    adder_32(a, b, r, c);
    return nl_depth_of(r, 32);
}

static int alu_depth(void) {
    nl_reset();
    bit a[32], b[32], op[4], r[32];
    alu_flags_t flags;
    inputs(a, 32);
    inputs(b, 32);
    inputs(op, 4);
    alu_execute_ext(a, b, op, r, &flags);
    return nl_depth_of(r, 32);
}

static int regfile_read_depth(void) {
    static register_file_t rf;
    nl_reset();
    regfile_init(&rf);
    for (int r = 0; r < 32; r++) {
        for (int i = 0; i < 32; i++) rf.regs[r].bits[i].q = nl_input();
    }
    bit addr[5];
    inputs(addr, 5);
    regfile_set_read_addr1(&rf, addr);
    regfile_set_read_addr2(&rf, addr);
    regfile_propagate(&rf);
    return nl_depth_of(rf.read_data1, 32);
}

static int decoder_depth(void) {
    static instruction_decoder_t dec;
    nl_reset();
    decoder_init(&dec);
    bit instr[32];
    inputs(instr, 32);
    decoder_set_instruction(&dec, instr);
    decoder_propagate(&dec);
    bit outs[8] = {dec.alu_op[0], dec.alu_op[1], dec.alu_op[2], dec.alu_op[3],
                   dec.reg_write, dec.mem_read, dec.alu_src, dec.reg_dst};
    return nl_depth_of(outs, 8);
}

static void test_adder32_critical(void) {
    test_suite("timing: real adder_32 critical path");
    int d = adder32_depth();
    printf("  adder_32: %d NAND levels\n", d);
    CHECK(d > 32, "ripple carry dominates (> 1 level per bit)");
}

static void test_datapath_fmax(void) {
    test_suite("timing: single-cycle datapath Fmax from the real modules");

    int d_decode = decoder_depth();
    int d_regread = regfile_read_depth();
    int d_alu = alu_depth();
    int d_adder = adder32_depth();
    int decode_or_read = d_decode > d_regread ? d_decode : d_regread;
    int critical = decode_or_read + d_alu;

    printf("  decode           : %d\n", d_decode);
    printf("  regfile read     : %d\n", d_regread);
    printf("  ALU              : %d\n", d_alu);
    printf("  critical (logic) : %d NAND levels\n", critical);
    printf("  at 100 ps/level  : %.1f ns -> Fmax %.1f MHz (memory and setup excluded)\n",
           critical * 0.1, 10000.0 / critical);

    CHECK(d_alu >= d_adder, "ALU path contains the adder path");
    CHECK(d_alu > d_regread, "ALU path longer than register read");
    CHECK(d_alu > d_decode, "ALU path longer than decode");
}

static sim_time_t g_fire_log[8];
static int g_fire_count = 0;

static void ev_record(void *ctx) {
    scheduler_t *s = (scheduler_t *)ctx;
    g_fire_log[g_fire_count++] = scheduler_get_time(s);
}

static void test_scheduler_ordering(void) {
    test_suite("timing: event scheduler fires in time order");

    scheduler_t sched;
    scheduler_init(&sched);

    scheduler_schedule(&sched, 30, ev_record, &sched);
    scheduler_schedule(&sched, 10, ev_record, &sched);
    scheduler_schedule(&sched, 20, ev_record, &sched);
    scheduler_schedule(&sched, 5, ev_record, &sched);

    g_fire_count = 0;
    int i;
    for (i = 0; i < 4; i++) scheduler_step(&sched);

    CHECK_EQ_U32((uint32_t)g_fire_log[0], 5, "first event at t=5");
    CHECK_EQ_U32((uint32_t)g_fire_log[1], 10, "second event at t=10");
    CHECK_EQ_U32((uint32_t)g_fire_log[2], 20, "third event at t=20");
    CHECK_EQ_U32((uint32_t)g_fire_log[3], 30, "fourth event at t=30");
}

static int g_tag_log[8];
static int g_tag_count = 0;
static int g_tags[4] = {0, 1, 2, 3};

static void ev_tag(void *ctx) {
    g_tag_log[g_tag_count++] = *(int *)ctx;
}

static void test_scheduler_fifo_and_full(void) {
    test_suite("timing: same-time events fire in arrival order; full queue reports it");

    scheduler_t sched;
    scheduler_init(&sched);
    int k;
    for (k = 0; k < 4; k++) scheduler_schedule(&sched, 7, ev_tag, &g_tags[k]);
    g_tag_count = 0;
    for (k = 0; k < 4; k++) scheduler_step(&sched);
    for (k = 0; k < 4; k++) CHECK_EQ_U32((uint32_t)g_tag_log[k], (uint32_t)k, "arrival order kept");

    scheduler_init(&sched);
    int rc = 0;
    for (k = 0; k < MAX_EVENTS; k++) rc |= scheduler_schedule(&sched, 1, ev_tag, &g_tags[0]);
    CHECK_EQ_U32((uint32_t)rc, 0u, "first MAX_EVENTS accepted");
    CHECK_EQ_U32((uint32_t)scheduler_schedule(&sched, 1, ev_tag, &g_tags[0]), (uint32_t)-1, "overflow rejected");
}

int main(void) {
    test_adder32_critical();
    test_datapath_fmax();
    test_scheduler_ordering();
    test_scheduler_fifo_and_full();
    return test_report();
}
