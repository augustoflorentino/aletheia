#ifndef ALETHEIA_ISS_H
#define ALETHEIA_ISS_H

#include <stdint.h>
#include "../system/ram.h"
#include "../isa/opcodes.h"

#define ISS_WORDS          RAM_WORDS
#define ISS_ROM_WORDS      1024
#define ISS_VEC_UNDEFINED  0x10u
#define ISS_VEC_ADDR_ERROR 0x50u
#define ISS_VEC_INTERRUPT  0x70u
#define ISS_OUT_MAX        4096
#define ISS_KEYS_MAX       256

typedef struct {
    uint32_t counter, reload;
    int enabled, auto_reload, pending;
} iss_timer_t;

typedef struct {
    uint32_t src, dst, count;
    int enabled;
} iss_dma_t;

typedef struct {
    uint32_t reg[32];
    uint32_t ram[ISS_WORDS];
    uint32_t pc;
    int halted;
    uint32_t epc;
    int in_exception, in_service;
    uint32_t pic_mask;
    int mmu_enabled, mmu_allowed;
    uint32_t pte[16];
    uint32_t dram[16];
    uint32_t gpio[4];
    iss_timer_t timer;
    iss_dma_t dma;
    unsigned char keys[ISS_KEYS_MAX];
    int key_head, key_tail;
    char out[ISS_OUT_MAX];
    int out_len;
    unsigned long long steps;
} iss_t;

void iss_reset(iss_t *c);
void iss_type(iss_t *c, const char *text);
void iss_step(iss_t *c, const uint32_t *rom);
unsigned long long iss_execute(iss_t *c, const uint32_t *rom, unsigned long long max_steps);

#endif
