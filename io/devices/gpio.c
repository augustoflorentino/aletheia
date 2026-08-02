#include "gpio.h"
#include "../../core/gates/and.h"
#include "../../core/gates/or.h"
#include "../../core/gates/not.h"
#include "../../core/mux/mux2.h"

void gpio_init(gpio_t *gpio) {
    dff_reset(&gpio->output[0]); dff_reset(&gpio->output[1]);
    dff_reset(&gpio->output[2]); dff_reset(&gpio->output[3]);
    dff_reset(&gpio->output[4]); dff_reset(&gpio->output[5]);
    dff_reset(&gpio->output[6]); dff_reset(&gpio->output[7]);

    dff_reset(&gpio->direction[0]); dff_reset(&gpio->direction[1]);
    dff_reset(&gpio->direction[2]); dff_reset(&gpio->direction[3]);
    dff_reset(&gpio->direction[4]); dff_reset(&gpio->direction[5]);
    dff_reset(&gpio->direction[6]); dff_reset(&gpio->direction[7]);

    gpio->input[0] = 0; gpio->input[1] = 0;
    gpio->input[2] = 0; gpio->input[3] = 0;
    gpio->input[4] = 0; gpio->input[5] = 0;
    gpio->input[6] = 0; gpio->input[7] = 0;

    gpio->output_data[0] = 0; gpio->output_data[1] = 0;
    gpio->output_data[2] = 0; gpio->output_data[3] = 0;
    gpio->output_data[4] = 0; gpio->output_data[5] = 0;
    gpio->output_data[6] = 0; gpio->output_data[7] = 0;

    gpio->dir_data[0] = 0; gpio->dir_data[1] = 0;
    gpio->dir_data[2] = 0; gpio->dir_data[3] = 0;
    gpio->dir_data[4] = 0; gpio->dir_data[5] = 0;
    gpio->dir_data[6] = 0; gpio->dir_data[7] = 0;

    gpio->pin_out[0] = 0; gpio->pin_out[1] = 0;
    gpio->pin_out[2] = 0; gpio->pin_out[3] = 0;
    gpio->pin_out[4] = 0; gpio->pin_out[5] = 0;
    gpio->pin_out[6] = 0; gpio->pin_out[7] = 0;

    gpio->pin_read[0] = 0; gpio->pin_read[1] = 0;
    gpio->pin_read[2] = 0; gpio->pin_read[3] = 0;
    gpio->pin_read[4] = 0; gpio->pin_read[5] = 0;
    gpio->pin_read[6] = 0; gpio->pin_read[7] = 0;

    gpio->chip_select = 0;
    gpio->read = 0;
    gpio->write = 0;
    gpio->addr[0] = 0;
    gpio->addr[1] = 0;
}

void gpio_reset(gpio_t *gpio) {
    gpio_init(gpio);
}

void gpio_set_cs(gpio_t *gpio, bit cs) {
    gpio->chip_select = cs;
}

void gpio_set_read(gpio_t *gpio, bit rd) {
    gpio->read = rd;
}

void gpio_set_write(gpio_t *gpio, bit wr) {
    gpio->write = wr;
}

void gpio_set_addr(gpio_t *gpio, bit a0, bit a1) {
    gpio->addr[0] = a0;
    gpio->addr[1] = a1;
}

void gpio_set_data(gpio_t *gpio, bit *data) {
    gpio->output_data[0] = data[0]; gpio->output_data[1] = data[1];
    gpio->output_data[2] = data[2]; gpio->output_data[3] = data[3];
    gpio->output_data[4] = data[4]; gpio->output_data[5] = data[5];
    gpio->output_data[6] = data[6]; gpio->output_data[7] = data[7];

    gpio->dir_data[0] = data[0]; gpio->dir_data[1] = data[1];
    gpio->dir_data[2] = data[2]; gpio->dir_data[3] = data[3];
    gpio->dir_data[4] = data[4]; gpio->dir_data[5] = data[5];
    gpio->dir_data[6] = data[6]; gpio->dir_data[7] = data[7];
}

