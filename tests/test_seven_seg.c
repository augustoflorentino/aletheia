#include "test.h"

#include "../io/devices/seven_seg.h"

static const bit expected[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1},
};

static void test_seven_seg(void) {
    test_suite("seven_seg: BCD digit -> 7 segments");

    int d, s;
    for (d = 0; d <= 9; d++) {
        bit bcd[4], seg[7];
        to_bits_n((uint32_t)d, bcd, 4);
        bcd_to_seven_seg(bcd, seg);
        for (s = 0; s < 7; s++) {
            char label[24];
            snprintf(label, sizeof(label), "digit %d seg %c", d, 'a' + s);
            CHECK_EQ_BIT(seg[s], expected[d][s], label);
        }
    }

    bit bcd[4], seg[7];
    to_bits_n(15u, bcd, 4);
    bcd_to_seven_seg(bcd, seg);
    int any = 0;
    for (s = 0; s < 7; s++) any |= seg[s];
    CHECK_EQ_BIT(any, 0, "invalid digit 15 -> blank");
}

int main(void) {
    test_seven_seg();
    return test_report();
}
