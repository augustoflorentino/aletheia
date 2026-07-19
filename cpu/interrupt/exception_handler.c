#include "exception_handler.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void exc_init(exception_handler_t *eh) {
    int i;
    i = 0; dff_reset(&eh->epc[i]); i = 1; dff_reset(&eh->epc[i]);
    i = 2; dff_reset(&eh->epc[i]); i = 3; dff_reset(&eh->epc[i]);
    i = 4; dff_reset(&eh->epc[i]); i = 5; dff_reset(&eh->epc[i]);
    i = 6; dff_reset(&eh->epc[i]); i = 7; dff_reset(&eh->epc[i]);
    i = 8; dff_reset(&eh->epc[i]); i = 9; dff_reset(&eh->epc[i]);
    i = 10; dff_reset(&eh->epc[i]); i = 11; dff_reset(&eh->epc[i]);
    i = 12; dff_reset(&eh->epc[i]); i = 13; dff_reset(&eh->epc[i]);
    i = 14; dff_reset(&eh->epc[i]); i = 15; dff_reset(&eh->epc[i]);
    i = 16; dff_reset(&eh->epc[i]); i = 17; dff_reset(&eh->epc[i]);
    i = 18; dff_reset(&eh->epc[i]); i = 19; dff_reset(&eh->epc[i]);
    i = 20; dff_reset(&eh->epc[i]); i = 21; dff_reset(&eh->epc[i]);
    i = 22; dff_reset(&eh->epc[i]); i = 23; dff_reset(&eh->epc[i]);
    i = 24; dff_reset(&eh->epc[i]); i = 25; dff_reset(&eh->epc[i]);
    i = 26; dff_reset(&eh->epc[i]); i = 27; dff_reset(&eh->epc[i]);
    i = 28; dff_reset(&eh->epc[i]); i = 29; dff_reset(&eh->epc[i]);
    i = 30; dff_reset(&eh->epc[i]); i = 31; dff_reset(&eh->epc[i]);

    dff_reset(&eh->cause[0]); dff_reset(&eh->cause[1]);
    dff_reset(&eh->cause[2]); dff_reset(&eh->cause[3]);

    dff_reset(&eh->status[0]); dff_reset(&eh->status[1]);
    dff_reset(&eh->status[2]); dff_reset(&eh->status[3]);
    dff_reset(&eh->status[4]); dff_reset(&eh->status[5]);
    dff_reset(&eh->status[6]); dff_reset(&eh->status[7]);

    dff_reset(&eh->in_exception);

    eh->exc_reset = 0;
    eh->exc_undefined = 0;
    eh->exc_syscall = 0;
    eh->exc_break = 0;
    eh->exc_overflow = 0;
    eh->exc_addr_error = 0;
    eh->exc_bus_error = 0;
    eh->exc_interrupt = 0;
    eh->exc_return = 0;

    i = 0; eh->pc_in[i] = 0; i = 1; eh->pc_in[i] = 0;
    i = 2; eh->pc_in[i] = 0; i = 3; eh->pc_in[i] = 0;
    i = 4; eh->pc_in[i] = 0; i = 5; eh->pc_in[i] = 0;
    i = 6; eh->pc_in[i] = 0; i = 7; eh->pc_in[i] = 0;
    i = 8; eh->pc_in[i] = 0; i = 9; eh->pc_in[i] = 0;
    i = 10; eh->pc_in[i] = 0; i = 11; eh->pc_in[i] = 0;
    i = 12; eh->pc_in[i] = 0; i = 13; eh->pc_in[i] = 0;
    i = 14; eh->pc_in[i] = 0; i = 15; eh->pc_in[i] = 0;
    i = 16; eh->pc_in[i] = 0; i = 17; eh->pc_in[i] = 0;
    i = 18; eh->pc_in[i] = 0; i = 19; eh->pc_in[i] = 0;
    i = 20; eh->pc_in[i] = 0; i = 21; eh->pc_in[i] = 0;
    i = 22; eh->pc_in[i] = 0; i = 23; eh->pc_in[i] = 0;
    i = 24; eh->pc_in[i] = 0; i = 25; eh->pc_in[i] = 0;
    i = 26; eh->pc_in[i] = 0; i = 27; eh->pc_in[i] = 0;
    i = 28; eh->pc_in[i] = 0; i = 29; eh->pc_in[i] = 0;
    i = 30; eh->pc_in[i] = 0; i = 31; eh->pc_in[i] = 0;

    eh->interrupt_vector[0] = 0;
    eh->interrupt_vector[1] = 0;
    eh->interrupt_vector[2] = 0;

    eh->exception_taken = 0;

    i = 0; eh->handler_addr[i] = 0; i = 1; eh->handler_addr[i] = 0;
    i = 2; eh->handler_addr[i] = 0; i = 3; eh->handler_addr[i] = 0;
    i = 4; eh->handler_addr[i] = 0; i = 5; eh->handler_addr[i] = 0;
    i = 6; eh->handler_addr[i] = 0; i = 7; eh->handler_addr[i] = 0;
    i = 8; eh->handler_addr[i] = 0; i = 9; eh->handler_addr[i] = 0;
    i = 10; eh->handler_addr[i] = 0; i = 11; eh->handler_addr[i] = 0;
    i = 12; eh->handler_addr[i] = 0; i = 13; eh->handler_addr[i] = 0;
    i = 14; eh->handler_addr[i] = 0; i = 15; eh->handler_addr[i] = 0;
    i = 16; eh->handler_addr[i] = 0; i = 17; eh->handler_addr[i] = 0;
    i = 18; eh->handler_addr[i] = 0; i = 19; eh->handler_addr[i] = 0;
    i = 20; eh->handler_addr[i] = 0; i = 21; eh->handler_addr[i] = 0;
    i = 22; eh->handler_addr[i] = 0; i = 23; eh->handler_addr[i] = 0;
    i = 24; eh->handler_addr[i] = 0; i = 25; eh->handler_addr[i] = 0;
    i = 26; eh->handler_addr[i] = 0; i = 27; eh->handler_addr[i] = 0;
    i = 28; eh->handler_addr[i] = 0; i = 29; eh->handler_addr[i] = 0;
    i = 30; eh->handler_addr[i] = 0; i = 31; eh->handler_addr[i] = 0;

    i = 0; eh->return_addr[i] = 0; i = 1; eh->return_addr[i] = 0;
    i = 2; eh->return_addr[i] = 0; i = 3; eh->return_addr[i] = 0;
    i = 4; eh->return_addr[i] = 0; i = 5; eh->return_addr[i] = 0;
    i = 6; eh->return_addr[i] = 0; i = 7; eh->return_addr[i] = 0;
    i = 8; eh->return_addr[i] = 0; i = 9; eh->return_addr[i] = 0;
    i = 10; eh->return_addr[i] = 0; i = 11; eh->return_addr[i] = 0;
    i = 12; eh->return_addr[i] = 0; i = 13; eh->return_addr[i] = 0;
    i = 14; eh->return_addr[i] = 0; i = 15; eh->return_addr[i] = 0;
    i = 16; eh->return_addr[i] = 0; i = 17; eh->return_addr[i] = 0;
    i = 18; eh->return_addr[i] = 0; i = 19; eh->return_addr[i] = 0;
    i = 20; eh->return_addr[i] = 0; i = 21; eh->return_addr[i] = 0;
    i = 22; eh->return_addr[i] = 0; i = 23; eh->return_addr[i] = 0;
    i = 24; eh->return_addr[i] = 0; i = 25; eh->return_addr[i] = 0;
    i = 26; eh->return_addr[i] = 0; i = 27; eh->return_addr[i] = 0;
    i = 28; eh->return_addr[i] = 0; i = 29; eh->return_addr[i] = 0;
    i = 30; eh->return_addr[i] = 0; i = 31; eh->return_addr[i] = 0;
}

