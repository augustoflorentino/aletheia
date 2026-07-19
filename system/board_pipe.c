#include "board.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"
#include "../cpu/alu/adder.h"

static void pset32(d_flip_flop_t *d, bit *s) {
    d[0].d = s[0];
    d[1].d = s[1];
    d[2].d = s[2];
    d[3].d = s[3];
    d[4].d = s[4];
    d[5].d = s[5];
    d[6].d = s[6];
    d[7].d = s[7];
    d[8].d = s[8];
    d[9].d = s[9];
    d[10].d = s[10];
    d[11].d = s[11];
    d[12].d = s[12];
    d[13].d = s[13];
    d[14].d = s[14];
    d[15].d = s[15];
    d[16].d = s[16];
    d[17].d = s[17];
    d[18].d = s[18];
    d[19].d = s[19];
    d[20].d = s[20];
    d[21].d = s[21];
    d[22].d = s[22];
    d[23].d = s[23];
    d[24].d = s[24];
    d[25].d = s[25];
    d[26].d = s[26];
    d[27].d = s[27];
    d[28].d = s[28];
    d[29].d = s[29];
    d[30].d = s[30];
    d[31].d = s[31];
}

static void pread32(d_flip_flop_t *s, bit *d) {
    d[0] = s[0].q;
    d[1] = s[1].q;
    d[2] = s[2].q;
    d[3] = s[3].q;
    d[4] = s[4].q;
    d[5] = s[5].q;
    d[6] = s[6].q;
    d[7] = s[7].q;
    d[8] = s[8].q;
    d[9] = s[9].q;
    d[10] = s[10].q;
    d[11] = s[11].q;
    d[12] = s[12].q;
    d[13] = s[13].q;
    d[14] = s[14].q;
    d[15] = s[15].q;
    d[16] = s[16].q;
    d[17] = s[17].q;
    d[18] = s[18].q;
    d[19] = s[19].q;
    d[20] = s[20].q;
    d[21] = s[21].q;
    d[22] = s[22].q;
    d[23] = s[23].q;
    d[24] = s[24].q;
    d[25] = s[25].q;
    d[26] = s[26].q;
    d[27] = s[27].q;
    d[28] = s[28].q;
    d[29] = s[29].q;
    d[30] = s[30].q;
    d[31] = s[31].q;
}

static void pset5(d_flip_flop_t *d, bit *s) {
    d[0].d = s[0];
    d[1].d = s[1];
    d[2].d = s[2];
    d[3].d = s[3];
    d[4].d = s[4];
}

static void pread5(d_flip_flop_t *s, bit *d) {
    d[0] = s[0].q;
    d[1] = s[1].q;
    d[2] = s[2].q;
    d[3] = s[3].q;
    d[4] = s[4].q;
}

static void pset4(d_flip_flop_t *d, bit *s) {
    d[0].d = s[0];
    d[1].d = s[1];
    d[2].d = s[2];
    d[3].d = s[3];
}

static void pread4(d_flip_flop_t *s, bit *d) {
    d[0] = s[0].q;
    d[1] = s[1].q;
    d[2] = s[2].q;
    d[3] = s[3].q;
}

static void pcopy32(bit *s, bit *d) {
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
    d[7] = s[7];
    d[8] = s[8];
    d[9] = s[9];
    d[10] = s[10];
    d[11] = s[11];
    d[12] = s[12];
    d[13] = s[13];
    d[14] = s[14];
    d[15] = s[15];
    d[16] = s[16];
    d[17] = s[17];
    d[18] = s[18];
    d[19] = s[19];
    d[20] = s[20];
    d[21] = s[21];
    d[22] = s[22];
    d[23] = s[23];
    d[24] = s[24];
    d[25] = s[25];
    d[26] = s[26];
    d[27] = s[27];
    d[28] = s[28];
    d[29] = s[29];
    d[30] = s[30];
    d[31] = s[31];
}

