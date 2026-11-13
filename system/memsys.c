#include "memsys.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/gates/nor.h"
#include "../core/gates/xor.h"
#include "../core/mux/mux.h"
#include "../cpu/alu/logic_unit.h"

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

static void copy_bits_8(bit *src, bit *dst) {
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
    dst[4] = src[4]; dst[5] = src[5]; dst[6] = src[6]; dst[7] = src[7];
}

static void copy_ram_bits(bit *src, bit *dst) {
    copy_bits_8(src, dst);
#if defined(ALETHEIA_MEM_1K) || defined(ALETHEIA_MEM_4K)
    dst[8] = src[8]; dst[9] = src[9];
#endif
#if defined(ALETHEIA_MEM_4K)
    dst[10] = src[10]; dst[11] = src[11];
#endif
}

static void copy_bits_16(bit *src, bit *dst) {
    copy_bits_8(src, dst);
    dst[8] = src[8]; dst[9] = src[9]; dst[10] = src[10]; dst[11] = src[11];
    dst[12] = src[12]; dst[13] = src[13]; dst[14] = src[14]; dst[15] = src[15];
}

static void and_word_1(bit *src, bit en, bit *dst) {
    bit en_word[32];
    en_word[0] = en; en_word[1] = en; en_word[2] = en; en_word[3] = en;
    en_word[4] = en; en_word[5] = en; en_word[6] = en; en_word[7] = en;
    en_word[8] = en; en_word[9] = en; en_word[10] = en; en_word[11] = en;
    en_word[12] = en; en_word[13] = en; en_word[14] = en; en_word[15] = en;
    en_word[16] = en; en_word[17] = en; en_word[18] = en; en_word[19] = en;
    en_word[20] = en; en_word[21] = en; en_word[22] = en; en_word[23] = en;
    en_word[24] = en; en_word[25] = en; en_word[26] = en; en_word[27] = en;
    en_word[28] = en; en_word[29] = en; en_word[30] = en; en_word[31] = en;
    and_word_32(src, en_word, dst);
}

void memsys_irq(board_t *b) {
    timer_propagate(&b->timer);
    bit irq_lines[8];
    irq_lines[0] = timer_get_irq(&b->timer);
    irq_lines[1] = 0; irq_lines[2] = 0; irq_lines[3] = 0;
    irq_lines[4] = 0; irq_lines[5] = 0; irq_lines[6] = 0; irq_lines[7] = 0;
    pic_raise(&b->pic, irq_lines);
    intc_set_global_enable(&b->pic.ic, 1);
    pic_propagate(&b->pic);
}

static void decode_io(bit *va, mem_port_t *p) {
    bit high_one = AND(AND(AND(AND(va[9], va[10]), AND(va[11], va[12])),
                           AND(AND(va[13], va[14]), AND(va[15], va[16]))),
                       AND(AND(AND(va[17], va[18]), AND(va[19], va[20])),
                           AND(AND(va[21], va[22]), AND(va[23], va[24]))));
    bit top_one = AND(AND(AND(va[25], va[26]), AND(va[27], va[28])),
                      AND(AND(va[29], va[30]), va[31]));
    bit io_page = AND(AND(high_one, top_one), AND(va[8], NOT(va[7])));
    bit a0 = va[0], a1 = va[1], a2 = va[2], a3 = va[3], d0 = va[4], d1 = va[5];
    bit dev = AND(NOT(va[6]), io_page);
    p->is_dram = AND(AND(va[6], NOR(d0, d1)), io_page);
    p->io_sel = OR(dev, p->is_dram);
    p->is_uart = AND(AND(AND(NOT(d1), NOT(d0)), NOR(OR(a1, a2), a3)), dev);
    p->is_disp = AND(AND(AND(NOT(d1), d0), NOT(a3)), dev);
    p->is_gpio = AND(AND(AND(d1, NOT(d0)), NOR(a2, a3)), dev);
    bit is_sys = AND(AND(AND(d1, d0), NOT(a3)), dev);
    bit sys_io = AND(p->io_sel, is_sys);
    p->sel_timer = AND(sys_io, NOT(a2));
    p->sel_picmask = AND(sys_io, AND(a2, AND(NOT(a1), NOT(a0))));
    p->sel_ptfill = AND(sys_io, AND(a2, AND(NOT(a1), a0)));
    p->sel_mmuen = AND(sys_io, AND(a2, AND(a1, NOT(a0))));
    p->sel_mode = AND(sys_io, AND(a2, AND(a1, a0)));
    p->sel_dma = AND(AND(dev, AND(d1, d0)), AND(a3, NOT(a2)));
}