void exc_reset(exception_handler_t *eh) {
    exc_init(eh);
}

void exc_set_reset(exception_handler_t *eh, bit reset) {
    eh->exc_reset = reset;
}

void exc_set_undefined(exception_handler_t *eh, bit undefined) {
    eh->exc_undefined = undefined;
}

void exc_set_syscall(exception_handler_t *eh, bit syscall) {
    eh->exc_syscall = syscall;
}

void exc_set_break(exception_handler_t *eh, bit brk) {
    eh->exc_break = brk;
}

void exc_set_overflow(exception_handler_t *eh, bit overflow) {
    eh->exc_overflow = overflow;
}

void exc_set_addr_error(exception_handler_t *eh, bit addr_error) {
    eh->exc_addr_error = addr_error;
}

void exc_set_bus_error(exception_handler_t *eh, bit bus_error) {
    eh->exc_bus_error = bus_error;
}

void exc_set_interrupt(exception_handler_t *eh, bit interrupt, bit *vector) {
    eh->exc_interrupt = interrupt;
    eh->interrupt_vector[0] = vector[0];
    eh->interrupt_vector[1] = vector[1];
    eh->interrupt_vector[2] = vector[2];
}

void exc_set_return(exception_handler_t *eh, bit ret) {
    eh->exc_return = ret;
}

