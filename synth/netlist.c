#include "netlist.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char type;
    bit a, b;
    int in_idx;
} nl_node_t;

static nl_node_t *nodes;
static int count, cap, nands, inputs;
static int *depth;
static unsigned char *mark;

static bit add_node(unsigned char type, bit a, bit b, int in_idx) {
    if (count == cap) {
        cap = cap ? cap * 2 : 1 << 16;
        nodes = realloc(nodes, (size_t)cap * sizeof *nodes);
        if (nodes == NULL) {
            fprintf(stderr, "netlist: out of memory\n");
            exit(1);
        }
    }
    nodes[count] = (nl_node_t){type, a, b, in_idx};
    return (bit)count++;
}

void nl_reset(void) {
    count = 0;
    nands = 0;
    inputs = 0;
    add_node(NL_CONST0, 0, 0, 0);
    add_node(NL_CONST1, 0, 0, 0);
}

bit nl_input(void) {
    return add_node(NL_INPUT, 0, 0, inputs++);
}

bit nl_nand(bit a, bit b) {
    nands++;
    return add_node(NL_NAND, a, b, 0);
}

int nl_count(void) {
    return count;
}
int nl_nands(void) {
    return nands;
}
int nl_inputs(void) {
    return inputs;
}

void nl_eval(const unsigned char *input_vals, unsigned char *node_vals) {
    for (int i = 0; i < count; i++) {
        switch (nodes[i].type) {
            case NL_CONST0: node_vals[i] = 0; break;
            case NL_CONST1: node_vals[i] = 1; break;
            case NL_INPUT: node_vals[i] = input_vals[nodes[i].in_idx]; break;
            default: node_vals[i] = !(node_vals[nodes[i].a] & node_vals[nodes[i].b]); break;
        }
    }
}

static void ensure_scratch(void) {
    depth = realloc(depth, (size_t)count * sizeof *depth);
    mark = realloc(mark, (size_t)count);
    if (depth == NULL || mark == NULL) {
        fprintf(stderr, "netlist: out of memory\n");
        exit(1);
    }
}

int nl_depth_of(const bit *outputs, int n) {
    ensure_scratch();
    for (int i = 0; i < count; i++) {
        int d = 0;
        if (nodes[i].type == NL_NAND) {
            int da = depth[nodes[i].a], db = depth[nodes[i].b];
            d = (da > db ? da : db) + 1;
        }
        depth[i] = d;
    }
    int maxd = 0;
    for (int k = 0; k < n; k++) {
        if (depth[outputs[k]] > maxd) maxd = depth[outputs[k]];
    }
    return maxd;
}

int nl_cone_nands(const bit *outputs, int n) {
    ensure_scratch();
    for (int i = 0; i < count; i++) mark[i] = 0;
    for (int k = 0; k < n; k++) mark[outputs[k]] = 1;
    int total = 0;
    for (int i = count - 1; i >= 0; i--) {
        if (!mark[i] || nodes[i].type != NL_NAND) continue;
        total++;
        mark[nodes[i].a] = 1;
        mark[nodes[i].b] = 1;
    }
    return total;
}
