#include "board.h"
#include "memsys.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/gates/xor.h"
#include "../core/gates/select.h"
#include "../core/mux/mux.h"
#include "../cpu/alu/adder.h"

static void copy_word(bit *src, bit *dst) {
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
    dst[4] = src[4]; dst[5] = src[5]; dst[6] = src[6]; dst[7] = src[7];
    dst[8] = src[8]; dst[9] = src[9]; dst[10] = src[10]; dst[11] = src[11];
    dst[12] = src[12]; dst[13] = src[13]; dst[14] = src[14]; dst[15] = src[15];
    dst[16] = src[16]; dst[17] = src[17]; dst[18] = src[18]; dst[19] = src[19];
    dst[20] = src[20]; dst[21] = src[21]; dst[22] = src[22]; dst[23] = src[23];
    dst[24] = src[24]; dst[25] = src[25]; dst[26] = src[26]; dst[27] = src[27];
    dst[28] = src[28]; dst[29] = src[29]; dst[30] = src[30]; dst[31] = src[31];
}

static void sign_extend_16(bit *imm, bit *out) {
    out[0] = imm[0]; out[1] = imm[1]; out[2] = imm[2]; out[3] = imm[3];
    out[4] = imm[4]; out[5] = imm[5]; out[6] = imm[6]; out[7] = imm[7];
    out[8] = imm[8]; out[9] = imm[9]; out[10] = imm[10]; out[11] = imm[11];
    out[12] = imm[12]; out[13] = imm[13]; out[14] = imm[14]; out[15] = imm[15];
    out[16] = imm[15]; out[17] = imm[15]; out[18] = imm[15]; out[19] = imm[15];
    out[20] = imm[15]; out[21] = imm[15]; out[22] = imm[15]; out[23] = imm[15];
    out[24] = imm[15]; out[25] = imm[15]; out[26] = imm[15]; out[27] = imm[15];
    out[28] = imm[15]; out[29] = imm[15]; out[30] = imm[15]; out[31] = imm[15];
}

static void zero_extend_26(bit *addr, bit *out) {
    out[0] = addr[0]; out[1] = addr[1]; out[2] = addr[2]; out[3] = addr[3];
    out[4] = addr[4]; out[5] = addr[5]; out[6] = addr[6]; out[7] = addr[7];
    out[8] = addr[8]; out[9] = addr[9]; out[10] = addr[10]; out[11] = addr[11];
    out[12] = addr[12]; out[13] = addr[13]; out[14] = addr[14]; out[15] = addr[15];
    out[16] = addr[16]; out[17] = addr[17]; out[18] = addr[18]; out[19] = addr[19];
    out[20] = addr[20]; out[21] = addr[21]; out[22] = addr[22]; out[23] = addr[23];
    out[24] = addr[24]; out[25] = addr[25]; out[26] = 0; out[27] = 0;
    out[28] = 0; out[29] = 0; out[30] = 0; out[31] = 0;
}

static void alu_operands(instruction_decoder_t *dec, bit *rs_val, bit *rt_val, bit *a, bit *b) {
    alu_operand_sel_t sel;
    sel.alu_src = dec->alu_src;
    sel.imm_zero_ext = dec->imm_zero_ext;
    sel.shift_rt = dec->shift_rt;
    sel.shift_shamt = dec->shift_shamt;
    sel.lui = dec->lui;
    alu_select_operands(&sel, dec->imm, dec->shamt, rs_val, rt_val, a, b);
}