void exc_set_pc(exception_handler_t *eh, bit *pc) {
    eh->pc_in[0] = pc[0]; eh->pc_in[1] = pc[1]; eh->pc_in[2] = pc[2]; eh->pc_in[3] = pc[3];
    eh->pc_in[4] = pc[4]; eh->pc_in[5] = pc[5]; eh->pc_in[6] = pc[6]; eh->pc_in[7] = pc[7];
    eh->pc_in[8] = pc[8]; eh->pc_in[9] = pc[9]; eh->pc_in[10] = pc[10]; eh->pc_in[11] = pc[11];
    eh->pc_in[12] = pc[12]; eh->pc_in[13] = pc[13]; eh->pc_in[14] = pc[14]; eh->pc_in[15] = pc[15];
    eh->pc_in[16] = pc[16]; eh->pc_in[17] = pc[17]; eh->pc_in[18] = pc[18]; eh->pc_in[19] = pc[19];
    eh->pc_in[20] = pc[20]; eh->pc_in[21] = pc[21]; eh->pc_in[22] = pc[22]; eh->pc_in[23] = pc[23];
    eh->pc_in[24] = pc[24]; eh->pc_in[25] = pc[25]; eh->pc_in[26] = pc[26]; eh->pc_in[27] = pc[27];
    eh->pc_in[28] = pc[28]; eh->pc_in[29] = pc[29]; eh->pc_in[30] = pc[30]; eh->pc_in[31] = pc[31];
}

