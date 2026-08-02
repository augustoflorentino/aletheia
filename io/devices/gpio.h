#ifndef ALETHEIA_GPIO_H
#define ALETHEIA_GPIO_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define GPIO_PINS 8

typedef struct {
    d_flip_flop_t output[GPIO_PINS];
    d_flip_flop_t direction[GPIO_PINS];

    bit input[GPIO_PINS];
    bit output_data[GPIO_PINS];
    bit dir_data[GPIO_PINS];
    bit pin_out[GPIO_PINS];
    bit pin_read[GPIO_PINS];

    bit chip_select;
    bit read;
    bit write;
    bit addr[2];
} gpio_t;

void gpio_init(gpio_t *gpio);
void gpio_reset(gpio_t *gpio);
void gpio_set_cs(gpio_t *gpio, bit cs);
void gpio_set_read(gpio_t *gpio, bit rd);
void gpio_set_write(gpio_t *gpio, bit wr);
void gpio_set_addr(gpio_t *gpio, bit a0, bit a1);
void gpio_set_data(gpio_t *gpio, bit *data);
void gpio_set_pins(gpio_t *gpio, bit *pins);
void gpio_clock_rise(gpio_t *gpio);
void gpio_clock_fall(gpio_t *gpio);
void gpio_propagate(gpio_t *gpio);
void gpio_read_data(gpio_t *gpio, bit *out);
void gpio_read_pins(gpio_t *gpio, bit *out);

#endif
