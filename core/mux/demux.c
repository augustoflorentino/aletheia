#include "demux.h"
#include "../gates/gates.h"

void demux2(bit in, bit sel, bit *out0, bit *out1) {
    *out0 = AND(in, NOT(sel));
    *out1 = AND(in, sel);
}

void demux4(bit in, bit sel0, bit sel1, bit *out0, bit *out1, bit *out2, bit *out3) {
    bit not_sel0 = NOT(sel0);
    bit not_sel1 = NOT(sel1);

    *out0 = AND3(in, not_sel1, not_sel0);
    *out1 = AND3(in, not_sel1, sel0);
    *out2 = AND3(in, sel1, not_sel0);
    *out3 = AND3(in, sel1, sel0);
}

void decoder_2to4(bit sel0, bit sel1, bit *out) {
    bit not_sel0 = NOT(sel0);
    bit not_sel1 = NOT(sel1);

    out[0] = AND(not_sel1, not_sel0);
    out[1] = AND(not_sel1, sel0);
    out[2] = AND(sel1, not_sel0);
    out[3] = AND(sel1, sel0);
}

void decoder_3to8(bit sel0, bit sel1, bit sel2, bit *out) {
    bit not_sel0 = NOT(sel0);
    bit not_sel1 = NOT(sel1);
    bit not_sel2 = NOT(sel2);

    out[0] = AND3(not_sel2, not_sel1, not_sel0);
    out[1] = AND3(not_sel2, not_sel1, sel0);
    out[2] = AND3(not_sel2, sel1, not_sel0);
    out[3] = AND3(not_sel2, sel1, sel0);
    out[4] = AND3(sel2, not_sel1, not_sel0);
    out[5] = AND3(sel2, not_sel1, sel0);
    out[6] = AND3(sel2, sel1, not_sel0);
    out[7] = AND3(sel2, sel1, sel0);
}
