#ifndef ALETHEIA_REFMODEL_H
#define ALETHEIA_REFMODEL_H

#include <stdint.h>

#include "../cpu/alu/alu.h"

static inline uint32_t ref_alu_result(int op, uint32_t a, uint32_t b) {
    switch (op) {
        case ALU_OP_ADD: return a + b;
        case ALU_OP_SUB: return a - b;
        case ALU_OP_AND: return a & b;
        case ALU_OP_OR: return a | b;
        case ALU_OP_XOR: return a ^ b;
        case ALU_OP_NOT: return ~a;
        case ALU_OP_SHL: return a << (b & 31u);
        case ALU_OP_SHR: return a >> (b & 31u);
        default: return 0;
    }
}

static inline bit ref_add_carry(uint32_t a, uint32_t b) {
    uint64_t full = (uint64_t)a + (uint64_t)b;
    return (bit)((full >> 32) & 1u);
}

static inline bit ref_add_overflow(uint32_t a, uint32_t b) {
    uint32_t r = a + b;
    uint32_t v = (~(a ^ b)) & (a ^ r) & 0x80000000u;
    return (bit)(v != 0);
}

#endif
