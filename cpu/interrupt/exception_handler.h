#ifndef ALETHEIA_EXCEPTION_HANDLER_H
#define ALETHEIA_EXCEPTION_HANDLER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define EXC_RESET        0
#define EXC_UNDEFINED    1
#define EXC_SYSCALL      2
#define EXC_BREAK        3
#define EXC_OVERFLOW     4
#define EXC_ADDR_ERROR   5
#define EXC_BUS_ERROR    6
#define EXC_INTERRUPT    7

typedef struct {
    d_flip_flop_t epc[32];
    d_flip_flop_t cause[4];
    d_flip_flop_t in_exception;

    bit exc_reset;
    bit exc_undefined;
    bit exc_syscall;
    bit exc_break;
    bit exc_overflow;
    bit exc_addr_error;
    bit exc_bus_error;
    bit exc_interrupt;
    bit exc_return;

    bit pc_in[32];
    bit interrupt_vector[3];

    bit exception_taken;
    bit handler_addr[32];
    bit return_addr[32];
} exception_handler_t;

void exc_init(exception_handler_t *eh);
void exc_reset(exception_handler_t *eh);

void exc_set_reset(exception_handler_t *eh, bit reset);
void exc_set_undefined(exception_handler_t *eh, bit undefined);
void exc_set_syscall(exception_handler_t *eh, bit syscall);
void exc_set_break(exception_handler_t *eh, bit brk);
void exc_set_overflow(exception_handler_t *eh, bit overflow);
void exc_set_addr_error(exception_handler_t *eh, bit addr_error);
void exc_set_bus_error(exception_handler_t *eh, bit bus_error);
void exc_set_interrupt(exception_handler_t *eh, bit interrupt, bit *vector);
void exc_set_return(exception_handler_t *eh, bit ret);
void exc_set_pc(exception_handler_t *eh, bit *pc);

void exc_clock_rise(exception_handler_t *eh);
void exc_clock_fall(exception_handler_t *eh);
void exc_propagate(exception_handler_t *eh);

bit exc_get_exception_taken(exception_handler_t *eh);
void exc_get_handler_addr(exception_handler_t *eh, bit *out);
void exc_get_return_addr(exception_handler_t *eh, bit *out);
void exc_get_cause(exception_handler_t *eh, bit *out);

#endif