void exc_clock_rise(exception_handler_t *eh) {
    int i;
    i = 0; dff_clock_rise(&eh->epc[i]); i = 1; dff_clock_rise(&eh->epc[i]);
    i = 2; dff_clock_rise(&eh->epc[i]); i = 3; dff_clock_rise(&eh->epc[i]);
    i = 4; dff_clock_rise(&eh->epc[i]); i = 5; dff_clock_rise(&eh->epc[i]);
    i = 6; dff_clock_rise(&eh->epc[i]); i = 7; dff_clock_rise(&eh->epc[i]);
    i = 8; dff_clock_rise(&eh->epc[i]); i = 9; dff_clock_rise(&eh->epc[i]);
    i = 10; dff_clock_rise(&eh->epc[i]); i = 11; dff_clock_rise(&eh->epc[i]);
    i = 12; dff_clock_rise(&eh->epc[i]); i = 13; dff_clock_rise(&eh->epc[i]);
    i = 14; dff_clock_rise(&eh->epc[i]); i = 15; dff_clock_rise(&eh->epc[i]);
    i = 16; dff_clock_rise(&eh->epc[i]); i = 17; dff_clock_rise(&eh->epc[i]);
    i = 18; dff_clock_rise(&eh->epc[i]); i = 19; dff_clock_rise(&eh->epc[i]);
    i = 20; dff_clock_rise(&eh->epc[i]); i = 21; dff_clock_rise(&eh->epc[i]);
    i = 22; dff_clock_rise(&eh->epc[i]); i = 23; dff_clock_rise(&eh->epc[i]);
    i = 24; dff_clock_rise(&eh->epc[i]); i = 25; dff_clock_rise(&eh->epc[i]);
    i = 26; dff_clock_rise(&eh->epc[i]); i = 27; dff_clock_rise(&eh->epc[i]);
    i = 28; dff_clock_rise(&eh->epc[i]); i = 29; dff_clock_rise(&eh->epc[i]);
    i = 30; dff_clock_rise(&eh->epc[i]); i = 31; dff_clock_rise(&eh->epc[i]);

    dff_clock_rise(&eh->cause[0]); dff_clock_rise(&eh->cause[1]);
    dff_clock_rise(&eh->cause[2]); dff_clock_rise(&eh->cause[3]);

    dff_clock_rise(&eh->status[0]); dff_clock_rise(&eh->status[1]);
    dff_clock_rise(&eh->status[2]); dff_clock_rise(&eh->status[3]);
    dff_clock_rise(&eh->status[4]); dff_clock_rise(&eh->status[5]);
    dff_clock_rise(&eh->status[6]); dff_clock_rise(&eh->status[7]);

    dff_clock_rise(&eh->in_exception);
}

