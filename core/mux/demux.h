#ifndef ALETHEIA_DEMUX_H
#define ALETHEIA_DEMUX_H

#include "../types.h"

void demux2(bit in, bit sel, bit *out0, bit *out1);
void demux4(bit in, bit sel0, bit sel1, bit *out0, bit *out1, bit *out2, bit *out3);

void decoder_2to4(bit sel0, bit sel1, bit *out);
void decoder_3to8(bit sel0, bit sel1, bit sel2, bit *out);

#endif
