#include "divider.h"
#include "../gates/gates.h"

void divider_init(clock_divider_t *div) {
    tff_reset(&div->ff0);
    tff_reset(&div->ff1);
    tff_reset(&div->ff2);
    tff_reset(&div->ff3);

    tff_set_t(&div->ff0, 1);

    div->div2 = 0;
    div->div4 = 0;
    div->div8 = 0;
    div->div16 = 0;
}

void divider_clock_rise(clock_divider_t *div) {
    tff_clock_rise(&div->ff0);
    tff_clock_rise(&div->ff1);
    tff_clock_rise(&div->ff2);
    tff_clock_rise(&div->ff3);

    div->div2 = div->ff0.q;
    div->div4 = div->ff1.q;
    div->div8 = div->ff2.q;
    div->div16 = div->ff3.q;
}

void divider_clock_fall(clock_divider_t *div) {
    tff_set_t(&div->ff1, div->ff0.q);
    tff_set_t(&div->ff2, AND(div->ff0.q, div->ff1.q));
    tff_set_t(&div->ff3, AND3(div->ff0.q, div->ff1.q, div->ff2.q));

    tff_clock_fall(&div->ff0);
    tff_clock_fall(&div->ff1);
    tff_clock_fall(&div->ff2);
    tff_clock_fall(&div->ff3);
}