void exc_clock_fall(exception_handler_t *eh) {
    bit any_exc = OR(OR(OR(eh->exc_reset, eh->exc_undefined),
                        OR(eh->exc_syscall, eh->exc_break)),
                     OR(OR(eh->exc_overflow, eh->exc_addr_error),
                        OR(eh->exc_bus_error, eh->exc_interrupt)));

    bit save_epc = AND(any_exc, NOT(eh->in_exception.q));

    eh->epc[0].d = mux2(eh->epc[0].q, eh->pc_in[0], save_epc);
    eh->epc[1].d = mux2(eh->epc[1].q, eh->pc_in[1], save_epc);
    eh->epc[2].d = mux2(eh->epc[2].q, eh->pc_in[2], save_epc);
    eh->epc[3].d = mux2(eh->epc[3].q, eh->pc_in[3], save_epc);
    eh->epc[4].d = mux2(eh->epc[4].q, eh->pc_in[4], save_epc);
    eh->epc[5].d = mux2(eh->epc[5].q, eh->pc_in[5], save_epc);
    eh->epc[6].d = mux2(eh->epc[6].q, eh->pc_in[6], save_epc);
    eh->epc[7].d = mux2(eh->epc[7].q, eh->pc_in[7], save_epc);
    eh->epc[8].d = mux2(eh->epc[8].q, eh->pc_in[8], save_epc);
    eh->epc[9].d = mux2(eh->epc[9].q, eh->pc_in[9], save_epc);
    eh->epc[10].d = mux2(eh->epc[10].q, eh->pc_in[10], save_epc);
    eh->epc[11].d = mux2(eh->epc[11].q, eh->pc_in[11], save_epc);
    eh->epc[12].d = mux2(eh->epc[12].q, eh->pc_in[12], save_epc);
    eh->epc[13].d = mux2(eh->epc[13].q, eh->pc_in[13], save_epc);
    eh->epc[14].d = mux2(eh->epc[14].q, eh->pc_in[14], save_epc);
    eh->epc[15].d = mux2(eh->epc[15].q, eh->pc_in[15], save_epc);
    eh->epc[16].d = mux2(eh->epc[16].q, eh->pc_in[16], save_epc);
    eh->epc[17].d = mux2(eh->epc[17].q, eh->pc_in[17], save_epc);
    eh->epc[18].d = mux2(eh->epc[18].q, eh->pc_in[18], save_epc);
    eh->epc[19].d = mux2(eh->epc[19].q, eh->pc_in[19], save_epc);
    eh->epc[20].d = mux2(eh->epc[20].q, eh->pc_in[20], save_epc);
    eh->epc[21].d = mux2(eh->epc[21].q, eh->pc_in[21], save_epc);
    eh->epc[22].d = mux2(eh->epc[22].q, eh->pc_in[22], save_epc);
    eh->epc[23].d = mux2(eh->epc[23].q, eh->pc_in[23], save_epc);
    eh->epc[24].d = mux2(eh->epc[24].q, eh->pc_in[24], save_epc);
    eh->epc[25].d = mux2(eh->epc[25].q, eh->pc_in[25], save_epc);
    eh->epc[26].d = mux2(eh->epc[26].q, eh->pc_in[26], save_epc);
    eh->epc[27].d = mux2(eh->epc[27].q, eh->pc_in[27], save_epc);
    eh->epc[28].d = mux2(eh->epc[28].q, eh->pc_in[28], save_epc);
    eh->epc[29].d = mux2(eh->epc[29].q, eh->pc_in[29], save_epc);
    eh->epc[30].d = mux2(eh->epc[30].q, eh->pc_in[30], save_epc);
    eh->epc[31].d = mux2(eh->epc[31].q, eh->pc_in[31], save_epc);

    bit cause_val[4];
    cause_val[0] = OR(eh->exc_undefined, OR(eh->exc_break, OR(eh->exc_addr_error, eh->exc_interrupt)));
    cause_val[1] = OR(eh->exc_syscall, OR(eh->exc_break, OR(eh->exc_bus_error, eh->exc_interrupt)));
    cause_val[2] = OR(eh->exc_overflow, OR(eh->exc_addr_error, OR(eh->exc_bus_error, eh->exc_interrupt)));
    cause_val[3] = 0;

    eh->cause[0].d = mux2(eh->cause[0].q, cause_val[0], save_epc);
    eh->cause[1].d = mux2(eh->cause[1].q, cause_val[1], save_epc);
    eh->cause[2].d = mux2(eh->cause[2].q, cause_val[2], save_epc);
    eh->cause[3].d = mux2(eh->cause[3].q, cause_val[3], save_epc);

    eh->in_exception.d = AND(mux2(eh->in_exception.q, 1, save_epc), NOT(eh->exc_return));

    int i;
    i = 0; dff_clock_fall(&eh->epc[i]); i = 1; dff_clock_fall(&eh->epc[i]);
    i = 2; dff_clock_fall(&eh->epc[i]); i = 3; dff_clock_fall(&eh->epc[i]);
    i = 4; dff_clock_fall(&eh->epc[i]); i = 5; dff_clock_fall(&eh->epc[i]);
    i = 6; dff_clock_fall(&eh->epc[i]); i = 7; dff_clock_fall(&eh->epc[i]);
    i = 8; dff_clock_fall(&eh->epc[i]); i = 9; dff_clock_fall(&eh->epc[i]);
    i = 10; dff_clock_fall(&eh->epc[i]); i = 11; dff_clock_fall(&eh->epc[i]);
    i = 12; dff_clock_fall(&eh->epc[i]); i = 13; dff_clock_fall(&eh->epc[i]);
    i = 14; dff_clock_fall(&eh->epc[i]); i = 15; dff_clock_fall(&eh->epc[i]);
    i = 16; dff_clock_fall(&eh->epc[i]); i = 17; dff_clock_fall(&eh->epc[i]);
    i = 18; dff_clock_fall(&eh->epc[i]); i = 19; dff_clock_fall(&eh->epc[i]);
    i = 20; dff_clock_fall(&eh->epc[i]); i = 21; dff_clock_fall(&eh->epc[i]);
    i = 22; dff_clock_fall(&eh->epc[i]); i = 23; dff_clock_fall(&eh->epc[i]);
    i = 24; dff_clock_fall(&eh->epc[i]); i = 25; dff_clock_fall(&eh->epc[i]);
    i = 26; dff_clock_fall(&eh->epc[i]); i = 27; dff_clock_fall(&eh->epc[i]);
    i = 28; dff_clock_fall(&eh->epc[i]); i = 29; dff_clock_fall(&eh->epc[i]);
    i = 30; dff_clock_fall(&eh->epc[i]); i = 31; dff_clock_fall(&eh->epc[i]);

    dff_clock_fall(&eh->cause[0]); dff_clock_fall(&eh->cause[1]);
    dff_clock_fall(&eh->cause[2]); dff_clock_fall(&eh->cause[3]);

    dff_clock_fall(&eh->status[0]); dff_clock_fall(&eh->status[1]);
    dff_clock_fall(&eh->status[2]); dff_clock_fall(&eh->status[3]);
    dff_clock_fall(&eh->status[4]); dff_clock_fall(&eh->status[5]);
    dff_clock_fall(&eh->status[6]); dff_clock_fall(&eh->status[7]);

    dff_clock_fall(&eh->in_exception);
}

