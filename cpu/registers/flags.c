#include "flags.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void flags_init(flags_register_t *flags) {
    dff_reset(&flags->zero);
    dff_reset(&flags->negative);
    dff_reset(&flags->carry);
    dff_reset(&flags->overflow);
    flags->load = 0;
}

void flags_reset(flags_register_t *flags) {
    flags_init(flags);
}

void flags_set_load(flags_register_t *flags, bit load) {
    flags->load = load;
}

void flags_set_input(flags_register_t *flags, bit z, bit n, bit c, bit v) {
    flags->z_in = z;
    flags->n_in = n;
    flags->c_in = c;
    flags->v_in = v;
}

void flags_clock_rise(flags_register_t *flags) {
    dff_clock_rise(&flags->zero);
    dff_clock_rise(&flags->negative);
    dff_clock_rise(&flags->carry);
    dff_clock_rise(&flags->overflow);
}

void flags_clock_fall(flags_register_t *flags) {
    flags->zero.d = mux2(flags->zero.q, flags->z_in, flags->load);
    flags->negative.d = mux2(flags->negative.q, flags->n_in, flags->load);
    flags->carry.d = mux2(flags->carry.q, flags->c_in, flags->load);
    flags->overflow.d = mux2(flags->overflow.q, flags->v_in, flags->load);

    dff_clock_fall(&flags->zero);
    dff_clock_fall(&flags->negative);
    dff_clock_fall(&flags->carry);
    dff_clock_fall(&flags->overflow);
}

void flags_propagate(flags_register_t *flags) {
    (void)flags;
}

bit flags_get_zero(flags_register_t *flags) {
    return flags->zero.q;
}

bit flags_get_negative(flags_register_t *flags) {
    return flags->negative.q;
}

bit flags_get_carry(flags_register_t *flags) {
    return flags->carry.q;
}

bit flags_get_overflow(flags_register_t *flags) {
    return flags->overflow.q;
}
