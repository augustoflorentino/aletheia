#include "iss.h"
#include "../isa/opcodes.h"
#include <string.h>

typedef struct {
    uint32_t value;
    int fault;
} access_t;

void iss_reset(iss_t *c) {
    memset(c, 0, sizeof *c);
    c->mmu_allowed = 1;
}

void iss_type(iss_t *c, const char *text) {
    for (; text && *text && c->key_tail < ISS_KEYS_MAX; text++) c->keys[c->key_tail++] = (unsigned char)*text;
}

static uint32_t shift_right_arith(uint32_t v, uint32_t n) {
    uint32_t fill = (v & 0x80000000u) && n ? ~(0xFFFFFFFFu >> n) : 0u;
    return (v >> n) | fill;
}

static int is_io(uint32_t va) {
    return (va & 0xFFFFFF80u) == 0xFFFFFF00u && (va & 0x70u) <= 0x40u;
}

static int translate(iss_t *c, uint32_t va, uint32_t *pa) {
    int mapping = (c->mmu_enabled && c->mmu_allowed) || c->user;
    *pa = va;
    if (!mapping) return 0;
    uint32_t vpn = (va >> 4) & 0xFFFFFu;
    uint32_t pte = vpn < 16 && (va >> 24) == 0 ? c->pte[vpn] : 0;
    if (!(pte & (1u << 24))) return 1;
    *pa = ((pte & 0xFFFFFu) << 4) | (va & 15u);
    return c->user && !(pte & (1u << 19));
}

static void dma_start(iss_t *c) {
    for (; c->dma.count; c->dma.count--) {
        c->ram[c->dma.dst & (ISS_WORDS - 1)] = c->ram[c->dma.src & (ISS_WORDS - 1)];
        c->dma.src = (c->dma.src + 1) & 0xFFFFu;
        c->dma.dst = (c->dma.dst + 1) & 0xFFFFu;
    }
}

static uint32_t timer_read(iss_t *c, uint32_t r) {
    uint32_t status = (uint32_t)c->timer.pending;
    uint32_t v[4] = {c->timer.counter, c->timer.reload,
                     (uint32_t)c->timer.enabled | (uint32_t)c->timer.auto_reload << 1, status};
    if (r == 3) c->timer.pending = 0;
    return v[r];
}

static uint32_t sys_read(iss_t *c, uint32_t r) {
    uint32_t dma[4] = {c->dma.src, c->dma.dst, c->dma.count,
                       (uint32_t)c->dma.enabled | (uint32_t)(c->dma.enabled && !c->dma.count) << 1};
    uint32_t hi[4] = {c->pic_mask, 0, (uint32_t)c->mmu_enabled, 0};
    if (r >= 8) return dma[r & 3];
    return r < 4 ? timer_read(c, r) : hi[r & 3];
}

static uint32_t io_read(iss_t *c, uint32_t va) {
    uint32_t off = va & 0x7Fu;
    if (off >= 0x40) return c->dram[off & 15u];
    if (off == 0x00) return 1u | (uint32_t)(c->key_head < c->key_tail) << 1;
    if (off == 0x01) return c->key_head < c->key_tail ? c->keys[c->key_head++] : 0;
    if (off >= 0x20 && off < 0x24) return c->gpio[off & 3u];
    if (off >= 0x30 && off < 0x3C) return sys_read(c, off - 0x30);
    return 0;
}

static void sys_write(iss_t *c, uint32_t r, uint32_t v) {
    if (r == 0) c->timer.counter = v & 0xFFFFu;
    if (r == 1) c->timer.reload = v & 0xFFFFu;
    if (r == 2) {
        c->timer.enabled = (int)(v & 1u);
        c->timer.auto_reload = (int)((v >> 1) & 1u);
    }
    if (r == 4) c->pic_mask = v & 0xFFu;
    if (r == 5) c->pte[(v >> 20) & 15u] = v & 0x1FFFFFFu;
    if (r == 6) c->mmu_enabled = (int)(v & 1u);
    if (r == 7 && (v & 1u)) c->user = 1;
    if (r == 8) c->dma.src = v & 0xFFFFu;
    if (r == 9) c->dma.dst = v & 0xFFFFu;
    if (r == 10) c->dma.count = v & 0xFFFFu;
    if (r == 11) c->dma.enabled = (int)(v & 1u);
    if (r == 11 && c->dma.enabled) dma_start(c);
}

static void io_write(iss_t *c, uint32_t va, uint32_t v) {
    uint32_t off = va & 0x7Fu;
    if (off >= 0x40) c->dram[off & 15u] = v;
    if (off == 0x00 && c->out_len < ISS_OUT_MAX) c->out[c->out_len++] = (char)(v & 0xFFu);
    if (off >= 0x20 && off < 0x24) c->gpio[off & 3u] = v;
    if (off >= 0x30 && off < 0x3C) sys_write(c, off - 0x30, v);
}

static access_t mem_access(iss_t *c, uint32_t va, uint32_t wdata, int write) {
    access_t a = {0, 0};
    if (is_io(va)) {
        a.fault = c->user;
        if (a.fault) return a;
        if (write) io_write(c, va, wdata);
        else a.value = io_read(c, va);
        return a;
    }
    uint32_t pa;
    a.fault = translate(c, va, &pa);
    if (a.fault) return a;
    if (write) c->ram[pa & (ISS_WORDS - 1)] = wdata;
    else a.value = c->ram[pa & (ISS_WORDS - 1)];
    return a;
}

