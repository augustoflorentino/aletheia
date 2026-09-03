#ifndef ALETHEIA_BOARD_H
#define ALETHEIA_BOARD_H

#include "../core/types.h"
#include "../core/clock/clock.h"
#include "power.h"
#include "config.h"
#include "../cpu/registers/register_file.h"
#include "../cpu/registers/pc.h"
#include "../cpu/registers/ir.h"
#include "../cpu/registers/mar.h"
#include "../cpu/registers/mdr.h"
#include "../cpu/pipeline/pipeline_reg.h"
#include "../cpu/pipeline/hazard_unit.h"
#include "../cpu/pipeline/forwarding.h"
#include "../cpu/control/decoder.h"
#include "../cpu/alu/alu.h"
#include "../memory/sram256.h"
#include "../memory/rom1k.h"
#include "ram.h"
#include "../memory/dram.h"
#include "../memory/cache/l2_cache.h"
#include "../memory/cache/l1_cache.h"
#include "../memory/mmu/mmu.h"
#include "../cpu/interrupt/exception_handler.h"
#include "../bus/buses.h"
#include "../io/devices/uart.h"
#include "../io/devices/display.h"
#include "../io/devices/gpio.h"
#include "../io/devices/timer.h"
#include "../io/devices/pic.h"
#include "../io/dma.h"
#include "../bus/arbiter.h"

typedef struct {
    hw_clock_t clk;
    power_controller_t power;
    system_config_t config;

    alu_flags_t alu_flags;
    register_file_t regs;
    program_counter_t pc;
    instruction_decoder_t dec;

    ram_t ram;
    dram_16x32_t dram;
    d_flip_flop_t dram_row[4];
    bit dram_leak;
    rom_1kx32_t rom;

    address_bus_t addr_bus;
    data_bus_t data_bus;
    control_bus_t ctrl_bus;


    uart_tx_t uart;
    uart_rx_t uart_rx;
    bit serial_in;
    display_t disp;
    gpio_t gpio;

    hw_timer_t timer;
    pic_t pic;
    exception_handler_t exc;
    mmu_t mmu;
    l1_cache_t l1;
    l2_cache_t l2;
    d_flip_flop_t prescale[3];
    d_flip_flop_t mmu_en;

    d_flip_flop_t mc_state[3];
    instruction_register_t ir;
    memory_address_register_t mar;
    memory_data_register_t mdr;
    register_32_t reg_a;
    register_32_t reg_b;
    register_32_t alu_out;

    dma_controller_t dma;
    arbiter_t arb;

    if_id_reg_t if_id;
    id_ex_reg_t id_ex;
    d_flip_flop_t pipe_halt_seen;
    d_flip_flop_t pipe_halt_drain;
    ex_mem_reg_t ex_mem;
    mem_wb_reg_t mem_wb;
    forwarding_unit_t fwd;
    hazard_unit_t haz;

    bit clock_out;
    bit reset_out;
    bit halt;
} board_t;

void board_init(board_t *board);
void board_reset(board_t *board);
void board_power_on(board_t *board);
void board_power_off(board_t *board);
void board_propagate(board_t *board);
void board_step(board_t *board);

void board_step_mc(board_t *board);

void board_dma_step(board_t *board);
void board_clock(board_t *board, void (*core_step)(board_t *));

void board_pipe_init(board_t *board);

void board_step_pipe(board_t *board);
bit board_is_running(board_t *board);
bit board_get_clock(board_t *board);
bit board_get_reset(board_t *board);

#endif
