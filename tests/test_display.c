#include "test.h"

#include "../io/devices/display.h"

static void dwrite(display_t *d, uint32_t pos, uint32_t ch) {
    bit a[3], c[8];
    to_bits_n(pos, a, 3);
    to_bits_n(ch, c, 8);
    display_set_addr(d, a);
    display_set_data(d, c);
    display_set_write(d, 1);
    display_clock_fall(d);
    display_clock_rise(d);
    display_set_write(d, 0);
}

static uint32_t dread(display_t *d, uint32_t pos) {
    bit a[3];
    to_bits_n(pos, a, 3);
    display_set_addr(d, a);
    display_propagate(d);
    return from_bits_n(d->data_out, 8);
}

static void test_write_read(void) {
    test_suite("display: write/read char positions");

    display_t d;
    display_init(&d);
    display_reset(&d);

    const char *s = "ALETHEIA";
    int i;
    for (i = 0; i < 8; i++) dwrite(&d, (uint32_t)i, (uint32_t)s[i]);
    for (i = 0; i < 8; i++) {
        char label[24];
        snprintf(label, sizeof(label), "pos %d = '%c'", i, s[i]);
        CHECK_EQ_U32(dread(&d, (uint32_t)i), (uint32_t)s[i], label);
    }
}

static void test_independent(void) {
    test_suite("display: positions independent");

    display_t d;
    display_init(&d);
    display_reset(&d);

    dwrite(&d, 3, (uint32_t)'X');
    CHECK_EQ_U32(dread(&d, 3), (uint32_t)'X', "pos 3 set");
    CHECK_EQ_U32(dread(&d, 4), 0u, "pos 4 untouched");
    dwrite(&d, 3, (uint32_t)'Y');
    CHECK_EQ_U32(dread(&d, 3), (uint32_t)'Y', "pos 3 overwritten");
}

int main(void) {
    test_write_read();
    test_independent();
    return test_report();
}