static void translate(board_t *b, mem_port_t *p) {
    bit ptfill_req = AND(p->write, p->sel_ptfill);
    bit fill_vaddr[32] = {0};
    fill_vaddr[4] = p->wdata[20];
    fill_vaddr[5] = p->wdata[21];
    fill_vaddr[6] = p->wdata[22];
    fill_vaddr[7] = p->wdata[23];
    bit mmu_vaddr[32];
    mux2_32(p->vaddr, fill_vaddr, ptfill_req, mmu_vaddr);
    mmu_set_vaddr(&b->mmu, mmu_vaddr);
    mmu_propagate(&b->mmu);
    bit user = b->umode.q;
    bit access = OR(p->read, p->write);
    bit mapping = OR(AND(b->mmu_en.q, config_mmu(&b->config)), user);
    p->translate = AND(mapping, AND(access, NOT(p->io_sel)));
    bit denied = OR(b->mmu.fault, AND(user, NOT(b->mmu.paddr[23])));
    p->fault = OR(AND(p->translate, denied), AND(user, AND(access, p->io_sel)));
}

static void read_ram(board_t *b, mem_port_t *p, bit *cached) {
    bit paddr[32];
    mux2_32(p->vaddr, b->mmu.paddr, p->translate, paddr);
    ram_set_addr(&b->ram, paddr);
    ram_set_data(&b->ram, p->wdata);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    bit sram_word[32];
    ram_read(&b->ram, sram_word);
    bit l1_addr[32] = {0};
    copy_ram_bits(paddr, l1_addr);
    l2_set_addr(&b->l2, l1_addr);
    l2_propagate(&b->l2);
    p->l2_hit = AND(b->l2.hit, config_l2(&b->config));
    mux2_32(sram_word, b->l2.data_out, p->l2_hit, p->sram_data);
    l1_set_addr(&b->l1, l1_addr);
    l1_propagate(&b->l1);
    p->l1_hit = AND(b->l1.hit, config_l1(&b->config));
    mux2_32(p->sram_data, b->l1.data_out, p->l1_hit, cached);
}

static void read_sys(board_t *b, bit *va, bit *out) {
    bit tword[32] = {0};
    timer_read_data(&b->timer, tword);
    bit mask_word[32] = {0};
    mask_word[0] = b->pic.ic.mask[0].q; mask_word[1] = b->pic.ic.mask[1].q;
    mask_word[2] = b->pic.ic.mask[2].q; mask_word[3] = b->pic.ic.mask[3].q;
    mask_word[4] = b->pic.ic.mask[4].q; mask_word[5] = b->pic.ic.mask[5].q;
    mask_word[6] = b->pic.ic.mask[6].q; mask_word[7] = b->pic.ic.mask[7].q;
    bit mmuen_word[32] = {b->mmu_en.q, 0};
    bit sys_hi[32], sys_hi_valid[32];
    mux2_32(mask_word, mmuen_word, va[1], sys_hi);
    and_word_1(sys_hi, NOT(va[0]), sys_hi_valid);
    mux2_32(tword, sys_hi_valid, va[2], out);
}

static void read_dma(board_t *b, bit *va, bit *out) {
    dma_propagate(&b->dma);
    bit src[32] = {0}, dst[32] = {0}, cnt[32] = {0};
    copy_bits_16(b->dma.src_out, src);
    copy_bits_16(b->dma.dst_out, dst);
    copy_bits_16(b->dma.count_out, cnt);
    bit ctl[32] = {b->dma.control[0].q, b->dma.tc, 0};
    mux4_32(src, dst, cnt, ctl, va[0], va[1], out);
}