static void take(iss_t *c, uint32_t vector) {
    if (!c->in_exception) c->epc = c->pc;
    c->in_exception = 1;
    c->user_prev = c->user;
    c->user = 0;
    c->pc = vector;
}

static void timer_tick(iss_t *c) {
    if (!c->timer.enabled || !c->timer.counter) return;
    c->timer.counter--;
    if (c->timer.counter) return;
    c->timer.pending = 1;
    if (c->timer.auto_reload) c->timer.counter = c->timer.reload;
}

static int rtype(iss_t *c, uint32_t funct, uint32_t vs, uint32_t vt, uint32_t sh, uint32_t *res, uint32_t *next) {
    switch (funct) {
        case FUNCT_ADD: *res = vs + vt; return 1;
        case FUNCT_SUB: *res = vs - vt; return 1;
        case FUNCT_AND: *res = vs & vt; return 1;
        case FUNCT_OR: *res = vs | vt; return 1;
        case FUNCT_XOR: *res = vs ^ vt; return 1;
        case FUNCT_NOR: *res = ~(vs | vt); return 1;
        case FUNCT_SLT: *res = (int32_t)vs < (int32_t)vt; return 1;
        case FUNCT_SLTU: *res = vs < vt; return 1;
        case FUNCT_SLL: *res = vt << sh; return 1;
        case FUNCT_SRL: *res = vt >> sh; return 1;
        case FUNCT_SRA: *res = shift_right_arith(vt, sh); return 1;
        case FUNCT_SLLV: *res = vt << (vs & 31u); return 1;
        case FUNCT_SRLV: *res = vt >> (vs & 31u); return 1;
        case FUNCT_SRAV: *res = shift_right_arith(vt, vs & 31u); return 1;
        case FUNCT_JR: *next = vs; return 0;
        case FUNCT_JALR:
            *res = c->pc + 1u;
            *next = vs;
            return 1;
        default: return -1;
    }
}

static int interrupt_due(iss_t *c) {
    return c->timer.pending && (c->pic_mask & 1u) && !c->in_service && !c->in_exception;
}

static void reti(iss_t *c, uint32_t *next) {
    *next = c->epc;
    c->in_exception = 0;
    c->in_service = 0;
    c->user = c->user_prev;
}

void iss_step(iss_t *c, const uint32_t *rom) {
    if (c->halted) return;
    c->steps++;
    timer_tick(c);
    if (interrupt_due(c)) {
        take(c, ISS_VEC_INTERRUPT);
        c->in_service = 1;
        return;
    }
    uint32_t in = rom[c->pc & (ISS_ROM_WORDS - 1)];
    uint32_t op = in >> 26, rs = (in >> 21) & 31u, rt = (in >> 16) & 31u;
    uint32_t rd = (in >> 11) & 31u, sh = (in >> 6) & 31u, funct = in & 0x3Fu;
    uint32_t imm_s = (uint32_t)(int32_t)(int16_t)(in & 0xFFFFu);
    uint32_t imm_z = in & 0xFFFFu;
    uint32_t vs = c->reg[rs], vt = c->reg[rt];
    uint32_t next = c->pc + 1u, res = 0;
    int wreg = -1, undefined = 0;
    access_t a = {0, 0};

    switch (op) {
        case OP_RTYPE: {
            int r = rtype(c, funct, vs, vt, sh, &res, &next);
            undefined = r < 0;
            if (r > 0) wreg = (int)rd;
            break;
        }
        case OP_ADDI:
            res = vs + imm_s;
            wreg = (int)rt;
            break;
        case OP_SLTI:
            res = (int32_t)vs < (int32_t)imm_s;
            wreg = (int)rt;
            break;
        case OP_SLTIU:
            res = vs < imm_s;
            wreg = (int)rt;
            break;
        case OP_ANDI:
            res = vs & imm_z;
            wreg = (int)rt;
            break;
        case OP_ORI:
            res = vs | imm_z;
            wreg = (int)rt;
            break;
        case OP_XORI:
            res = vs ^ imm_z;
            wreg = (int)rt;
            break;
        case OP_LUI:
            res = imm_z << 16;
            wreg = (int)rt;
            break;
        case OP_LW:
            a = mem_access(c, vs + imm_s, 0, 0);
            res = a.value;
            wreg = (int)rt;
            break;
        case OP_SW: a = mem_access(c, vs + imm_s, vt, 1); break;
        case OP_BEQ:
            if (vs == vt) next = c->pc + 1u + imm_s;
            break;
        case OP_BNE:
            if (vs != vt) next = c->pc + 1u + imm_s;
            break;
        case OP_J: next = in & 0x3FFFFFFu; break;
        case OP_JAL:
            res = c->pc + 1u;
            wreg = 31;
            next = in & 0x3FFFFFFu;
            break;
        case OP_RETI:
            undefined = c->user;
            if (!c->user) reti(c, &next);
            break;
        case OP_HALT: c->halted = 1; return;
        default: undefined = 1; break;
    }
    if (undefined) {
        take(c, ISS_VEC_UNDEFINED);
        return;
    }
    if (a.fault) {
        take(c, ISS_VEC_ADDR_ERROR);
        return;
    }
    if (wreg > 0) c->reg[wreg] = res;
    c->pc = next;
}

unsigned long long iss_execute(iss_t *c, const uint32_t *rom, unsigned long long max_steps) {
    unsigned long long start = c->steps;
    while (!c->halted && c->steps - start < max_steps) iss_step(c, rom);
    return c->steps - start;
}