void exc_propagate(exception_handler_t *eh) {
    bit any_exc = OR(OR(OR(eh->exc_reset, eh->exc_undefined),
                        OR(eh->exc_syscall, eh->exc_break)),
                     OR(OR(eh->exc_overflow, eh->exc_addr_error),
                        OR(eh->exc_bus_error, eh->exc_interrupt)));

    eh->exception_taken = AND(any_exc, NOT(eh->in_exception.q));

    eh->handler_addr[0] = 0; eh->handler_addr[1] = 0;
    eh->handler_addr[2] = 0; eh->handler_addr[3] = 0;

    eh->handler_addr[4] = OR(eh->exc_undefined, OR(eh->exc_break, OR(eh->exc_addr_error, eh->exc_interrupt)));
    eh->handler_addr[5] = OR(eh->exc_syscall, OR(eh->exc_break, OR(eh->exc_bus_error, eh->exc_interrupt)));
    eh->handler_addr[6] = OR(eh->exc_overflow, OR(eh->exc_addr_error, OR(eh->exc_bus_error, eh->exc_interrupt)));
    eh->handler_addr[7] = 0;

    eh->handler_addr[8] = 0;
    eh->handler_addr[9] = 0;
    eh->handler_addr[10] = 0;
    eh->handler_addr[11] = 0;

    eh->handler_addr[12] = 0; eh->handler_addr[13] = 0;
    eh->handler_addr[14] = 0; eh->handler_addr[15] = 0;
    eh->handler_addr[16] = 0; eh->handler_addr[17] = 0;
    eh->handler_addr[18] = 0; eh->handler_addr[19] = 0;
    eh->handler_addr[20] = 0; eh->handler_addr[21] = 0;
    eh->handler_addr[22] = 0; eh->handler_addr[23] = 0;
    eh->handler_addr[24] = 0; eh->handler_addr[25] = 0;
    eh->handler_addr[26] = 0; eh->handler_addr[27] = 0;
    eh->handler_addr[28] = 0; eh->handler_addr[29] = 0;
    eh->handler_addr[30] = 0; eh->handler_addr[31] = 0;

    eh->return_addr[0] = eh->epc[0].q; eh->return_addr[1] = eh->epc[1].q;
    eh->return_addr[2] = eh->epc[2].q; eh->return_addr[3] = eh->epc[3].q;
    eh->return_addr[4] = eh->epc[4].q; eh->return_addr[5] = eh->epc[5].q;
    eh->return_addr[6] = eh->epc[6].q; eh->return_addr[7] = eh->epc[7].q;
    eh->return_addr[8] = eh->epc[8].q; eh->return_addr[9] = eh->epc[9].q;
    eh->return_addr[10] = eh->epc[10].q; eh->return_addr[11] = eh->epc[11].q;
    eh->return_addr[12] = eh->epc[12].q; eh->return_addr[13] = eh->epc[13].q;
    eh->return_addr[14] = eh->epc[14].q; eh->return_addr[15] = eh->epc[15].q;
    eh->return_addr[16] = eh->epc[16].q; eh->return_addr[17] = eh->epc[17].q;
    eh->return_addr[18] = eh->epc[18].q; eh->return_addr[19] = eh->epc[19].q;
    eh->return_addr[20] = eh->epc[20].q; eh->return_addr[21] = eh->epc[21].q;
    eh->return_addr[22] = eh->epc[22].q; eh->return_addr[23] = eh->epc[23].q;
    eh->return_addr[24] = eh->epc[24].q; eh->return_addr[25] = eh->epc[25].q;
    eh->return_addr[26] = eh->epc[26].q; eh->return_addr[27] = eh->epc[27].q;
    eh->return_addr[28] = eh->epc[28].q; eh->return_addr[29] = eh->epc[29].q;
    eh->return_addr[30] = eh->epc[30].q; eh->return_addr[31] = eh->epc[31].q;
}