void gpio_set_pins(gpio_t *gpio, bit *pins) {
    gpio->input[0] = pins[0]; gpio->input[1] = pins[1];
    gpio->input[2] = pins[2]; gpio->input[3] = pins[3];
    gpio->input[4] = pins[4]; gpio->input[5] = pins[5];
    gpio->input[6] = pins[6]; gpio->input[7] = pins[7];
}

void gpio_clock_rise(gpio_t *gpio) {
    dff_clock_rise(&gpio->output[0]); dff_clock_rise(&gpio->output[1]);
    dff_clock_rise(&gpio->output[2]); dff_clock_rise(&gpio->output[3]);
    dff_clock_rise(&gpio->output[4]); dff_clock_rise(&gpio->output[5]);
    dff_clock_rise(&gpio->output[6]); dff_clock_rise(&gpio->output[7]);

    dff_clock_rise(&gpio->direction[0]); dff_clock_rise(&gpio->direction[1]);
    dff_clock_rise(&gpio->direction[2]); dff_clock_rise(&gpio->direction[3]);
    dff_clock_rise(&gpio->direction[4]); dff_clock_rise(&gpio->direction[5]);
    dff_clock_rise(&gpio->direction[6]); dff_clock_rise(&gpio->direction[7]);

}

void gpio_clock_fall(gpio_t *gpio) {
    bit active = gpio->chip_select;
    bit wr = AND(gpio->write, active);
    bit na0 = NOT(gpio->addr[0]);
    bit na1 = NOT(gpio->addr[1]);

    bit sel_out = AND(na1, na0);
    bit sel_dir = AND(na1, gpio->addr[0]);

    bit wr_out = AND(wr, sel_out);
    bit wr_dir = AND(wr, sel_dir);

    gpio->output[0].d = mux2(gpio->output[0].q, gpio->output_data[0], wr_out);
    gpio->output[1].d = mux2(gpio->output[1].q, gpio->output_data[1], wr_out);
    gpio->output[2].d = mux2(gpio->output[2].q, gpio->output_data[2], wr_out);
    gpio->output[3].d = mux2(gpio->output[3].q, gpio->output_data[3], wr_out);
    gpio->output[4].d = mux2(gpio->output[4].q, gpio->output_data[4], wr_out);
    gpio->output[5].d = mux2(gpio->output[5].q, gpio->output_data[5], wr_out);
    gpio->output[6].d = mux2(gpio->output[6].q, gpio->output_data[6], wr_out);
    gpio->output[7].d = mux2(gpio->output[7].q, gpio->output_data[7], wr_out);

    gpio->direction[0].d = mux2(gpio->direction[0].q, gpio->dir_data[0], wr_dir);
    gpio->direction[1].d = mux2(gpio->direction[1].q, gpio->dir_data[1], wr_dir);
    gpio->direction[2].d = mux2(gpio->direction[2].q, gpio->dir_data[2], wr_dir);
    gpio->direction[3].d = mux2(gpio->direction[3].q, gpio->dir_data[3], wr_dir);
    gpio->direction[4].d = mux2(gpio->direction[4].q, gpio->dir_data[4], wr_dir);
    gpio->direction[5].d = mux2(gpio->direction[5].q, gpio->dir_data[5], wr_dir);
    gpio->direction[6].d = mux2(gpio->direction[6].q, gpio->dir_data[6], wr_dir);
    gpio->direction[7].d = mux2(gpio->direction[7].q, gpio->dir_data[7], wr_dir);

    dff_clock_fall(&gpio->output[0]); dff_clock_fall(&gpio->output[1]);
    dff_clock_fall(&gpio->output[2]); dff_clock_fall(&gpio->output[3]);
    dff_clock_fall(&gpio->output[4]); dff_clock_fall(&gpio->output[5]);
    dff_clock_fall(&gpio->output[6]); dff_clock_fall(&gpio->output[7]);

    dff_clock_fall(&gpio->direction[0]); dff_clock_fall(&gpio->direction[1]);
    dff_clock_fall(&gpio->direction[2]); dff_clock_fall(&gpio->direction[3]);
    dff_clock_fall(&gpio->direction[4]); dff_clock_fall(&gpio->direction[5]);
    dff_clock_fall(&gpio->direction[6]); dff_clock_fall(&gpio->direction[7]);

}

