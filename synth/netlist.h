#ifndef ALETHEIA_NETLIST_H
#define ALETHEIA_NETLIST_H

#include "../core/types.h"

enum { NL_CONST0,
       NL_CONST1,
       NL_INPUT,
       NL_NAND };

void nl_reset(void);
bit nl_input(void);
bit nl_nand(bit a, bit b);
int nl_count(void);
int nl_nands(void);
int nl_inputs(void);
void nl_eval(const unsigned char *input_vals, unsigned char *node_vals);
int nl_depth_of(const bit *outputs, int n);
int nl_cone_nands(const bit *outputs, int n);

#endif