static void psext16(bit *imm, bit *out) {
    out[0] = imm[0];
    out[1] = imm[1];
    out[2] = imm[2];
    out[3] = imm[3];
    out[4] = imm[4];
    out[5] = imm[5];
    out[6] = imm[6];
    out[7] = imm[7];
    out[8] = imm[8];
    out[9] = imm[9];
    out[10] = imm[10];
    out[11] = imm[11];
    out[12] = imm[12];
    out[13] = imm[13];
    out[14] = imm[14];
    out[15] = imm[15];
    out[16] = imm[15];
    out[17] = imm[15];
    out[18] = imm[15];
    out[19] = imm[15];
    out[20] = imm[15];
    out[21] = imm[15];
    out[22] = imm[15];
    out[23] = imm[15];
    out[24] = imm[15];
    out[25] = imm[15];
    out[26] = imm[15];
    out[27] = imm[15];
    out[28] = imm[15];
    out[29] = imm[15];
    out[30] = imm[15];
    out[31] = imm[15];
}

void board_pipe_init(board_t *b) {
    if_id_init(&b->if_id);
    id_ex_init(&b->id_ex);
    ex_mem_init(&b->ex_mem);
    mem_wb_init(&b->mem_wb);
    forwarding_init(&b->fwd);
    hazard_init(&b->haz);
    pc_reset(&b->pc);
    regfile_reset(&b->regs);
    sram256_reset(&b->ram);
    b->halt = 0;
}