static void read_io(board_t *b, mem_port_t *p, bit *out) {
    bit *va = p->vaddr;
    uart_tx_propagate(&b->uart);
    uart_rx_propagate(&b->uart_rx);
    bit uart_status[32] = {b->uart.ready, b->uart_rx.avail_out, b->uart.overrun_out,
                           b->uart_rx.overrun_out, b->uart_rx.framing_out, 0};
    bit uart_data[32] = {0};
    copy_bits_8(b->uart_rx.holding_out, uart_data);
    bit uart_word[32];
    mux2_32(uart_status, uart_data, va[0], uart_word);

    bit gpio_word[32] = {0};
    gpio_propagate(&b->gpio);
    gpio_read_data(&b->gpio, gpio_word);

    bit sys_lo[32], dma_word[32], sys_word[32];
    read_sys(b, va, sys_lo);
    read_dma(b, va, dma_word);
    mux2_32(sys_lo, dma_word, va[3], sys_word);

    bit zero_word[32] = {0};
    bit dev_lo[32], dev_hi[32], dev_word[32];
    mux2_32(uart_word, zero_word, va[4], dev_lo);
    mux2_32(gpio_word, sys_word, va[4], dev_hi);
    mux2_32(dev_lo, dev_hi, va[5], dev_word);
    bit mapped = OR(OR(OR(p->is_uart, p->is_disp), OR(p->is_gpio, AND(va[4], AND(va[5], NOT(va[3]))))), p->sel_dma);
    bit dev_valid[32], dram_word[32];
    and_word_1(dev_word, mapped, dev_valid);
    dram_set_addr(&b->dram, va);
    dram_propagate(&b->dram);
    copy_word(b->dram.data_out, dram_word);
    mux2_32(dev_valid, dram_word, p->is_dram, out);
}

void memsys_probe(board_t *b, bit *vaddr, bit *wdata, bit read, bit write, mem_port_t *p) {
    copy_word(vaddr, p->vaddr);
    copy_word(wdata, p->wdata);
    p->read = read;
    p->write = write;
    decode_io(vaddr, p);
    timer_set_cs(&b->timer, p->sel_timer);
    timer_set_addr(&b->timer, vaddr[0], vaddr[1]);
    gpio_set_cs(&b->gpio, AND(p->io_sel, p->is_gpio));
    gpio_set_addr(&b->gpio, vaddr[0], vaddr[1]);
    translate(b, p);
    bit cached[32], io_data[32];
    read_ram(b, p, cached);
    read_io(b, p, io_data);
    mux2_32(cached, io_data, p->io_sel, p->rdata);
    copy_word(vaddr, b->probe_addr);
    copy_word(wdata, b->probe_wdata);
    copy_word(p->rdata, b->probe_rdata);
    b->probe_read = read;
    b->probe_write = write;
}

static void commit_ram(board_t *b, mem_port_t *p, bit store_en, bit load_en) {
    bit ram_ok = AND(NOT(p->io_sel), NOT(p->fault));
    bit sram_we = AND(store_en, ram_ok);
    ram_set_write(&b->ram, sram_we);
    bit l1_fill = AND(load_en, NOT(p->l1_hit));
    bit l1_din[32];
    mux2_32(p->sram_data, p->wdata, p->write, l1_din);
    l1_set_data(&b->l1, l1_din);
    l1_set_write(&b->l1, AND(config_l1(&b->config), AND(ram_ok, OR(sram_we, l1_fill))));
    bit l2_fill = AND(l1_fill, NOT(p->l2_hit));
    l2_set_data(&b->l2, l1_din);
    l2_set_write(&b->l2, AND(config_l2(&b->config), AND(ram_ok, OR(sram_we, l2_fill))));
}

