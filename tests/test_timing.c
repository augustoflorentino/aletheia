#include "test.h"

#include "../simulation/tgate.h"
#include "../simulation/scheduler.h"
#include "../core/gates/gates.h"

static void t_adder32(tsig *a, tsig *b, tsig cin, tsig *result, tsig *cout) {
    tsig c = cin;
    int i;
    for (i = 0; i < 32; i++) {
        tsig s, co;
        t_full_adder(a[i], b[i], c, &s, &co);
        result[i] = s;
        c = co;
    }
    *cout = c;
}

static void test_full_adder_timing(void) {
    test_suite("timing: full adder critical path");
    tgate_set_delay(1);

    tsig sum, cout;
    t_full_adder(ts(1), ts(1), ts(0), &sum, &cout);

    CHECK_EQ_U32((uint32_t)sum.t, 2, "full_adder sum arrival (2 gate delays)");
    CHECK_EQ_U32((uint32_t)cout.t, 3, "full_adder carry arrival (3 gate delays)");
}

static void test_adder32_critical(void) {
    test_suite("timing: ripple-carry adder_32 critical path");
    tgate_set_delay(1);

    tsig a[32], b[32], result[32], cout;
    int i;
    for (i = 0; i < 32; i++) { a[i] = ts(1); b[i] = ts(1); }

    tgate_reset_critical();
    t_adder32(a, b, ts(0), result, &cout);

    CHECK_EQ_U32((uint32_t)result[31].t, 64, "adder_32 MSB sum arrival");
    CHECK_EQ_U32((uint32_t)cout.t, 65, "adder_32 final carry-out arrival");
    CHECK_EQ_U32((uint32_t)tgate_critical(), 65, "adder_32 critical path = 65");
}

static sim_time_t alu_select_delay(void) {
    tgate_set_delay(1);
    tsig a[32], b[32], result[32], cout;
    int i;
    for (i = 0; i < 32; i++) { a[i] = ts(1); b[i] = ts(0); }
    tgate_reset_critical();
    t_adder32(a, b, ts(0), result, &cout);

    tsig r = result[31];
    tsig op0 = ts(1), op1 = ts(1), op2 = ts(1);
    tsig l1 = t_mux(r, r, op0);
    tsig l2 = t_mux(l1, l1, op1);
    tsig l3 = t_mux(l2, l2, op2);
    (void)l3;
    return tgate_critical();
}

static sim_time_t regfile_read_delay(void) {
    tgate_set_delay(1);
    tgate_reset_critical();
    tsig x = ts(1);
    tsig sel = ts(0);
    int lvl;
    for (lvl = 0; lvl < 5; lvl++) {
        x = t_mux(x, x, sel);
    }
    return tgate_critical();
}

static sim_time_t decoder_delay(void) {
    tgate_set_delay(1);
    tgate_reset_critical();
    tsig op0 = ts(1), op1 = ts(1), op2 = ts(0);
    tsig op3 = ts(0), op4 = ts(0), op5 = ts(1);
    tsig is_lw = t_and(t_and(op5, t_not(op4)),
                       t_and(t_not(op3), t_and(t_not(op2), t_and(op1, op0))));
    tsig alu_src = t_or(is_lw, is_lw);
    tsig mux_ctrl = t_mux(ts(0), ts(1), alu_src);
    (void)mux_ctrl;
    return tgate_critical();
}

static void test_datapath_fmax(void) {
    test_suite("timing: single-cycle datapath Fmax");
    tgate_set_delay(1);

    sim_time_t clk_to_q = 3;
    sim_time_t mem_access = 5;
    sim_time_t setup = 2;

    sim_time_t d_decode = decoder_delay();
    sim_time_t d_regread = regfile_read_delay();
    sim_time_t d_alu = alu_select_delay();
    sim_time_t d_wbmux = 3;

    sim_time_t decode_or_read = d_decode > d_regread ? d_decode : d_regread;

    sim_time_t critical = clk_to_q + mem_access + decode_or_read
                        + d_alu + mem_access + d_wbmux + setup;

    printf("  clk->q           : %llu\n", clk_to_q);
    printf("  instr mem access : %llu\n", mem_access);
    printf("  decode           : %llu\n", d_decode);
    printf("  regfile read     : %llu\n", d_regread);
    printf("  (max decode/read): %llu\n", decode_or_read);
    printf("  ALU              : %llu\n", d_alu);
    printf("  data mem access  : %llu\n", mem_access);
    printf("  writeback mux    : %llu\n", d_wbmux);
    printf("  setup            : %llu\n", setup);
    printf("  --------------------------------\n");
    printf("  critical path    : %llu gate-delay units\n", critical);
    printf("  assuming 1 unit  = 0.1 ns (gate delay 100 ps)\n");
    printf("  min clock period : %.1f ns\n", (double)critical * 0.1);
    printf("  estimated Fmax   : %.1f MHz\n", 10000.0 / (double)critical);

    CHECK(critical > 90, "critical path dominated by ALU carry chain (>90 units)");
    CHECK(critical > 10, "critical path exceeds default clock_period=10 (unrealistic default)");
    CHECK(d_alu > d_regread, "ALU path longer than regfile read path");
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

static int count_f_transitions(int hazard_free) {
    sim_time_t d = 1;
    sim_time_t Tc = 20;
    sim_time_t t;
    int transitions = 0;
    int last = -1;

    for (t = 0; t <= 40; t++) {
        bit c_2d = (t >= 2 * d && (t - 2 * d) < Tc) ? 1 : 0;
        bit c_3d = (t >= 3 * d && (t - 3 * d) < Tc) ? 1 : 0;

        bit f;
        if (hazard_free) {
            bit c_now = (t < Tc) ? 1 : 0;
            f = OR(c_now, NOT(c_now));
        } else {
            f = OR(c_2d, NOT(c_3d));
        }

        if (last >= 0 && f != last) transitions++;
        last = f;
    }
    return transitions;
}

static void test_glitch_detection(void) {
    test_suite("timing: static-1 hazard glitch detection");

    int glitchy = count_f_transitions(0);
    int clean = count_f_transitions(1);

    CHECK(glitchy >= 2, "hazard circuit glitches (>=2 transitions, spike to 0)");
    CHECK_EQ_U32((uint32_t)clean, 0, "zero-delay reference: no glitch");
    printf("  hazard f transitions: %d (expected spike), clean: %d\n", glitchy, clean);
}

int main(void) {
    test_full_adder_timing();
    test_adder32_critical();
    test_datapath_fmax();
    test_scheduler_ordering();
    test_glitch_detection();
    return test_report();
}