void board_init(board_t *board) {
    clock_init(&board->clk);
    power_init(&board->power);
    config_init(&board->config);

    board->alu_flags.zero = 0;
    board->alu_flags.negative = 0;
    board->alu_flags.carry = 0;
    board->alu_flags.overflow = 0;
    regfile_init(&board->regs);
    pc_init(&board->pc);
    decoder_init(&board->dec);

    ram_init(&board->ram);
    rom1k_init(&board->rom);

    addr_bus_init(&board->addr_bus);
    data_bus_init(&board->data_bus);
    ctrl_bus_init(&board->ctrl_bus);

    uart_tx_init(&board->uart);
    uart_rx_init(&board->uart_rx);
    board->serial_in = 1;
    display_init(&board->disp);
    gpio_init(&board->gpio);
    timer_init(&board->timer);
    pic_init(&board->pic);
    exc_init(&board->exc);
    mmu_init(&board->mmu);
    l1_init(&board->l1);
    dff_reset(&board->mmu_en);
    dff_reset(&board->umode);
    dff_reset(&board->uprev);

    dff_reset(&board->mc_state[0]);
    dff_reset(&board->mc_state[1]);
    dff_reset(&board->mc_state[2]);
    ir_init(&board->ir);
    mar_init(&board->mar);
    mdr_init(&board->mdr);
    reg32_init(&board->reg_a);
    reg32_init(&board->reg_b);
    reg32_init(&board->alu_out);
    dma_init(&board->dma);
    l2_init(&board->l2);
    dff_reset(&board->prescale[0]); dff_reset(&board->prescale[1]); dff_reset(&board->prescale[2]);
    dram_init(&board->dram);
    board->dram_leak = 0;
    dff_reset(&board->dram_row[0]); dff_reset(&board->dram_row[1]);
    dff_reset(&board->dram_row[2]); dff_reset(&board->dram_row[3]);
    dma_set_dreq(&board->dma, 1);
    arbiter_init(&board->arb);

    board->clock_out = 0;
    board->reset_out = 1;
    board->halt = 0;
}

void board_reset(board_t *board) {
    clock_init(&board->clk);
    power_reset(&board->power);
    config_reset(&board->config);

    board->alu_flags.zero = 0;
    board->alu_flags.negative = 0;
    board->alu_flags.carry = 0;
    board->alu_flags.overflow = 0;
    regfile_reset(&board->regs);
    pc_reset(&board->pc);
    decoder_reset(&board->dec);

    ram_reset(&board->ram);

    addr_bus_reset(&board->addr_bus);
    data_bus_reset(&board->data_bus);
    ctrl_bus_reset(&board->ctrl_bus);

    uart_tx_init(&board->uart);
    uart_rx_init(&board->uart_rx);
    board->serial_in = 1;
    display_reset(&board->disp);
    gpio_reset(&board->gpio);
    timer_reset(&board->timer);
    pic_reset(&board->pic);
    exc_reset(&board->exc);
    mmu_reset(&board->mmu);
    l1_reset(&board->l1);
    l2_reset(&board->l2);
    dff_reset(&board->prescale[0]); dff_reset(&board->prescale[1]); dff_reset(&board->prescale[2]);
    dff_reset(&board->mmu_en);
    dff_reset(&board->umode);
    dff_reset(&board->uprev);

    dff_reset(&board->mc_state[0]);
    dff_reset(&board->mc_state[1]);
    dff_reset(&board->mc_state[2]);
    ir_reset(&board->ir);
    mar_reset(&board->mar);
    mdr_reset(&board->mdr);
    reg32_reset(&board->reg_a);
    reg32_reset(&board->reg_b);
    reg32_reset(&board->alu_out);
    dma_reset(&board->dma);
    dff_reset(&board->dram_row[0]); dff_reset(&board->dram_row[1]);
    dff_reset(&board->dram_row[2]); dff_reset(&board->dram_row[3]);
    dma_set_dreq(&board->dma, 1);
    arbiter_reset(&board->arb);

    board->clock_out = 0;
    board->reset_out = 1;
    board->halt = 0;
}

void board_power_on(board_t *board) {
    power_set_power(&board->power, 1);
}

void board_power_off(board_t *board) {
    power_set_power(&board->power, 0);
}

void board_propagate(board_t *board) {
    regfile_propagate(&board->regs);
    pc_propagate(&board->pc);
    ram_propagate(&board->ram);
    addr_bus_propagate(&board->addr_bus);
    data_bus_propagate(&board->data_bus);
    ctrl_bus_propagate(&board->ctrl_bus);
}

