#ifndef ALETHEIA_NETLIST_H
#define ALETHEIA_NETLIST_H

#include "../core/types.h"
#include "../core/gates/gates.h"

#define NL_MAX 200000

enum { NL_CONST0,
       NL_CONST1,
       NL_INPUT,
       NL_NAND };

typedef struct {
    int type;
    int a, b;
    int in_idx;
} nl_node_t;

static nl_node_t nl_nodes[NL_MAX];
static int nl_count;
static int nl_nands;
static int nl_inputs;

static void nl_reset(void) {
    nl_count = 0;
    nl_nands = 0;
    nl_inputs = 0;
}

static inline int nl_new(int type, int a, int b, int in_idx) {
    int id = nl_count++;
    nl_nodes[id].type = type;
    nl_nodes[id].a = a;
    nl_nodes[id].b = b;
    nl_nodes[id].in_idx = in_idx;
    return id;
}

static inline int nl_const0(void) {
    return nl_new(NL_CONST0, 0, 0, 0);
}
static inline int nl_const1(void) {
    return nl_new(NL_CONST1, 0, 0, 0);
}
static inline int nl_input(void) {
    int idx = nl_inputs++;
    return nl_new(NL_INPUT, 0, 0, idx);
}

static inline int nl_nand(int a, int b) {
    nl_nands++;
    return nl_new(NL_NAND, a, b, 0);
}

static inline int nl_not(int a) {
    return nl_nand(a, a);
}
static inline int nl_and(int a, int b) {
    return nl_not(nl_nand(a, b));
}
static inline int nl_or(int a, int b) {
    return nl_nand(nl_not(a), nl_not(b));
}
static inline int nl_nor(int a, int b) {
    return nl_not(nl_or(a, b));
}
static inline int nl_xor(int a, int b) {
    int c = nl_nand(a, b);
    return nl_nand(nl_nand(a, c), nl_nand(b, c));
}
static inline int nl_xnor(int a, int b) {
    return nl_not(nl_xor(a, b));
}
static inline int nl_mux(int a, int b, int s) {
    return nl_or(nl_and(a, nl_not(s)), nl_and(b, s));
}

static void nl_eval(const bit *input_vals, bit *node_vals) {
    int i;
    for (i = 0; i < nl_count; i++) {
        int t = nl_nodes[i].type;
        if (t == NL_CONST0) node_vals[i] = 0;
        else if (t == NL_CONST1) node_vals[i] = 1;
        else if (t == NL_INPUT) node_vals[i] = input_vals[nl_nodes[i].in_idx];
        else node_vals[i] = NAND(node_vals[nl_nodes[i].a], node_vals[nl_nodes[i].b]);
    }
}

static inline int nl_critical_depth(int *depth_scratch) {
    int i, maxd = 0;
    for (i = 0; i < nl_count; i++) {
        int d = 0;
        if (nl_nodes[i].type == NL_NAND) {
            int da = depth_scratch[nl_nodes[i].a];
            int db = depth_scratch[nl_nodes[i].b];
            d = (da > db ? da : db) + 1;
        }
        depth_scratch[i] = d;
        if (d > maxd) maxd = d;
    }
    return maxd;
}

#endif