void board_step_pipe(board_t *b) {
    bit run = NOT(b->halt);

    bit mw_alu[32], mw_mem[32], mw_wreg[5];
    pread32(b->mem_wb.alu_result, mw_alu);
    pread32(b->mem_wb.mem_data, mw_mem);
    pread5(b->mem_wb.write_reg, mw_wreg);
    bit mw_regwrite = b->mem_wb.reg_write.q;
    bit mw_valid = b->mem_wb.valid.q;
    bit wb_data[32];
    mux2_32(mw_alu, mw_mem, b->mem_wb.mem_to_reg.q, wb_data);
    regfile_set_write_addr(&b->regs, mw_wreg);
    regfile_set_write_data(&b->regs, wb_data);
    regfile_set_write_enable(&b->regs, AND(mw_regwrite, mw_valid));
    regfile_clock_fall(&b->regs);
    regfile_clock_rise(&b->regs);

    bit em_alu[32], em_rt[32], em_wreg[5];
    pread32(b->ex_mem.alu_result, em_alu);
    pread32(b->ex_mem.rt_data, em_rt);
    pread5(b->ex_mem.write_reg, em_wreg);
    bit em_memwrite = b->ex_mem.mem_write.q;
    bit em_regwrite = b->ex_mem.reg_write.q;
    bit em_valid = b->ex_mem.valid.q;
    bit maddr[8];
    maddr[0]=em_alu[0]; maddr[1]=em_alu[1]; maddr[2]=em_alu[2]; maddr[3]=em_alu[3];
    maddr[4]=em_alu[4]; maddr[5]=em_alu[5]; maddr[6]=em_alu[6]; maddr[7]=em_alu[7];
    sram256_set_addr(&b->ram, maddr);
    sram256_set_data(&b->ram, em_rt);
    sram256_set_write(&b->ram, AND(AND(em_memwrite, em_valid), run));
    sram256_set_cs(&b->ram, 1);
    sram256_propagate(&b->ram);
    bit em_memdata[32];
    sram256_read(&b->ram, em_memdata);
    pset32(b->mem_wb.alu_result, em_alu);
    pset32(b->mem_wb.mem_data, em_memdata);
    pset5(b->mem_wb.write_reg, em_wreg);
    b->mem_wb.mem_to_reg.d = b->ex_mem.mem_to_reg.q;
    b->mem_wb.reg_write.d = em_regwrite;
    b->mem_wb.valid.d = em_valid;

    bit ix_pc[32], ix_rsdata[32], ix_rtdata[32], ix_imm[32], ix_rs[5], ix_rt[5], ix_rd[5], ix_aluop[4];
    pread32(b->id_ex.pc, ix_pc);
    pread32(b->id_ex.rs_data, ix_rsdata);
    pread32(b->id_ex.rt_data, ix_rtdata);
    pread32(b->id_ex.imm_extended, ix_imm);
    pread5(b->id_ex.rs, ix_rs);
    pread5(b->id_ex.rt, ix_rt);
    pread5(b->id_ex.rd, ix_rd);
    pread4(b->id_ex.alu_op, ix_aluop);
    bit ix_alusrc = b->id_ex.alu_src.q;
    bit ix_memread = b->id_ex.mem_read.q;
    bit ix_branch = b->id_ex.branch.q;
    bit ix_valid = b->id_ex.valid.q;

    forwarding_set_id_ex_rs(&b->fwd, ix_rs);
    forwarding_set_id_ex_rt(&b->fwd, ix_rt);
    forwarding_set_ex_mem_rd(&b->fwd, em_wreg);
    forwarding_set_mem_wb_rd(&b->fwd, mw_wreg);
    forwarding_set_ex_mem_reg_write(&b->fwd, AND(em_regwrite, em_valid));
    forwarding_set_mem_wb_reg_write(&b->fwd, AND(mw_regwrite, mw_valid));
    forwarding_propagate(&b->fwd);
    bit fa[2], fb[2];
    forwarding_get_forward_a(&b->fwd, fa);
    forwarding_get_forward_b(&b->fwd, fb);

    bit opA[32], opB[32], tA[32], tB[32];
    mux2_32(ix_rsdata, wb_data, fa[0], tA);
    mux2_32(tA, em_alu, fa[1], opA);
    mux2_32(ix_rtdata, wb_data, fb[0], tB);
    mux2_32(tB, em_alu, fb[1], opB);

    bit alu_b[32];
    mux2_32(opB, ix_imm, ix_alusrc, alu_b);
    bit base0 = OR(ix_aluop[0], AND(ix_aluop[2], ix_aluop[1]));
    bit base1 = ix_aluop[1];
    bit ctrl[3];
    ctrl[0] = mux2(base0, 1, ix_branch);
    ctrl[1] = mux2(base1, 0, ix_branch);
    ctrl[2] = 0;
    bit ex_result[32];
    alu_flags_t flags;
    alu_execute(opA, alu_b, ctrl, ex_result, &flags);
    bit branch_taken = AND(AND(ix_branch, flags.zero), ix_valid);
    bit btarget[32], bcarry[33];
    adder_32(ix_pc, ix_imm, btarget, bcarry);

    pset32(b->ex_mem.alu_result, ex_result);
    pset32(b->ex_mem.rt_data, opB);
    pset5(b->ex_mem.write_reg, ix_rd);
    b->ex_mem.mem_read.d = b->id_ex.mem_read.q;
    b->ex_mem.mem_write.d = b->id_ex.mem_write.q;
    b->ex_mem.mem_to_reg.d = b->id_ex.mem_to_reg.q;
    b->ex_mem.reg_write.d = b->id_ex.reg_write.q;
    b->ex_mem.zero_flag.d = flags.zero;
    b->ex_mem.valid.d = ix_valid;

    bit ii_pc[32], ii_instr[32];
    pread32(b->if_id.pc, ii_pc);
    pread32(b->if_id.instruction, ii_instr);
    bit ii_valid = b->if_id.valid.q;
    decoder_set_instruction(&b->dec, ii_instr);
    decoder_propagate(&b->dec);
    regfile_set_read_addr1(&b->regs, b->dec.rs);
    regfile_set_read_addr2(&b->regs, b->dec.rt);
    regfile_propagate(&b->regs);
    bit id_rd1[32], id_rd2[32];
    pcopy32(b->regs.read_data1, id_rd1);
    pcopy32(b->regs.read_data2, id_rd2);
    bit id_imm[32];
    psext16(b->dec.imm, id_imm);
    bit id_wreg[5];
    id_wreg[0] = mux2(b->dec.rt[0], b->dec.rd[0], b->dec.reg_dst);
    id_wreg[1] = mux2(b->dec.rt[1], b->dec.rd[1], b->dec.reg_dst);
    id_wreg[2] = mux2(b->dec.rt[2], b->dec.rd[2], b->dec.reg_dst);
    id_wreg[3] = mux2(b->dec.rt[3], b->dec.rd[3], b->dec.reg_dst);
    id_wreg[4] = mux2(b->dec.rt[4], b->dec.rd[4], b->dec.reg_dst);

    hazard_set_id_ex_mem_read(&b->haz, AND(ix_memread, ix_valid));
    hazard_set_id_ex_rt(&b->haz, ix_rt);
    hazard_set_if_id_rs(&b->haz, b->dec.rs);
    hazard_set_if_id_rt(&b->haz, b->dec.rt);
    hazard_set_ex_mem_branch(&b->haz, 0);
    hazard_set_ex_mem_zero(&b->haz, 0);
    hazard_propagate(&b->haz);
    bit stall = hazard_get_stall(&b->haz);

    pset32(b->id_ex.pc, ii_pc);
    pset32(b->id_ex.rs_data, id_rd1);
    pset32(b->id_ex.rt_data, id_rd2);
    pset32(b->id_ex.imm_extended, id_imm);
    pset5(b->id_ex.rs, b->dec.rs);
    pset5(b->id_ex.rt, b->dec.rt);
    pset5(b->id_ex.rd, id_wreg);
    pset4(b->id_ex.alu_op, b->dec.alu_op);
    b->id_ex.alu_src.d = b->dec.alu_src;
    b->id_ex.reg_dst.d = b->dec.reg_dst;
    b->id_ex.mem_read.d = b->dec.mem_read;
    b->id_ex.mem_write.d = b->dec.mem_write;
    b->id_ex.mem_to_reg.d = b->dec.mem_to_reg;
    b->id_ex.reg_write.d = b->dec.reg_write;
    b->id_ex.branch.d = b->dec.branch;
    b->id_ex.valid.d = ii_valid;

    pc_propagate(&b->pc);
    bit pcaddr[8];
    pcaddr[0]=b->pc.reg.data_out[0]; pcaddr[1]=b->pc.reg.data_out[1];
    pcaddr[2]=b->pc.reg.data_out[2]; pcaddr[3]=b->pc.reg.data_out[3];
    pcaddr[4]=b->pc.reg.data_out[4]; pcaddr[5]=b->pc.reg.data_out[5];
    pcaddr[6]=b->pc.reg.data_out[6]; pcaddr[7]=b->pc.reg.data_out[7];
    rom256_set_addr(&b->rom, pcaddr);
    rom256_set_cs(&b->rom, 1);
    rom256_propagate(&b->rom);
    bit if_instr[32];
    rom256_read(&b->rom, if_instr);
    bit one_w[32] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bit pc_plus1[32], pc_carry[33];
    adder_32(b->pc.reg.data_out, one_w, pc_plus1, pc_carry);
    pset32(b->if_id.pc, pc_plus1);
    pset32(b->if_id.instruction, if_instr);
    b->if_id.valid.d = run;

    bit halt_now = AND(ii_valid, AND(AND(b->dec.opcode[0], b->dec.opcode[1]),
                       AND(AND(b->dec.opcode[2], b->dec.opcode[3]),
                           AND(b->dec.opcode[4], b->dec.opcode[5]))));

    if_id_set_stall(&b->if_id, AND(stall, NOT(branch_taken)));
    if_id_set_flush(&b->if_id, branch_taken);
    id_ex_set_stall(&b->id_ex, 0);
    id_ex_set_flush(&b->id_ex, OR(stall, branch_taken));
    ex_mem_set_flush(&b->ex_mem, 0);

    bit next_pc[32];
    mux2_32(pc_plus1, btarget, branch_taken, next_pc);
    bit pc_load = AND(OR(NOT(stall), branch_taken), run);
    pc_set_load_data(&b->pc, next_pc);
    pc_set_load(&b->pc, pc_load);
    pc_set_increment(&b->pc, 0);

    b->halt = OR(b->halt, halt_now);

    if_id_clock_fall(&b->if_id);
    id_ex_clock_fall(&b->id_ex);
    ex_mem_clock_fall(&b->ex_mem);
    mem_wb_clock_fall(&b->mem_wb);
    pc_clock_fall(&b->pc);
    sram256_clock_fall(&b->ram);
    if_id_clock_rise(&b->if_id);
    id_ex_clock_rise(&b->id_ex);
    ex_mem_clock_rise(&b->ex_mem);
    mem_wb_clock_rise(&b->mem_wb);
    pc_clock_rise(&b->pc);
    sram256_clock_rise(&b->ram);
}