void gpio_propagate(gpio_t *gpio) {
    bit d0 = gpio->direction[0].q; bit d1 = gpio->direction[1].q;
    bit d2 = gpio->direction[2].q; bit d3 = gpio->direction[3].q;
    bit d4 = gpio->direction[4].q; bit d5 = gpio->direction[5].q;
    bit d6 = gpio->direction[6].q; bit d7 = gpio->direction[7].q;

    gpio->pin_out[0] = AND(gpio->output[0].q, d0);
    gpio->pin_out[1] = AND(gpio->output[1].q, d1);
    gpio->pin_out[2] = AND(gpio->output[2].q, d2);
    gpio->pin_out[3] = AND(gpio->output[3].q, d3);
    gpio->pin_out[4] = AND(gpio->output[4].q, d4);
    gpio->pin_out[5] = AND(gpio->output[5].q, d5);
    gpio->pin_out[6] = AND(gpio->output[6].q, d6);
    gpio->pin_out[7] = AND(gpio->output[7].q, d7);

    gpio->pin_read[0] = mux2(gpio->input[0], gpio->output[0].q, d0);
    gpio->pin_read[1] = mux2(gpio->input[1], gpio->output[1].q, d1);
    gpio->pin_read[2] = mux2(gpio->input[2], gpio->output[2].q, d2);
    gpio->pin_read[3] = mux2(gpio->input[3], gpio->output[3].q, d3);
    gpio->pin_read[4] = mux2(gpio->input[4], gpio->output[4].q, d4);
    gpio->pin_read[5] = mux2(gpio->input[5], gpio->output[5].q, d5);
    gpio->pin_read[6] = mux2(gpio->input[6], gpio->output[6].q, d6);
    gpio->pin_read[7] = mux2(gpio->input[7], gpio->output[7].q, d7);
}

void gpio_read_data(gpio_t *gpio, bit *out) {
    bit a0 = gpio->addr[0];
    bit a1 = gpio->addr[1];
    out[0] = mux2(mux2(gpio->output[0].q, gpio->direction[0].q, a0), AND(gpio->pin_read[0], NOT(a0)), a1);
    out[1] = mux2(mux2(gpio->output[1].q, gpio->direction[1].q, a0), AND(gpio->pin_read[1], NOT(a0)), a1);
    out[2] = mux2(mux2(gpio->output[2].q, gpio->direction[2].q, a0), AND(gpio->pin_read[2], NOT(a0)), a1);
    out[3] = mux2(mux2(gpio->output[3].q, gpio->direction[3].q, a0), AND(gpio->pin_read[3], NOT(a0)), a1);
    out[4] = mux2(mux2(gpio->output[4].q, gpio->direction[4].q, a0), AND(gpio->pin_read[4], NOT(a0)), a1);
    out[5] = mux2(mux2(gpio->output[5].q, gpio->direction[5].q, a0), AND(gpio->pin_read[5], NOT(a0)), a1);
    out[6] = mux2(mux2(gpio->output[6].q, gpio->direction[6].q, a0), AND(gpio->pin_read[6], NOT(a0)), a1);
    out[7] = mux2(mux2(gpio->output[7].q, gpio->direction[7].q, a0), AND(gpio->pin_read[7], NOT(a0)), a1);
}

void gpio_read_pins(gpio_t *gpio, bit *out) {
    out[0] = gpio->pin_out[0]; out[1] = gpio->pin_out[1];
    out[2] = gpio->pin_out[2]; out[3] = gpio->pin_out[3];
    out[4] = gpio->pin_out[4]; out[5] = gpio->pin_out[5];
    out[6] = gpio->pin_out[6]; out[7] = gpio->pin_out[7];
}