void board_step(board_t *board) {
    pc_propagate(&board->pc);

    bit instr[32];
    rom1k_fetch(&board->rom, board->pc.reg.data_out, instr);

    decoder_set_instruction(&board->dec, instr);
    decoder_propagate(&board->dec);

    bit one_w[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bit pc_plus1[32], pc_carry[33];
    adder_32(board->pc.reg.data_out, one_w, pc_plus1, pc_carry);

    bit frozen = board->halt;
    bit run = NOT(frozen);
    bit halt_now = AND(AND(board->dec.opcode[0], board->dec.opcode[1]),
                   AND(AND(board->dec.opcode[2], board->dec.opcode[3]),
                       AND(board->dec.opcode[4], board->dec.opcode[5])));

    regfile_set_read_addr1(&board->regs, board->dec.rs);
    regfile_set_read_addr2(&board->regs, board->dec.rt);
    regfile_propagate(&board->regs);
    bit rdata1[32], rdata2[32];
    copy_word(board->regs.read_data1, rdata1);
    copy_word(board->regs.read_data2, rdata2);

    bit imm_ext[32];
    sign_extend_16(board->dec.imm, imm_ext);

    bit alu_a[32], alu_b[32];
    alu_operands(&board->dec, rdata1, rdata2, alu_a, alu_b);

    bit alu_result[32];
    alu_execute_ext(alu_a, alu_b, board->dec.alu_op, alu_result, &board->alu_flags);

    memsys_irq(board);
    mem_port_t port;
    memsys_probe(board, alu_result, rdata2, board->dec.mem_read, board->dec.mem_write, &port);

    exc_outcome_t exc;
    board_exceptions(board, board->pc.reg.data_out, port.fault, board->dec.undefined, 1,
                     board->dec.reti, run, &exc);
    bit take_exc = exc.take;
    bit ok = exc.ok;
    memsys_commit(board, &port, ok);

    bit *mem_data = port.rdata;

    bit wb_base[32];
    mux2_32(alu_result, mem_data, board->dec.mem_to_reg, wb_base);

    bit wb[32];
    mux2_32(wb_base, pc_plus1, board->dec.link, wb);

    bit wreg[5];
    wreg[0] = mux2(mux2(board->dec.rt[0], board->dec.rd[0], board->dec.reg_dst), 1, board->dec.jal);
    wreg[1] = mux2(mux2(board->dec.rt[1], board->dec.rd[1], board->dec.reg_dst), 1, board->dec.jal);
    wreg[2] = mux2(mux2(board->dec.rt[2], board->dec.rd[2], board->dec.reg_dst), 1, board->dec.jal);
    wreg[3] = mux2(mux2(board->dec.rt[3], board->dec.rd[3], board->dec.reg_dst), 1, board->dec.jal);
    wreg[4] = mux2(mux2(board->dec.rt[4], board->dec.rd[4], board->dec.reg_dst), 1, board->dec.jal);

    regfile_set_write_addr(&board->regs, wreg);
    regfile_set_write_data(&board->regs, wb);
    regfile_set_write_enable(&board->regs, AND(board->dec.reg_write, ok));

    bit branch_target[32], c2[33];
    adder_32(pc_plus1, imm_ext, branch_target, c2);

    bit jump_target[32];
    zero_extend_26(board->dec.addr, jump_target);

    bit take_branch = AND(board->dec.branch, XOR(board->alu_flags.zero, board->dec.branch_ne));
    bit next_pc[32];
    mux2_32(pc_plus1, branch_target, take_branch, next_pc);
    mux2_32(next_pc, jump_target, board->dec.jump, next_pc);

    mux2_32(next_pc, rdata1, board->dec.jr, next_pc);

    mux2_32(next_pc, board->exc.return_addr, board->dec.reti, next_pc);

    mux2_32(next_pc, board->exc.handler_addr, take_exc, next_pc);

    pc_set_load_data(&board->pc, next_pc);
    pc_set_load(&board->pc, run);
    pc_set_increment(&board->pc, 0);

    board->halt = OR(frozen, AND(halt_now, NOT(take_exc)));

    regfile_clock_fall(&board->regs);
    pc_clock_fall(&board->pc);
    memsys_clock_fall(board);
    regfile_clock_rise(&board->regs);
    pc_clock_rise(&board->pc);
    memsys_clock_rise(board);
}

bit board_is_running(board_t *board) {
    bit ready = power_is_ready(&board->power);
    bit not_halt = NOT(board->halt);
    return AND(ready, not_halt);
}

bit board_get_clock(board_t *board) {
    return board->clock_out;
}

bit board_get_reset(board_t *board) {
    return board->reset_out;
}

void board_step_mc(board_t *board) {
    bit q0 = board->mc_state[0].q;
    bit q1 = board->mc_state[1].q;
    bit q2 = board->mc_state[2].q;
    bit n0 = NOT(q0), n1 = NOT(q1), n2 = NOT(q2);
    bit s0 = AND(n2, AND(n1, n0));
    bit s1 = AND(n2, AND(n1, q0));
    bit s2 = AND(n2, AND(q1, n0));
    bit s3 = AND(n2, AND(q1, q0));
    bit s4 = AND(q2, AND(n1, n0));

    pc_propagate(&board->pc);

    bit instr[32];
    rom1k_fetch(&board->rom, board->pc.reg.data_out, instr);

    bit one_w[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bit pc_plus1[32], pc_carry[33];
    adder_32(board->pc.reg.data_out, one_w, pc_plus1, pc_carry);

    ir_propagate(&board->ir);
    bit ir_word[32];
    copy_word(board->ir.reg.data_out, ir_word);
    decoder_set_instruction(&board->dec, ir_word);
    decoder_propagate(&board->dec);

    bit frozen = board->halt;
    bit run = NOT(frozen);
    bit is_halt = AND(s1, AND(AND(board->dec.opcode[0], board->dec.opcode[1]),
                       AND(AND(board->dec.opcode[2], board->dec.opcode[3]),
                           AND(board->dec.opcode[4], board->dec.opcode[5]))));

    regfile_set_read_addr1(&board->regs, board->dec.rs);
    regfile_set_read_addr2(&board->regs, board->dec.rt);
    regfile_propagate(&board->regs);

    reg32_propagate(&board->reg_a);
    reg32_propagate(&board->reg_b);
    reg32_propagate(&board->alu_out);
    mar_propagate(&board->mar);
    mdr_propagate(&board->mdr);

    bit imm_ext[32];
    sign_extend_16(board->dec.imm, imm_ext);

    bit opA[32], opB[32];
    alu_operands(&board->dec, board->reg_a.data_out, board->reg_b.data_out, opA, opB);
    bit srcA[32], srcB[32];
    mux2_32(opA, board->pc.reg.data_out, s1, srcA);
    mux2_32(opB, imm_ext, s1, srcB);

    bit not_s1 = NOT(s1);
    bit alu_ctrl[4];
    alu_ctrl[0] = AND(board->dec.alu_op[0], not_s1);
    alu_ctrl[1] = AND(board->dec.alu_op[1], not_s1);
    alu_ctrl[2] = AND(board->dec.alu_op[2], not_s1);
    alu_ctrl[3] = AND(board->dec.alu_op[3], not_s1);

    bit alu_result[32];
    alu_execute_ext(srcA, srcB, alu_ctrl, alu_result, &board->alu_flags);

    bit mem_op = OR(board->dec.mem_read, board->dec.mem_write);

    addr_bus_propagate(&board->addr_bus);
    data_bus_propagate(&board->data_bus);
    ctrl_bus_propagate(&board->ctrl_bus);
    bit bus_wdata[32];
    data_bus_read(&board->data_bus, bus_wdata);

    memsys_irq(board);
    mem_port_t port;
    memsys_probe(board, board->mar.reg.data_out, bus_wdata,
                 AND(s3, board->dec.mem_read), AND(s3, board->dec.mem_write), &port);

    bit all_ones[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    bit pc_minus1[32], pm_carry[33], epc[32];
    adder_32(board->pc.reg.data_out, all_ones, pc_minus1, pm_carry);
    mux2_32(board->pc.reg.data_out, pc_minus1, NOT(s0), epc);

    exc_outcome_t exc;
    board_exceptions(board, epc, port.fault, AND(s1, board->dec.undefined), s0,
                     AND(s2, board->dec.reti), run, &exc);
    bit take_exc = exc.take;
    bit ok = exc.ok;
    memsys_commit(board, &port, ok);

    addr_bus_set_addr(&board->addr_bus, alu_result);
    addr_bus_set_drive(&board->addr_bus, AND(AND(s2, mem_op), run));
    data_bus_set_data(&board->data_bus, board->reg_b.data_out);
    data_bus_set_drive(&board->data_bus, AND(AND(s2, board->dec.mem_write), run));
    data_bus_set_rw(&board->data_bus, board->dec.mem_write);
    ctrl_bus_set_mreq(&board->ctrl_bus, AND(OR(s2, s3), mem_op));
    ctrl_bus_set_read(&board->ctrl_bus, AND(s3, board->dec.mem_read));
    ctrl_bus_set_write(&board->ctrl_bus, AND(s3, board->dec.mem_write));
    ctrl_bus_set_drive(&board->ctrl_bus, 1);

    bit wb_a[32];
    mux2_32(board->alu_out.data_out, board->mdr.reg.data_out, board->dec.mem_to_reg, wb_a);
    bit wb[32];
    mux2_32(wb_a, board->pc.reg.data_out, board->dec.link, wb);

    bit wreg[5];
    wreg[0] = mux2(mux2(board->dec.rt[0], board->dec.rd[0], board->dec.reg_dst), 1, board->dec.jal);
    wreg[1] = mux2(mux2(board->dec.rt[1], board->dec.rd[1], board->dec.reg_dst), 1, board->dec.jal);
    wreg[2] = mux2(mux2(board->dec.rt[2], board->dec.rd[2], board->dec.reg_dst), 1, board->dec.jal);
    wreg[3] = mux2(mux2(board->dec.rt[3], board->dec.rd[3], board->dec.reg_dst), 1, board->dec.jal);
    wreg[4] = mux2(mux2(board->dec.rt[4], board->dec.rd[4], board->dec.reg_dst), 1, board->dec.jal);

    bit wr_s4 = AND(s4, board->dec.reg_write);
    bit wr_jal = AND(s2, board->dec.link);
    regfile_set_write_addr(&board->regs, wreg);
    regfile_set_write_data(&board->regs, wb);
    regfile_set_write_enable(&board->regs, AND(OR(wr_s4, wr_jal), ok));

    bit jump_target[32];
    zero_extend_26(board->dec.addr, jump_target);
    bit take_branch = AND(board->dec.branch, XOR(board->alu_flags.zero, board->dec.branch_ne));

    bit npc[32];
    copy_word(pc_plus1, npc);
    mux2_32(npc, board->alu_out.data_out, AND(s2, take_branch), npc);
    mux2_32(npc, jump_target, AND(s2, board->dec.jump), npc);
    mux2_32(npc, board->reg_a.data_out, AND(s2, board->dec.jr), npc);
    mux2_32(npc, board->exc.return_addr, exc.reti, npc);
    mux2_32(npc, board->exc.handler_addr, take_exc, npc);

    bit redirect = AND(s2, OR(OR(take_branch, board->dec.reti),
                              OR(board->dec.jump, board->dec.jr)));
    bit pc_load = OR(AND(OR(s0, redirect), ok), take_exc);
    pc_set_load_data(&board->pc, npc);
    pc_set_load(&board->pc, pc_load);
    pc_set_increment(&board->pc, 0);

    ir_set_input(&board->ir, instr);
    ir_set_load(&board->ir, AND(s0, ok));

    reg32_set_input(&board->reg_a, board->regs.read_data1);
    reg32_set_load(&board->reg_a, AND(s1, ok));
    reg32_set_input(&board->reg_b, board->regs.read_data2);
    reg32_set_load(&board->reg_b, AND(s1, ok));

    reg32_set_input(&board->alu_out, alu_result);
    reg32_set_load(&board->alu_out, AND(OR(s1, s2), ok));

    mar_set_input(&board->mar, alu_result);
    mar_set_load(&board->mar, AND(AND(s2, OR(board->dec.mem_read,
                                             board->dec.mem_write)), run));

    mdr_set_input(&board->mdr, port.rdata);
    mdr_set_load(&board->mdr, AND(AND(s3, board->dec.mem_read), ok));

    bit alu_wb = AND(board->dec.reg_write,
                     AND(NOT(board->dec.mem_to_reg), NOT(board->dec.link)));
    bit to_s1 = s0;
    bit to_s2 = s1;
    bit to_s3 = AND(s2, OR(board->dec.mem_read, board->dec.mem_write));
    bit to_s4 = OR(AND(s2, alu_wb), AND(s3, board->dec.mem_read));
    bit ns0 = OR(to_s1, to_s3);
    bit ns1 = OR(to_s2, to_s3);
    bit ns2 = to_s4;
    board->mc_state[0].d = AND(mux2(q0, ns0, run), NOT(take_exc));
    board->mc_state[1].d = AND(mux2(q1, ns1, run), NOT(take_exc));
    board->mc_state[2].d = AND(mux2(q2, ns2, run), NOT(take_exc));

    board->halt = OR(frozen, AND(is_halt, NOT(take_exc)));

    ir_clock_fall(&board->ir);
    mar_clock_fall(&board->mar);
    mdr_clock_fall(&board->mdr);
    reg32_clock_fall(&board->reg_a);
    reg32_clock_fall(&board->reg_b);
    reg32_clock_fall(&board->alu_out);
    regfile_clock_fall(&board->regs);
    pc_clock_fall(&board->pc);
    memsys_clock_fall(board);
    addr_bus_clock_fall(&board->addr_bus);
    data_bus_clock_fall(&board->data_bus);
    ctrl_bus_clock_fall(&board->ctrl_bus);
    dff_clock_fall(&board->mc_state[0]);
    dff_clock_fall(&board->mc_state[1]);
    dff_clock_fall(&board->mc_state[2]);

    ir_clock_rise(&board->ir);
    mar_clock_rise(&board->mar);
    mdr_clock_rise(&board->mdr);
    reg32_clock_rise(&board->reg_a);
    reg32_clock_rise(&board->reg_b);
    reg32_clock_rise(&board->alu_out);
    regfile_clock_rise(&board->regs);
    pc_clock_rise(&board->pc);
    memsys_clock_rise(board);
    addr_bus_clock_rise(&board->addr_bus);
    data_bus_clock_rise(&board->data_bus);
    ctrl_bus_clock_rise(&board->ctrl_bus);
    dff_clock_rise(&board->mc_state[0]);
    dff_clock_rise(&board->mc_state[1]);
    dff_clock_rise(&board->mc_state[2]);
}

void board_dma_step(board_t *board) {
    memsys_bus_hold(board);
    memsys_dma_cycle(board);
}

static void board_hold_reset(board_t *board) {
    (void)board;
}

void board_clock(board_t *board, void (*core_step)(board_t *)) {
    void (*const domain[2][2])(board_t *) = {{board_hold_reset, board_hold_reset}, {core_step, board_dma_step}};
    power_propagate(&board->power);
    bit ready = power_is_ready(&board->power);
    domain[wire_select(ready)][wire_select(memsys_bus_hold(board))](board);
    power_clock_fall(&board->power);
    power_clock_rise(&board->power);
    power_propagate(&board->power);
}
