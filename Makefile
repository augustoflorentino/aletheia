CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -I.
LDFLAGS =

CORE_SRCS = core/gates/and.c core/gates/or.c core/gates/not.c core/gates/xor.c \
            core/gates/nand.c core/gates/nor.c \
            core/sequential/sr_latch.c core/sequential/d_latch.c \
            core/sequential/d_flip_flop.c core/sequential/jk_flip_flop.c \
            core/sequential/t_flip_flop.c \
            core/mux/mux2.c core/mux/mux4.c core/mux/demux.c \
            core/clock/clock.c core/clock/divider.c

CPU_SRCS = cpu/alu/adder.c cpu/alu/subtractor.c cpu/alu/multiplier.c \
           cpu/alu/divider.c cpu/alu/shifter.c cpu/alu/comparator.c \
           cpu/alu/logic_unit.c cpu/alu/alu.c \
           cpu/registers/register.c cpu/registers/register_file.c \
           cpu/registers/pc.c cpu/registers/ir.c \
           cpu/registers/mar.c cpu/registers/mdr.c cpu/registers/flags.c \
           cpu/control/decoder.c cpu/control/sequencer.c \
           cpu/control/control_unit.c cpu/control/microcode.c \
           cpu/pipeline/pipeline_reg.c cpu/pipeline/hazard_unit.c \
           cpu/pipeline/forwarding.c \
           cpu/interrupt/interrupt_controller.c cpu/interrupt/exception_handler.c

MEMORY_SRCS = memory/cell.c memory/sram.c memory/sram256.c memory/rom.c memory/rom256.c memory/dram.c \
              memory/cache/cache_line.c memory/cache/l1_cache.c memory/cache/replacement.c memory/cache/cache_controller.c memory/cache/l2_cache.c \
              memory/mmu/tlb.c memory/mmu/page_table.c memory/mmu/mmu.c

BUS_SRCS = bus/address_bus.c bus/data_bus.c bus/control_bus.c \
           bus/arbiter.c

IO_SRCS = io/io_controller.c io/port.c io/dma.c \
          io/devices/timer.c io/devices/gpio.c io/devices/seven_seg.c \
          io/devices/uart.c io/devices/display.c io/devices/pic.c \
          io/convert/bin_to_bcd.c io/convert/bcd_to_bin.c \
          io/convert/bcd_to_ascii.c io/convert/ascii_to_bcd.c \
          io/format/number_formatter.c io/format/number_parser.c

SYSTEM_SRCS = system/power.c system/config.c system/board.c system/board_pipe.c

SIMULATION_SRCS = simulation/scheduler.c \
                  simulation/timing.c simulation/waveform.c simulation/debugger.c \
                  simulation/simulator.c

ISA_SRCS = isa/encoding.c isa/assembler.c isa/disassembler.c

ALL_SRCS = $(CORE_SRCS) $(CPU_SRCS) $(MEMORY_SRCS) $(BUS_SRCS) $(IO_SRCS) \
           $(SYSTEM_SRCS) $(SIMULATION_SRCS) $(ISA_SRCS)

OBJS = $(ALL_SRCS:.c=.o)

TESTS = tests/test_gates tests/test_alu tests/test_arith tests/test_registers \
        tests/test_pipeline tests/test_interrupt tests/test_cosim \
        tests/test_timing tests/test_bcd tests/test_seven_seg \
        tests/test_format tests/test_uart tests/test_parser \
        tests/test_cache tests/test_mmu tests/test_dram tests/test_replacement tests/test_cache_ctrl tests/test_display tests/test_pic tests/test_pipeline_regression tests/test_l2 tests/test_halt tests/test_print_path tests/test_synth tests/test_mmio tests/test_jal tests/test_stack tests/test_dma tests/test_primitives tests/test_irq_e2e tests/test_vm tests/test_dcache

all: aletheia

aletheia: $(OBJS) main.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/%: tests/%.c tests/test.h tests/refmodel.h $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDFLAGS) -o $@

test: check-compliance $(TESTS)
	@echo "Running test suite..."
	@rc=0; for t in $(TESTS); do ./$$t || rc=1; done; \
	if [ $$rc -eq 0 ]; then echo "ALL TESTS PASSED"; else echo "TESTS FAILED"; fi; exit $$rc

test-san:
	@echo "Running tests under gcc UBSan (-fsanitize=undefined)..."
	@rc=0; for t in $(TESTS); do \
	  $(CC) -Wall -Wextra -Werror -I. -fsanitize=undefined -fno-sanitize-recover=all -g \
	    $$t.c $(ALL_SRCS) -o $$t.san 2>/dev/null && ./$$t.san >/dev/null 2>&1 || { echo "SAN FAIL: $$t"; rc=1; }; \
	  rm -f $$t.san; \
	done; \
	if [ $$rc -eq 0 ]; then echo "UBSAN CLEAN"; else echo "UBSAN ISSUES"; fi; exit $$rc

clean:
	rm -f $(OBJS) main.o aletheia $(TESTS) tests/*.san $(ALL_SRCS:.c=.d) $(TESTS:=.d) main.d

-include $(ALL_SRCS:.c=.d) main.d

check-compliance:
	@echo "Checking for forbidden constructs outside simulation/..."
	@! grep -rn --include="*.c" -E '\bif\s*\(' core/ cpu/ memory/ bus/ io/ system/ 2>/dev/null | grep -v "simulation/" || true
	@! grep -rn --include="*.c" -E '\bfor\s*\(' core/ cpu/ memory/ bus/ io/ system/ 2>/dev/null | grep -v "simulation/" || true
	@! grep -rn --include="*.c" -E '\bwhile\s*\(' core/ cpu/ memory/ bus/ io/ system/ 2>/dev/null | grep -v "simulation/" || true
	@! grep -rn --include="*.c" -E '\bswitch\s*\(' core/ cpu/ memory/ bus/ io/ system/ 2>/dev/null | grep -v "simulation/" || true
	@echo "Compliance check complete"

FORMAT_SRCS = $(filter-out core/% cpu/% memory/% bus/% io/% system/%,$(wildcard *.c */*.c */*.h))

format:
	@clang-format -i $(FORMAT_SRCS)

format-check:
	@clang-format --dry-run --Werror $(FORMAT_SRCS) && echo "format ok"

.PHONY: all clean check-compliance test test-san format format-check
