#include "seven_seg.h"
#include "../../core/gates/gates.h"

void bcd_to_seven_seg(bit bcd[4], bit seg[7]) {
    bit b0 = bcd[0];
    bit b1 = bcd[1];
    bit b2 = bcd[2];
    bit b3 = bcd[3];
    bit n0 = NOT(b0);
    bit n1 = NOT(b1);
    bit n2 = NOT(b2);
    bit n3 = NOT(b3);

    bit is0 = AND(AND(n3, n2), AND(n1, n0));
    bit is1 = AND(AND(n3, n2), AND(n1, b0));
    bit is2 = AND(AND(n3, n2), AND(b1, n0));
    bit is3 = AND(AND(n3, n2), AND(b1, b0));
    bit is4 = AND(AND(n3, b2), AND(n1, n0));
    bit is5 = AND(AND(n3, b2), AND(n1, b0));
    bit is6 = AND(AND(n3, b2), AND(b1, n0));
    bit is7 = AND(AND(n3, b2), AND(b1, b0));
    bit is8 = AND(AND(b3, n2), AND(n1, n0));
    bit is9 = AND(AND(b3, n2), AND(n1, b0));

    seg[0] = OR(OR(OR(is0, is2), OR(is3, is5)), OR(OR(is6, is7), OR(is8, is9)));
    seg[1] = OR(OR(OR(is0, is1), OR(is2, is3)), OR(OR(is4, is7), OR(is8, is9)));
    seg[2] = OR(OR(OR(is0, is1), OR(is3, is4)), OR(OR(is5, is6), OR(is7, OR(is8, is9))));
    seg[3] = OR(OR(OR(is0, is2), OR(is3, is5)), OR(OR(is6, is8), is9));
    seg[4] = OR(OR(is0, is2), OR(is6, is8));
    seg[5] = OR(OR(OR(is0, is4), OR(is5, is6)), OR(is8, is9));
    seg[6] = OR(OR(OR(is2, is3), OR(is4, is5)), OR(OR(is6, is8), is9));
}