static void commit_devices(board_t *b, mem_port_t *p, bit store_en, bit load_en) {
    bit *va = p->vaddr;
    bit uart_rd = AND(load_en, AND(p->io_sel, p->is_uart));
    uart_tx_set_data(&b->uart, p->wdata);
    uart_tx_set_load(&b->uart, AND(store_en, AND(p->io_sel, p->is_uart)));
    uart_tx_set_clear_status(&b->uart, AND(uart_rd, NOT(va[0])));
    uart_rx_set_serial(&b->uart_rx, b->serial_in);
    uart_rx_set_read(&b->uart_rx, AND(uart_rd, va[0]), AND(uart_rd, NOT(va[0])));

    display_set_addr(&b->disp, va);
    display_set_data(&b->disp, p->wdata);
    display_set_write(&b->disp, AND(store_en, AND(p->io_sel, p->is_disp)));

    gpio_set_write(&b->gpio, AND(store_en, AND(p->io_sel, p->is_gpio)));
    gpio_set_data(&b->gpio, p->wdata);

    timer_set_write(&b->timer, AND(store_en, p->sel_timer));
    timer_set_read(&b->timer, AND(load_en, p->sel_timer));
    timer_set_data(&b->timer, p->wdata);
}

static void commit_dram(board_t *b, mem_port_t *p, bit store_en) {
    bit access = AND(OR(p->read, p->write), p->is_dram);
    bit *r0 = &b->dram_row[0].q, *r1 = &b->dram_row[1].q, *r2 = &b->dram_row[2].q, *r3 = &b->dram_row[3].q;
    bit row[4] = {*r0, *r1, *r2, *r3};
    bit addr[4];
    addr[0] = mux2(row[0], p->vaddr[0], access);
    addr[1] = mux2(row[1], p->vaddr[1], access);
    addr[2] = mux2(row[2], p->vaddr[2], access);
    addr[3] = mux2(row[3], p->vaddr[3], access);
    dram_set_addr(&b->dram, addr);
    dram_set_data(&b->dram, p->wdata);
    dram_set_write(&b->dram, AND(store_en, p->is_dram));
    dram_set_refresh(&b->dram, NOT(access));
    dram_set_leak(&b->dram, b->dram_leak);
    bit c1 = *r0, c2 = AND(*r0, *r1), c3 = AND(c2, *r2);
    b->dram_row[0].d = NOT(*r0);
    b->dram_row[1].d = XOR(*r1, c1);
    b->dram_row[2].d = XOR(*r2, c2);
    b->dram_row[3].d = XOR(*r3, c3);
}

static void commit_system(board_t *b, mem_port_t *p, bit store_en) {
    bit *w = p->wdata;
    bit picmask_we = AND(store_en, p->sel_picmask);
    bit mask_next[8];
    mask_next[0] = mux2(b->pic.ic.mask[0].q, w[0], picmask_we);
    mask_next[1] = mux2(b->pic.ic.mask[1].q, w[1], picmask_we);
    mask_next[2] = mux2(b->pic.ic.mask[2].q, w[2], picmask_we);
    mask_next[3] = mux2(b->pic.ic.mask[3].q, w[3], picmask_we);
    mask_next[4] = mux2(b->pic.ic.mask[4].q, w[4], picmask_we);
    mask_next[5] = mux2(b->pic.ic.mask[5].q, w[5], picmask_we);
    mask_next[6] = mux2(b->pic.ic.mask[6].q, w[6], picmask_we);
    mask_next[7] = mux2(b->pic.ic.mask[7].q, w[7], picmask_we);
    intc_set_mask(&b->pic.ic, mask_next);

    b->mmu_en.d = mux2(b->mmu_en.q, w[0], AND(store_en, p->sel_mmuen));
    b->umode.d = OR(b->umode.d, AND(AND(store_en, p->sel_mode), w[0]));

    bit slot = b->slot_sel;
    bit dma_addr[3], dma_data[32];
    dma_addr[0] = mux2(p->vaddr[0], b->slot_addr[0], slot);
    dma_addr[1] = mux2(p->vaddr[1], b->slot_addr[1], slot);
    dma_addr[2] = mux2(p->vaddr[2], b->slot_addr[2], slot);
    mux2_32(w, b->slot_data, slot, dma_data);
    dma_set_addr(&b->dma, dma_addr);
    dma_set_cpu_data(&b->dma, dma_data);
    dma_set_cs(&b->dma, OR(p->sel_dma, slot));
    dma_set_write(&b->dma, OR(AND(store_en, p->sel_dma), slot));

    mmu_set_pt_pfn(&b->mmu, w);
    mmu_set_pt_fill(&b->mmu, AND(store_en, p->sel_ptfill));
    mmu_set_pt_valid(&b->mmu, w[24]);
    mmu_set_fill_pfn(&b->mmu, b->mmu.pt.pfn_out);
    mmu_set_fill(&b->mmu, AND(p->translate, AND(NOT(b->mmu.tlb_hit), b->mmu.pt_hit)));
}

