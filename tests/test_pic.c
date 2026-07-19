#include "test.h"

#include "../io/devices/pic.h"

static void test_pic_flow(void) {
    test_suite("pic: raise -> request -> ack");

    pic_t p;
    pic_init(&p);
    pic_reset(&p);

    bit mask[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    pic_write_mask(&p, mask);
    pic_set_enable(&p, 1);
    pic_clock_fall(&p);
    pic_clock_rise(&p);

    bit irq[8] = {0, 0, 0, 1, 0, 0, 0, 0};
    pic_raise(&p, irq);
    pic_clock_fall(&p);
    pic_clock_rise(&p);
    pic_propagate(&p);

    CHECK_EQ_BIT(p.int_request, 1, "int request asserted");
    CHECK_EQ_U32(from_bits_n(p.int_vector, 3), 3u, "vector points to line 3");

    bit no_irq[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bit vec[3];
    to_bits_n(3u, vec, 3);
    pic_raise(&p, no_irq);
    pic_ack(&p, vec);
    pic_clock_fall(&p);
    pic_clock_rise(&p);
    pic_propagate(&p);

    CHECK_EQ_BIT(p.int_request, 0, "int request cleared after ack");
}

static void test_pic_mask(void) {
    test_suite("pic: masked IRQ does not request");

    pic_t p;
    pic_init(&p);
    pic_reset(&p);

    bit mask[8] = {1, 1, 0, 1, 1, 1, 1, 1};
    pic_write_mask(&p, mask);
    pic_set_enable(&p, 1);
    pic_clock_fall(&p);
    pic_clock_rise(&p);

    bit irq[8] = {0, 0, 1, 0, 0, 0, 0, 0};
    pic_raise(&p, irq);
    pic_clock_fall(&p);
    pic_clock_rise(&p);
    pic_propagate(&p);

    CHECK_EQ_BIT(p.int_request, 0, "masked line 2 does not request");
}

int main(void) {
    test_pic_flow();
    test_pic_mask();
    return test_report();
}
