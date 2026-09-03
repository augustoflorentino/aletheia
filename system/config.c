#include "config.h"

void config_init(system_config_t *cfg) {
    cfg->pin[STRAP_L1] = 1;
    cfg->pin[STRAP_L2] = 0;
    cfg->pin[STRAP_MMU] = 1;
    cfg->pin[STRAP_TDIV0] = 0;
    cfg->pin[STRAP_TDIV1] = 0;
    config_reset(cfg);
}

static void latch(d_flip_flop_t *ff, bit pin) {
    ff->d = pin;
    dff_clock_fall(ff);
    dff_clock_rise(ff);
}

void config_reset(system_config_t *cfg) {
    latch(&cfg->strap[STRAP_L1], cfg->pin[STRAP_L1]);
    latch(&cfg->strap[STRAP_L2], cfg->pin[STRAP_L2]);
    latch(&cfg->strap[STRAP_MMU], cfg->pin[STRAP_MMU]);
    latch(&cfg->strap[STRAP_TDIV0], cfg->pin[STRAP_TDIV0]);
    latch(&cfg->strap[STRAP_TDIV1], cfg->pin[STRAP_TDIV1]);
}

bit config_l1(system_config_t *cfg) {
    return cfg->strap[STRAP_L1].q;
}

bit config_l2(system_config_t *cfg) {
    return cfg->strap[STRAP_L2].q;
}

bit config_mmu(system_config_t *cfg) {
    return cfg->strap[STRAP_MMU].q;
}

void config_timer_div(system_config_t *cfg, bit *out) {
    out[0] = cfg->strap[STRAP_TDIV0].q;
    out[1] = cfg->strap[STRAP_TDIV1].q;
}
