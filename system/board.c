#include "board.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
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
    control_init(&board->ctrl);
    decoder_init(&board->dec);

    sram256_init(&board->ram);
    rom256_init(&board->rom);

    addr_bus_init(&board->addr_bus);
    data_bus_init(&board->data_bus);
    ctrl_bus_init(&board->ctrl_bus);

    io_ctrl_init(&board->io_ctrl);
    uart_tx_init(&board->uart);
    display_init(&board->disp);
    gpio_init(&board->gpio);
    timer_init(&board->timer);
    pic_init(&board->pic);
    exc_init(&board->exc);
    mmu_init(&board->mmu);
    l1_init(&board->l1);
    dff_reset(&board->mmu_en);

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
    control_reset(&board->ctrl);
    decoder_reset(&board->dec);

    sram256_reset(&board->ram);
    rom256_reset(&board->rom);

    addr_bus_reset(&board->addr_bus);
    data_bus_reset(&board->data_bus);
    ctrl_bus_reset(&board->ctrl_bus);

    io_ctrl_reset(&board->io_ctrl);
    uart_tx_init(&board->uart);
    display_reset(&board->disp);
    gpio_reset(&board->gpio);
    timer_reset(&board->timer);
    pic_reset(&board->pic);
    exc_reset(&board->exc);
    mmu_reset(&board->mmu);
    l1_reset(&board->l1);
    dff_reset(&board->mmu_en);

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
    control_propagate(&board->ctrl);
    sram256_propagate(&board->ram);
    rom256_propagate(&board->rom);
    addr_bus_propagate(&board->addr_bus);
    data_bus_propagate(&board->data_bus);
    ctrl_bus_propagate(&board->ctrl_bus);
    io_ctrl_propagate(&board->io_ctrl);
}

