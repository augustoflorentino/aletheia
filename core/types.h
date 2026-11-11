#ifndef ALETHEIA_TYPES_H
#define ALETHEIA_TYPES_H

#if defined(ALETHEIA_NETLIST)
typedef unsigned int bit;
#elif defined(ALETHEIA_LANES)
typedef unsigned long long bit;
#else
typedef unsigned char bit;
#endif

typedef bit byte_t[8];
typedef bit word_t[32];
typedef bit dword_t[64];

typedef bit address_t[32];
typedef bit opcode_t[8];

typedef struct {
    word_t data;
    address_t addr;
    bit read;
    bit write;
    bit ready;
} bus_signals_t;

typedef struct {
    void *state;
    void (*reset)(void *self);
    void (*clock_rise)(void *self);
    void (*clock_fall)(void *self);
    void (*propagate)(void *self);
} component_t;

#endif