void memsys_watch_instr(board_t *b, bit *instr) {
    copy_word(instr, b->probe_instr);
}

void memsys_commit(board_t *b, mem_port_t *p, bit enable) {
    bit go = AND(enable, NOT(p->fault));
    bit store_en = AND(p->write, go);
    b->probe_store = store_en;
    bit load_en = AND(p->read, go);
    commit_ram(b, p, store_en, load_en);
    commit_devices(b, p, store_en, load_en);
    commit_system(b, p, store_en);
    commit_dram(b, p, store_en);
}

void board_exceptions(board_t *b, bit *epc, bit fault, bit undefined, bit irq_allowed,
                      bit reti_in, bit run, exc_outcome_t *out) {
    bit user = b->umode.q;
    bit reti = AND(reti_in, NOT(user));
    bit fault_r = AND(fault, run);
    bit undef_r = AND(OR(undefined, AND(reti_in, user)), run);
    bit irq_req = AND(AND(b->pic.int_request, irq_allowed), AND(run, NOR(fault_r, undef_r)));
    exc_set_addr_error(&b->exc, fault_r);
    exc_set_undefined(&b->exc, undef_r);
    exc_set_interrupt(&b->exc, irq_req, b->pic.int_vector);
    exc_set_pc(&b->exc, epc);
    exc_propagate(&b->exc);
    out->take = AND(exc_get_exception_taken(&b->exc), run);
    out->ok = AND(run, NOT(out->take));
    intc_set_service(&b->pic.ic, AND(out->take, irq_req), b->pic.int_vector);
    out->reti = AND(reti, out->ok);
    exc_set_return(&b->exc, out->reti);
    bit ack_num[3], svc_any;
    intc_get_top_service(&b->pic.ic, ack_num, &svc_any);
    intc_set_ack(&b->pic.ic, AND(out->reti, svc_any), ack_num);
    b->umode.d = mux2(mux2(user, b->uprev.q, out->reti), 0, out->take);
    b->uprev.d = mux2(b->uprev.q, user, out->take);
}

bit memsys_bus_hold(board_t *b) {
    dma_propagate(&b->dma);
    arbiter_set_request(&b->arb, 0, dma_get_bus_request(&b->dma), 0, 0);
    arbiter_propagate(&b->arb);
    dma_set_bus_grant(&b->dma, arbiter_get_grant(&b->arb, 1, 0));
    dma_propagate(&b->dma);
    return AND(dma_get_dack(&b->dma), NOT(dma_get_tc(&b->dma)));
}

static void dma_transfer(board_t *b) {
    bit do_xfer = AND(dma_get_dack(&b->dma), NOT(dma_get_tc(&b->dma)));
    bit src[32] = {0}, dst[32] = {0}, val[32];
    copy_ram_bits(b->dma.src_out, src);
    copy_ram_bits(b->dma.dst_out, dst);
    ram_set_addr(&b->ram, src);
    ram_set_write(&b->ram, 0);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_read(&b->ram, val);
    ram_set_addr(&b->ram, dst);
    ram_set_data(&b->ram, val);
    ram_set_write(&b->ram, do_xfer);
    l1_set_addr(&b->l1, dst);
    l1_propagate(&b->l1);
    l1_set_data(&b->l1, val);
    l1_set_write(&b->l1, AND(AND(do_xfer, b->l1.hit), config_l1(&b->config)));
    l2_set_addr(&b->l2, dst);
    l2_propagate(&b->l2);
    l2_set_data(&b->l2, val);
    l2_set_write(&b->l2, AND(AND(do_xfer, b->l2.hit), config_l2(&b->config)));
}