bit exc_get_exception_taken(exception_handler_t *eh) {
    return eh->exception_taken;
}

void exc_get_handler_addr(exception_handler_t *eh, bit *out) {
    out[0] = eh->handler_addr[0]; out[1] = eh->handler_addr[1];
    out[2] = eh->handler_addr[2]; out[3] = eh->handler_addr[3];
    out[4] = eh->handler_addr[4]; out[5] = eh->handler_addr[5];
    out[6] = eh->handler_addr[6]; out[7] = eh->handler_addr[7];
    out[8] = eh->handler_addr[8]; out[9] = eh->handler_addr[9];
    out[10] = eh->handler_addr[10]; out[11] = eh->handler_addr[11];
    out[12] = eh->handler_addr[12]; out[13] = eh->handler_addr[13];
    out[14] = eh->handler_addr[14]; out[15] = eh->handler_addr[15];
    out[16] = eh->handler_addr[16]; out[17] = eh->handler_addr[17];
    out[18] = eh->handler_addr[18]; out[19] = eh->handler_addr[19];
    out[20] = eh->handler_addr[20]; out[21] = eh->handler_addr[21];
    out[22] = eh->handler_addr[22]; out[23] = eh->handler_addr[23];
    out[24] = eh->handler_addr[24]; out[25] = eh->handler_addr[25];
    out[26] = eh->handler_addr[26]; out[27] = eh->handler_addr[27];
    out[28] = eh->handler_addr[28]; out[29] = eh->handler_addr[29];
    out[30] = eh->handler_addr[30]; out[31] = eh->handler_addr[31];
}

void exc_get_return_addr(exception_handler_t *eh, bit *out) {
    out[0] = eh->return_addr[0]; out[1] = eh->return_addr[1];
    out[2] = eh->return_addr[2]; out[3] = eh->return_addr[3];
    out[4] = eh->return_addr[4]; out[5] = eh->return_addr[5];
    out[6] = eh->return_addr[6]; out[7] = eh->return_addr[7];
    out[8] = eh->return_addr[8]; out[9] = eh->return_addr[9];
    out[10] = eh->return_addr[10]; out[11] = eh->return_addr[11];
    out[12] = eh->return_addr[12]; out[13] = eh->return_addr[13];
    out[14] = eh->return_addr[14]; out[15] = eh->return_addr[15];
    out[16] = eh->return_addr[16]; out[17] = eh->return_addr[17];
    out[18] = eh->return_addr[18]; out[19] = eh->return_addr[19];
    out[20] = eh->return_addr[20]; out[21] = eh->return_addr[21];
    out[22] = eh->return_addr[22]; out[23] = eh->return_addr[23];
    out[24] = eh->return_addr[24]; out[25] = eh->return_addr[25];
    out[26] = eh->return_addr[26]; out[27] = eh->return_addr[27];
    out[28] = eh->return_addr[28]; out[29] = eh->return_addr[29];
    out[30] = eh->return_addr[30]; out[31] = eh->return_addr[31];
}

void exc_get_cause(exception_handler_t *eh, bit *out) {
    out[0] = eh->cause[0].q;
    out[1] = eh->cause[1].q;
    out[2] = eh->cause[2].q;
    out[3] = eh->cause[3].q;
}
