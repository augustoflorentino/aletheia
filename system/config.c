#include "config.h"
#include "../core/gates/and.h"
#include "../core/gates/not.h"
#include "../core/mux/mux2.h"

void config_init(system_config_t *cfg) {
    dff_reset(&cfg->boot_mode[0]); dff_reset(&cfg->boot_mode[1]);
    dff_reset(&cfg->clock_select[0]); dff_reset(&cfg->clock_select[1]);
    dff_reset(&cfg->wait_states[0]); dff_reset(&cfg->wait_states[1]);
    dff_reset(&cfg->bus_width[0]); dff_reset(&cfg->bus_width[1]);

    cfg->boot_mode_in[0] = 0; cfg->boot_mode_in[1] = 0;
    cfg->clock_select_in[0] = 0; cfg->clock_select_in[1] = 0;
    cfg->wait_states_in[0] = 0; cfg->wait_states_in[1] = 0;
    cfg->bus_width_in[0] = 0; cfg->bus_width_in[1] = 0;

    cfg->boot_mode_out[0] = 0; cfg->boot_mode_out[1] = 0;
    cfg->clock_select_out[0] = 0; cfg->clock_select_out[1] = 0;
    cfg->wait_states_out[0] = 0; cfg->wait_states_out[1] = 0;
    cfg->bus_width_out[0] = 0; cfg->bus_width_out[1] = 0;

    cfg->chip_select = 0;
    cfg->write = 0;
    cfg->addr[0] = 0;
    cfg->addr[1] = 0;
}

void config_reset(system_config_t *cfg) {
    config_init(cfg);
}

void config_set_cs(system_config_t *cfg, bit cs) {
    cfg->chip_select = cs;
}

void config_set_write(system_config_t *cfg, bit wr) {
    cfg->write = wr;
}

void config_set_addr(system_config_t *cfg, bit a0, bit a1) {
    cfg->addr[0] = a0;
    cfg->addr[1] = a1;
}

void config_set_data(system_config_t *cfg, bit *data) {
    cfg->boot_mode_in[0] = data[0]; cfg->boot_mode_in[1] = data[1];
    cfg->clock_select_in[0] = data[0]; cfg->clock_select_in[1] = data[1];
    cfg->wait_states_in[0] = data[0]; cfg->wait_states_in[1] = data[1];
    cfg->bus_width_in[0] = data[0]; cfg->bus_width_in[1] = data[1];
}

void config_clock_rise(system_config_t *cfg) {
    dff_clock_rise(&cfg->boot_mode[0]); dff_clock_rise(&cfg->boot_mode[1]);
    dff_clock_rise(&cfg->clock_select[0]); dff_clock_rise(&cfg->clock_select[1]);
    dff_clock_rise(&cfg->wait_states[0]); dff_clock_rise(&cfg->wait_states[1]);
    dff_clock_rise(&cfg->bus_width[0]); dff_clock_rise(&cfg->bus_width[1]);
}

void config_clock_fall(system_config_t *cfg) {
    bit active = cfg->chip_select;
    bit wr = AND(cfg->write, active);
    bit na0 = NOT(cfg->addr[0]);
    bit na1 = NOT(cfg->addr[1]);

    bit sel_boot = AND(na1, na0);
    bit sel_clk = AND(na1, cfg->addr[0]);
    bit sel_wait = AND(cfg->addr[1], na0);
    bit sel_bus = AND(cfg->addr[1], cfg->addr[0]);

    bit wr_boot = AND(wr, sel_boot);
    bit wr_clk = AND(wr, sel_clk);
    bit wr_wait = AND(wr, sel_wait);
    bit wr_bus = AND(wr, sel_bus);

    cfg->boot_mode[0].d = mux2(cfg->boot_mode[0].q, cfg->boot_mode_in[0], wr_boot);
    cfg->boot_mode[1].d = mux2(cfg->boot_mode[1].q, cfg->boot_mode_in[1], wr_boot);

    cfg->clock_select[0].d = mux2(cfg->clock_select[0].q, cfg->clock_select_in[0], wr_clk);
    cfg->clock_select[1].d = mux2(cfg->clock_select[1].q, cfg->clock_select_in[1], wr_clk);

    cfg->wait_states[0].d = mux2(cfg->wait_states[0].q, cfg->wait_states_in[0], wr_wait);
    cfg->wait_states[1].d = mux2(cfg->wait_states[1].q, cfg->wait_states_in[1], wr_wait);

    cfg->bus_width[0].d = mux2(cfg->bus_width[0].q, cfg->bus_width_in[0], wr_bus);
    cfg->bus_width[1].d = mux2(cfg->bus_width[1].q, cfg->bus_width_in[1], wr_bus);

    dff_clock_fall(&cfg->boot_mode[0]); dff_clock_fall(&cfg->boot_mode[1]);
    dff_clock_fall(&cfg->clock_select[0]); dff_clock_fall(&cfg->clock_select[1]);
    dff_clock_fall(&cfg->wait_states[0]); dff_clock_fall(&cfg->wait_states[1]);
    dff_clock_fall(&cfg->bus_width[0]); dff_clock_fall(&cfg->bus_width[1]);
}

void config_propagate(system_config_t *cfg) {
    cfg->boot_mode_out[0] = cfg->boot_mode[0].q;
    cfg->boot_mode_out[1] = cfg->boot_mode[1].q;

    cfg->clock_select_out[0] = cfg->clock_select[0].q;
    cfg->clock_select_out[1] = cfg->clock_select[1].q;

    cfg->wait_states_out[0] = cfg->wait_states[0].q;
    cfg->wait_states_out[1] = cfg->wait_states[1].q;

    cfg->bus_width_out[0] = cfg->bus_width[0].q;
    cfg->bus_width_out[1] = cfg->bus_width[1].q;
}

void config_get_boot_mode(system_config_t *cfg, bit *out) {
    out[0] = cfg->boot_mode_out[0];
    out[1] = cfg->boot_mode_out[1];
}

void config_get_clock_select(system_config_t *cfg, bit *out) {
    out[0] = cfg->clock_select_out[0];
    out[1] = cfg->clock_select_out[1];
}

void config_get_wait_states(system_config_t *cfg, bit *out) {
    out[0] = cfg->wait_states_out[0];
    out[1] = cfg->wait_states_out[1];
}

void config_get_bus_width(system_config_t *cfg, bit *out) {
    out[0] = cfg->bus_width_out[0];
    out[1] = cfg->bus_width_out[1];
}