void board_step(board_t *board) {
    pc_propagate(&board->pc);

    bit pc_addr[8];
    pc_addr[0] = board->pc.reg.data_out[0];
    pc_addr[1] = board->pc.reg.data_out[1];
    pc_addr[2] = board->pc.reg.data_out[2];
    pc_addr[3] = board->pc.reg.data_out[3];
    pc_addr[4] = board->pc.reg.data_out[4];
    pc_addr[5] = board->pc.reg.data_out[5];
    pc_addr[6] = board->pc.reg.data_out[6];
    pc_addr[7] = board->pc.reg.data_out[7];

    rom256_set_addr(&board->rom, pc_addr);
    rom256_set_cs(&board->rom, 1);
    rom256_propagate(&board->rom);
    bit instr[32];
    rom256_read(&board->rom, instr);

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

    bit alu_b[32];
    mux2_32(rdata2, imm_ext, board->dec.alu_src, alu_b);

    bit alu_ctrl[3];
    bit base0 = OR(board->dec.alu_op[0], AND(board->dec.alu_op[2], board->dec.alu_op[1]));
    bit base1 = board->dec.alu_op[1];
    alu_ctrl[0] = mux2(base0, 1, board->dec.branch);
    alu_ctrl[1] = mux2(base1, 0, board->dec.branch);
    alu_ctrl[2] = 0;

    bit alu_result[32];
    alu_execute(rdata1, alu_b, alu_ctrl, alu_result, &board->alu_flags);

    timer_propagate(&board->timer);
    bit irq_lines[8];
    irq_lines[0] = timer_get_irq(&board->timer);
    irq_lines[1] = 0; irq_lines[2] = 0; irq_lines[3] = 0;
    irq_lines[4] = 0; irq_lines[5] = 0; irq_lines[6] = 0; irq_lines[7] = 0;
    pic_raise(&board->pic, irq_lines);
    intc_set_global_enable(&board->pic.ic, 1);
    pic_propagate(&board->pic);

    bit vpn_zero = AND(AND(NOT(alu_result[12]), NOT(alu_result[13])),
                       AND(NOT(alu_result[14]), NOT(alu_result[15])));
    bit io_sel = AND(alu_result[8], vpn_zero);
    bit d0 = alu_result[4];
    bit d1 = alu_result[5];
    bit is_uart = AND(NOT(d1), NOT(d0));
    bit is_disp = AND(NOT(d1), d0);
    bit is_gpio = AND(d1, NOT(d0));
    bit is_sys = AND(d1, d0);
    bit sys_io = AND(io_sel, is_sys);

    bit a0 = alu_result[0];
    bit a1 = alu_result[1];
    bit a2 = alu_result[2];
    bit sel_timer = AND(sys_io, NOT(a2));
    bit sel_picmask = AND(sys_io, AND(a2, AND(NOT(a1), NOT(a0))));
    bit sel_ptfill = AND(sys_io, AND(a2, AND(NOT(a1), a0)));
    bit sel_mmuen = AND(sys_io, AND(a2, AND(a1, NOT(a0))));

    bit mem_op = OR(board->dec.mem_read, board->dec.mem_write);
    bit mem_access = AND(mem_op, NOT(io_sel));
    bit translate = AND(board->mmu_en.q, mem_access);

    bit ptfill_req = AND(board->dec.mem_write, sel_ptfill);
    bit fill_vaddr[32];
    fill_vaddr[0] = 0; fill_vaddr[1] = 0; fill_vaddr[2] = 0; fill_vaddr[3] = 0;
    fill_vaddr[4] = 0; fill_vaddr[5] = 0; fill_vaddr[6] = 0; fill_vaddr[7] = 0;
    fill_vaddr[8] = 0; fill_vaddr[9] = 0; fill_vaddr[10] = 0; fill_vaddr[11] = 0;
    fill_vaddr[12] = rdata2[20];
    fill_vaddr[13] = rdata2[21];
    fill_vaddr[14] = rdata2[22];
    fill_vaddr[15] = rdata2[23];
    fill_vaddr[16] = 0; fill_vaddr[17] = 0; fill_vaddr[18] = 0; fill_vaddr[19] = 0;
    fill_vaddr[20] = 0; fill_vaddr[21] = 0; fill_vaddr[22] = 0; fill_vaddr[23] = 0;
    fill_vaddr[24] = 0; fill_vaddr[25] = 0; fill_vaddr[26] = 0; fill_vaddr[27] = 0;
    fill_vaddr[28] = 0; fill_vaddr[29] = 0; fill_vaddr[30] = 0; fill_vaddr[31] = 0;

    bit mmu_vaddr[32];
    mux2_32(alu_result, fill_vaddr, ptfill_req, mmu_vaddr);
    mmu_set_vaddr(&board->mmu, mmu_vaddr);
    mmu_propagate(&board->mmu);

    bit fault_v = AND(translate, board->mmu.fault);

    bit fault = AND(fault_v, run);
    bit irq_req = AND(board->pic.int_request, AND(run, NOT(fault)));
    exc_set_addr_error(&board->exc, fault);
    exc_set_interrupt(&board->exc, irq_req, board->pic.int_vector);
    exc_set_pc(&board->exc, board->pc.reg.data_out);
    exc_propagate(&board->exc);
    bit take_exc = AND(exc_get_exception_taken(&board->exc), run);
    bit ok = AND(run, NOT(take_exc));

    bit irq_take = AND(take_exc, irq_req);
    intc_set_service(&board->pic.ic, irq_take, board->pic.int_vector);

    bit reti_commit = AND(board->dec.reti, ok);
    exc_set_return(&board->exc, reti_commit);

    bit s0q = board->pic.ic.in_service[0].q;
    bit s1q = board->pic.ic.in_service[1].q;
    bit s2q = board->pic.ic.in_service[2].q;
    bit s3q = board->pic.ic.in_service[3].q;
    bit s4q = board->pic.ic.in_service[4].q;
    bit s5q = board->pic.ic.in_service[5].q;
    bit s6q = board->pic.ic.in_service[6].q;
    bit s7q = board->pic.ic.in_service[7].q;
    bit svc_any = OR(OR(OR(s0q, s1q), OR(s2q, s3q)),
                     OR(OR(s4q, s5q), OR(s6q, s7q)));
    bit ack_num[3];
    ack_num[0] = OR(OR(s1q, s3q), OR(s5q, s7q));
    ack_num[1] = OR(OR(s2q, s3q), OR(s6q, s7q));
    ack_num[2] = OR(OR(s4q, s5q), OR(s6q, s7q));
    intc_set_ack(&board->pic.ic, AND(reti_commit, svc_any), ack_num);

    bit paddr[32];
    mux2_32(alu_result, board->mmu.paddr, translate, paddr);

    bit store_en = AND(board->dec.mem_write, ok);
    bit load_en = AND(board->dec.mem_read, ok);

    bit sram_we = AND(store_en, AND(NOT(io_sel), NOT(fault_v)));
    bit uart_we = AND(store_en, AND(io_sel, is_uart));
    bit disp_we = AND(store_en, AND(io_sel, is_disp));
    bit gpio_we = AND(store_en, AND(io_sel, is_gpio));

    bit mem_addr[8];
    mem_addr[0] = paddr[0];
    mem_addr[1] = paddr[1];
    mem_addr[2] = paddr[2];
    mem_addr[3] = paddr[3];
    mem_addr[4] = paddr[4];
    mem_addr[5] = paddr[5];
    mem_addr[6] = paddr[6];
    mem_addr[7] = paddr[7];
    sram256_set_addr(&board->ram, mem_addr);
    sram256_set_data(&board->ram, rdata2);
    sram256_set_write(&board->ram, sram_we);
    sram256_set_cs(&board->ram, 1);
    sram256_propagate(&board->ram);

    bit sram_data[32];
    sram256_read(&board->ram, sram_data);

    l1_set_addr(&board->l1, paddr);
    l1_propagate(&board->l1);
    bit l1_hit = board->l1.hit;
    bit cached_read[32];
    mux2_32(sram_data, board->l1.data_out, l1_hit, cached_read);

    bit l1_fill = AND(load_en, NOT(l1_hit));
    bit l1_we = AND(AND(NOT(io_sel), NOT(fault_v)), OR(sram_we, l1_fill));
    bit l1_din[32];
    mux2_32(sram_data, rdata2, board->dec.mem_write, l1_din);
    l1_set_data(&board->l1, l1_din);
    l1_set_write(&board->l1, l1_we);

    uart_tx_set_data(&board->uart, rdata2);
    uart_tx_set_load(&board->uart, uart_we);

    bit disp_addr[3];
    disp_addr[0] = alu_result[0];
    disp_addr[1] = alu_result[1];
    disp_addr[2] = alu_result[2];
    display_set_addr(&board->disp, disp_addr);
    display_set_data(&board->disp, rdata2);
    display_set_write(&board->disp, disp_we);

    gpio_set_cs(&board->gpio, AND(io_sel, is_gpio));
    gpio_set_write(&board->gpio, gpio_we);
    gpio_set_addr(&board->gpio, alu_result[2], alu_result[3]);
    gpio_set_data(&board->gpio, rdata2);

    timer_set_cs(&board->timer, sel_timer);
    timer_set_addr(&board->timer, a0, a1);
    timer_set_write(&board->timer, AND(store_en, sel_timer));
    timer_set_read(&board->timer, AND(load_en, sel_timer));
    timer_set_data(&board->timer, rdata2);

    bit picmask_we = AND(store_en, sel_picmask);
    bit mask_next[8];
    mask_next[0] = mux2(board->pic.ic.mask[0].q, rdata2[0], picmask_we);
    mask_next[1] = mux2(board->pic.ic.mask[1].q, rdata2[1], picmask_we);
    mask_next[2] = mux2(board->pic.ic.mask[2].q, rdata2[2], picmask_we);
    mask_next[3] = mux2(board->pic.ic.mask[3].q, rdata2[3], picmask_we);
    mask_next[4] = mux2(board->pic.ic.mask[4].q, rdata2[4], picmask_we);
    mask_next[5] = mux2(board->pic.ic.mask[5].q, rdata2[5], picmask_we);
    mask_next[6] = mux2(board->pic.ic.mask[6].q, rdata2[6], picmask_we);
    mask_next[7] = mux2(board->pic.ic.mask[7].q, rdata2[7], picmask_we);
    intc_set_mask(&board->pic.ic, mask_next);

    bit mmuen_we = AND(store_en, sel_mmuen);
    board->mmu_en.d = mux2(board->mmu_en.q, rdata2[0], mmuen_we);

    mmu_set_pt_pfn(&board->mmu, rdata2);
    mmu_set_pt_fill(&board->mmu, AND(store_en, sel_ptfill));

    bit tlb_fill = AND(translate, AND(NOT(board->mmu.tlb_hit),
                                      board->mmu.pt.present_out));
    mmu_set_fill_pfn(&board->mmu, board->mmu.pt.pfn_out);
    mmu_set_fill(&board->mmu, tlb_fill);

    uart_tx_propagate(&board->uart);
    bit uart_word[32];
    uart_word[0] = board->uart.ready;
    uart_word[1] = 0; uart_word[2] = 0; uart_word[3] = 0; uart_word[4] = 0;
    uart_word[5] = 0; uart_word[6] = 0; uart_word[7] = 0; uart_word[8] = 0;
    uart_word[9] = 0; uart_word[10] = 0; uart_word[11] = 0; uart_word[12] = 0;
    uart_word[13] = 0; uart_word[14] = 0; uart_word[15] = 0; uart_word[16] = 0;
    uart_word[17] = 0; uart_word[18] = 0; uart_word[19] = 0; uart_word[20] = 0;
    uart_word[21] = 0; uart_word[22] = 0; uart_word[23] = 0; uart_word[24] = 0;
    uart_word[25] = 0; uart_word[26] = 0; uart_word[27] = 0; uart_word[28] = 0;
    uart_word[29] = 0; uart_word[30] = 0; uart_word[31] = 0;

    bit tcount[16];
    timer_read_data(&board->timer, tcount);
    bit sys_status = AND(a1, a0);
    bit not_status = NOT(sys_status);
    bit sys_word[32];
    sys_word[0] = mux2(tcount[0], timer_get_irq(&board->timer), sys_status);
    sys_word[1] = AND(tcount[1], not_status);
    sys_word[2] = AND(tcount[2], not_status);
    sys_word[3] = AND(tcount[3], not_status);
    sys_word[4] = AND(tcount[4], not_status);
    sys_word[5] = AND(tcount[5], not_status);
    sys_word[6] = AND(tcount[6], not_status);
    sys_word[7] = AND(tcount[7], not_status);
    sys_word[8] = AND(tcount[8], not_status);
    sys_word[9] = AND(tcount[9], not_status);
    sys_word[10] = AND(tcount[10], not_status);
    sys_word[11] = AND(tcount[11], not_status);
    sys_word[12] = AND(tcount[12], not_status);
    sys_word[13] = AND(tcount[13], not_status);
    sys_word[14] = AND(tcount[14], not_status);
    sys_word[15] = AND(tcount[15], not_status);
    sys_word[16] = 0; sys_word[17] = 0; sys_word[18] = 0; sys_word[19] = 0;
    sys_word[20] = 0; sys_word[21] = 0; sys_word[22] = 0; sys_word[23] = 0;
    sys_word[24] = 0; sys_word[25] = 0; sys_word[26] = 0; sys_word[27] = 0;
    sys_word[28] = 0; sys_word[29] = 0; sys_word[30] = 0; sys_word[31] = 0;

    bit io_data[32];
    mux2_32(uart_word, sys_word, is_sys, io_data);

    bit mem_data[32];
    mux2_32(cached_read, io_data, io_sel, mem_data);

    bit wb_base[32];
    mux2_32(alu_result, mem_data, board->dec.mem_to_reg, wb_base);

    bit wb[32];
    mux2_32(wb_base, pc_plus1, board->dec.jal, wb);

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

    bit take_branch = AND(board->dec.branch, board->alu_flags.zero);
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
    sram256_clock_fall(&board->ram);
    uart_tx_clock_fall(&board->uart);
    display_clock_fall(&board->disp);
    gpio_clock_fall(&board->gpio);
    timer_clock_fall(&board->timer);
    pic_clock_fall(&board->pic);
    exc_clock_fall(&board->exc);
    mmu_clock_fall(&board->mmu);
    l1_clock_fall(&board->l1);
    dff_clock_fall(&board->mmu_en);
    regfile_clock_rise(&board->regs);
    pc_clock_rise(&board->pc);
    sram256_clock_rise(&board->ram);
    uart_tx_clock_rise(&board->uart);
    display_clock_rise(&board->disp);
    gpio_clock_rise(&board->gpio);
    timer_clock_rise(&board->timer);
    pic_clock_rise(&board->pic);
    exc_clock_rise(&board->exc);
    mmu_clock_rise(&board->mmu);
    l1_clock_rise(&board->l1);
    dff_clock_rise(&board->mmu_en);
    uart_tx_propagate(&board->uart);
    display_propagate(&board->disp);
    gpio_propagate(&board->gpio);
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

    bit pc_addr[8];
    pc_addr[0] = board->pc.reg.data_out[0];
    pc_addr[1] = board->pc.reg.data_out[1];
    pc_addr[2] = board->pc.reg.data_out[2];
    pc_addr[3] = board->pc.reg.data_out[3];
    pc_addr[4] = board->pc.reg.data_out[4];
    pc_addr[5] = board->pc.reg.data_out[5];
    pc_addr[6] = board->pc.reg.data_out[6];
    pc_addr[7] = board->pc.reg.data_out[7];
    rom256_set_addr(&board->rom, pc_addr);
    rom256_set_cs(&board->rom, 1);
    rom256_propagate(&board->rom);
    bit instr[32];
    rom256_read(&board->rom, instr);

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
    bit halt_now = AND(s1, AND(AND(board->dec.opcode[0], board->dec.opcode[1]),
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

    bit srcA[32];
    mux2_32(board->reg_a.data_out, board->pc.reg.data_out, s1, srcA);
    bit srcB_base[32];
    mux2_32(board->reg_b.data_out, imm_ext, board->dec.alu_src, srcB_base);
    bit srcB[32];
    mux2_32(srcB_base, imm_ext, s1, srcB);

    bit base0 = OR(board->dec.alu_op[0], AND(board->dec.alu_op[2], board->dec.alu_op[1]));
    bit base1 = board->dec.alu_op[1];
    bit c0_s2 = mux2(base0, 1, board->dec.branch);
    bit c1_s2 = mux2(base1, 0, board->dec.branch);
    bit alu_ctrl[3];
    alu_ctrl[0] = mux2(c0_s2, 0, s1);
    alu_ctrl[1] = mux2(c1_s2, 0, s1);
    alu_ctrl[2] = 0;

    bit alu_result[32];
    alu_execute(srcA, srcB, alu_ctrl, alu_result, &board->alu_flags);

    bit mem_op = OR(board->dec.mem_read, board->dec.mem_write);

    addr_bus_propagate(&board->addr_bus);
    data_bus_propagate(&board->data_bus);
    ctrl_bus_propagate(&board->ctrl_bus);

    bit bus_addr[32];
    addr_bus_read(&board->addr_bus, bus_addr);
    bit bus_wdata[32];
    data_bus_read(&board->data_bus, bus_wdata);

    bit mem_addr[8];
    mem_addr[0] = bus_addr[0];
    mem_addr[1] = bus_addr[1];
    mem_addr[2] = bus_addr[2];
    mem_addr[3] = bus_addr[3];
    mem_addr[4] = bus_addr[4];
    mem_addr[5] = bus_addr[5];
    mem_addr[6] = bus_addr[6];
    mem_addr[7] = bus_addr[7];
    sram256_set_addr(&board->ram, mem_addr);
    sram256_set_data(&board->ram, bus_wdata);
    sram256_set_write(&board->ram, AND(AND(s3, board->dec.mem_write), run));
    sram256_set_cs(&board->ram, 1);
    sram256_propagate(&board->ram);
    bit sram_data[32];
    sram256_read(&board->ram, sram_data);

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
    mux2_32(wb_a, board->pc.reg.data_out, board->dec.jal, wb);

    bit wreg[5];
    wreg[0] = mux2(mux2(board->dec.rt[0], board->dec.rd[0], board->dec.reg_dst), 1, board->dec.jal);
    wreg[1] = mux2(mux2(board->dec.rt[1], board->dec.rd[1], board->dec.reg_dst), 1, board->dec.jal);
    wreg[2] = mux2(mux2(board->dec.rt[2], board->dec.rd[2], board->dec.reg_dst), 1, board->dec.jal);
    wreg[3] = mux2(mux2(board->dec.rt[3], board->dec.rd[3], board->dec.reg_dst), 1, board->dec.jal);
    wreg[4] = mux2(mux2(board->dec.rt[4], board->dec.rd[4], board->dec.reg_dst), 1, board->dec.jal);

    bit wr_s4 = AND(s4, board->dec.reg_write);
    bit wr_jal = AND(s2, board->dec.jal);
    regfile_set_write_addr(&board->regs, wreg);
    regfile_set_write_data(&board->regs, wb);
    regfile_set_write_enable(&board->regs, AND(OR(wr_s4, wr_jal), run));

    bit jump_target[32];
    zero_extend_26(board->dec.addr, jump_target);
    bit take_branch = AND(board->dec.branch, board->alu_flags.zero);

    bit npc[32];
    copy_word(pc_plus1, npc);
    mux2_32(npc, board->alu_out.data_out, AND(s2, take_branch), npc);
    mux2_32(npc, jump_target, AND(s2, board->dec.jump), npc);
    mux2_32(npc, board->reg_a.data_out, AND(s2, board->dec.jr), npc);

    bit pc_load = AND(OR(s0, AND(s2, OR(take_branch,
                       OR(board->dec.jump, board->dec.jr)))), run);
    pc_set_load_data(&board->pc, npc);
    pc_set_load(&board->pc, pc_load);
    pc_set_increment(&board->pc, 0);

    ir_set_input(&board->ir, instr);
    ir_set_load(&board->ir, AND(s0, run));

    reg32_set_input(&board->reg_a, board->regs.read_data1);
    reg32_set_load(&board->reg_a, AND(s1, run));
    reg32_set_input(&board->reg_b, board->regs.read_data2);
    reg32_set_load(&board->reg_b, AND(s1, run));

    reg32_set_input(&board->alu_out, alu_result);
    reg32_set_load(&board->alu_out, AND(OR(s1, s2), run));

    mar_set_input(&board->mar, alu_result);
    mar_set_load(&board->mar, AND(AND(s2, OR(board->dec.mem_read,
                                             board->dec.mem_write)), run));

    mdr_set_input(&board->mdr, sram_data);
    mdr_set_load(&board->mdr, AND(AND(s3, board->dec.mem_read), run));

    bit alu_wb = AND(board->dec.reg_write,
                     AND(NOT(board->dec.mem_to_reg), NOT(board->dec.jal)));
    bit to_s1 = s0;
    bit to_s2 = s1;
    bit to_s3 = AND(s2, OR(board->dec.mem_read, board->dec.mem_write));
    bit to_s4 = OR(AND(s2, alu_wb), AND(s3, board->dec.mem_read));
    bit ns0 = OR(to_s1, to_s3);
    bit ns1 = OR(to_s2, to_s3);
    bit ns2 = to_s4;
    board->mc_state[0].d = mux2(q0, ns0, run);
    board->mc_state[1].d = mux2(q1, ns1, run);
    board->mc_state[2].d = mux2(q2, ns2, run);

    board->halt = OR(frozen, halt_now);

    ir_clock_fall(&board->ir);
    mar_clock_fall(&board->mar);
    mdr_clock_fall(&board->mdr);
    reg32_clock_fall(&board->reg_a);
    reg32_clock_fall(&board->reg_b);
    reg32_clock_fall(&board->alu_out);
    regfile_clock_fall(&board->regs);
    pc_clock_fall(&board->pc);
    sram256_clock_fall(&board->ram);
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
    sram256_clock_rise(&board->ram);
    addr_bus_clock_rise(&board->addr_bus);
    data_bus_clock_rise(&board->data_bus);
    ctrl_bus_clock_rise(&board->ctrl_bus);
    dff_clock_rise(&board->mc_state[0]);
    dff_clock_rise(&board->mc_state[1]);
    dff_clock_rise(&board->mc_state[2]);
}

void board_dma_step(board_t *board) {
    dma_propagate(&board->dma);
    bit busreq = dma_get_bus_request(&board->dma);

    arbiter_set_request(&board->arb, 0, busreq, 0, 0);
    arbiter_propagate(&board->arb);
    bit grant = arbiter_get_grant(&board->arb, 1, 0);
    dma_set_bus_grant(&board->dma, grant);
    dma_propagate(&board->dma);

    bit dack = dma_get_dack(&board->dma);
    bit tc = dma_get_tc(&board->dma);
    bit do_xfer = AND(dack, NOT(tc));

    bit src8[8];
    src8[0] = board->dma.src_out[0];
    src8[1] = board->dma.src_out[1];
    src8[2] = board->dma.src_out[2];
    src8[3] = board->dma.src_out[3];
    src8[4] = board->dma.src_out[4];
    src8[5] = board->dma.src_out[5];
    src8[6] = board->dma.src_out[6];
    src8[7] = board->dma.src_out[7];
    sram256_set_addr(&board->ram, src8);
    sram256_set_write(&board->ram, 0);
    sram256_set_cs(&board->ram, 1);
    sram256_propagate(&board->ram);
    bit val[32];
    sram256_read(&board->ram, val);

    bit dst8[8];
    dst8[0] = board->dma.dst_out[0];
    dst8[1] = board->dma.dst_out[1];
    dst8[2] = board->dma.dst_out[2];
    dst8[3] = board->dma.dst_out[3];
    dst8[4] = board->dma.dst_out[4];
    dst8[5] = board->dma.dst_out[5];
    dst8[6] = board->dma.dst_out[6];
    dst8[7] = board->dma.dst_out[7];
    sram256_set_addr(&board->ram, dst8);
    sram256_set_data(&board->ram, val);
    sram256_set_write(&board->ram, do_xfer);
    sram256_set_cs(&board->ram, 1);

    bit snoop_addr[32];
    snoop_addr[0] = dst8[0]; snoop_addr[1] = dst8[1];
    snoop_addr[2] = dst8[2]; snoop_addr[3] = dst8[3];
    snoop_addr[4] = dst8[4]; snoop_addr[5] = dst8[5];
    snoop_addr[6] = dst8[6]; snoop_addr[7] = dst8[7];
    snoop_addr[8] = 0; snoop_addr[9] = 0; snoop_addr[10] = 0;
    snoop_addr[11] = 0; snoop_addr[12] = 0; snoop_addr[13] = 0;
    snoop_addr[14] = 0; snoop_addr[15] = 0; snoop_addr[16] = 0;
    snoop_addr[17] = 0; snoop_addr[18] = 0; snoop_addr[19] = 0;
    snoop_addr[20] = 0; snoop_addr[21] = 0; snoop_addr[22] = 0;
    snoop_addr[23] = 0; snoop_addr[24] = 0; snoop_addr[25] = 0;
    snoop_addr[26] = 0; snoop_addr[27] = 0; snoop_addr[28] = 0;
    snoop_addr[29] = 0; snoop_addr[30] = 0; snoop_addr[31] = 0;
    l1_set_addr(&board->l1, snoop_addr);
    l1_propagate(&board->l1);
    bit snoop_we = AND(do_xfer, board->l1.hit);
    l1_set_data(&board->l1, val);
    l1_set_write(&board->l1, snoop_we);

    sram256_clock_fall(&board->ram);
    dma_clock_fall(&board->dma);
    arbiter_clock_fall(&board->arb);
    l1_clock_fall(&board->l1);
    sram256_clock_rise(&board->ram);
    dma_clock_rise(&board->dma);
    arbiter_clock_rise(&board->arb);
    l1_clock_rise(&board->l1);
}