void memsys_dma_cycle(board_t *b) {
    bit zero[32] = {0};
    mem_port_t idle;
    exc_outcome_t none;
    memsys_irq(b);
    memsys_probe(b, zero, zero, 0, 0, &idle);
    memsys_commit(b, &idle, 0);
    board_exceptions(b, zero, 0, 0, 0, 0, 0, &none);
    dma_transfer(b);
    memsys_clock_fall(b);
    memsys_clock_rise(b);
}

static void prescaler(board_t *b) {
    bit q0 = b->prescale[0].q, q1 = b->prescale[1].q, q2 = b->prescale[2].q;
    bit div[2];
    config_timer_div(&b->config, div);
    bit z01 = NOR(q0, q1);
    timer_set_tick(&b->timer, mux4(1, NOT(q0), z01, AND(z01, NOT(q2)), div[0], div[1]));
    b->prescale[0].d = NOT(q0);
    b->prescale[1].d = XOR(q1, q0);
    b->prescale[2].d = XOR(q2, AND(q0, q1));
}

void memsys_clock_fall(board_t *b) {
    prescaler(b);
    dff_clock_fall(&b->prescale[0]); dff_clock_fall(&b->prescale[1]); dff_clock_fall(&b->prescale[2]);
    dram_clock_fall(&b->dram);
    dff_clock_fall(&b->dram_row[0]); dff_clock_fall(&b->dram_row[1]);
    dff_clock_fall(&b->dram_row[2]); dff_clock_fall(&b->dram_row[3]);
    dma_clock_fall(&b->dma);
    arbiter_clock_fall(&b->arb);
    ram_clock_fall(&b->ram);
    uart_tx_clock_fall(&b->uart);
    uart_rx_clock_fall(&b->uart_rx);
    display_clock_fall(&b->disp);
    gpio_clock_fall(&b->gpio);
    timer_clock_fall(&b->timer);
    pic_clock_fall(&b->pic);
    exc_clock_fall(&b->exc);
    mmu_clock_fall(&b->mmu);
    l1_clock_fall(&b->l1);
    l2_clock_fall(&b->l2);
    dff_clock_fall(&b->mmu_en);
    dff_clock_fall(&b->umode);
    dff_clock_fall(&b->uprev);
}

void memsys_clock_rise(board_t *b) {
    dff_clock_rise(&b->prescale[0]); dff_clock_rise(&b->prescale[1]); dff_clock_rise(&b->prescale[2]);
    dram_clock_rise(&b->dram);
    dff_clock_rise(&b->dram_row[0]); dff_clock_rise(&b->dram_row[1]);
    dff_clock_rise(&b->dram_row[2]); dff_clock_rise(&b->dram_row[3]);
    dma_clock_rise(&b->dma);
    arbiter_clock_rise(&b->arb);
    ram_clock_rise(&b->ram);
    uart_tx_clock_rise(&b->uart);
    uart_rx_clock_rise(&b->uart_rx);
    display_clock_rise(&b->disp);
    gpio_clock_rise(&b->gpio);
    timer_clock_rise(&b->timer);
    pic_clock_rise(&b->pic);
    exc_clock_rise(&b->exc);
    mmu_clock_rise(&b->mmu);
    l1_clock_rise(&b->l1);
    l2_clock_rise(&b->l2);
    dff_clock_rise(&b->mmu_en);
    dff_clock_rise(&b->umode);
    dff_clock_rise(&b->uprev);
    uart_tx_propagate(&b->uart);
    display_propagate(&b->disp);
    gpio_propagate(&b->gpio);
}
