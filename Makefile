CC = gcc
CFLAGS = -std=c11 -D_DEFAULT_SOURCE -O2 -Wall -Wextra -Werror -MMD -MP -I.
LDFLAGS =

CORE_SRCS = core/gates/and.c core/gates/or.c core/gates/not.c core/gates/xor.c \
            core/gates/nand.c core/gates/nor.c core/gates/select.c \
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
           cpu/control/decoder.c \
           cpu/pipeline/pipeline_reg.c cpu/pipeline/hazard_unit.c \
           cpu/pipeline/forwarding.c \
           cpu/interrupt/interrupt_controller.c cpu/interrupt/exception_handler.c

MEMORY_SRCS = memory/cell.c memory/sram.c memory/sram256.c memory/rom.c memory/rom256.c memory/rom1k.c memory/dram.c \
              memory/cache/cache_line.c memory/cache/l1_cache.c memory/cache/replacement.c memory/cache/cache_controller.c memory/cache/l2_cache.c \
              memory/mmu/tlb.c memory/mmu/page_table.c memory/mmu/mmu.c

BUS_SRCS = bus/address_bus.c bus/data_bus.c bus/control_bus.c \
           bus/arbiter.c

IO_SRCS = io/dma.c \
          io/devices/timer.c io/devices/gpio.c \
          io/devices/uart.c io/devices/display.c io/devices/pic.c

SYSTEM_SRCS = system/power.c system/config.c system/memsys.c system/board.c system/board_pipe.c

SIMULATION_SRCS = simulation/scheduler.c \
                  simulation/timing.c simulation/waveform.c simulation/debugger.c \
                  simulation/simulator.c simulation/fault.c

REF_SRCS = ref/iss.c
ISA_SRCS = isa/optable.c isa/encoding.c isa/assembler.c isa/disassembler.c isa/asm_file.c

ALL_SRCS = $(CORE_SRCS) $(CPU_SRCS) $(MEMORY_SRCS) $(BUS_SRCS) $(IO_SRCS) \
           $(SYSTEM_SRCS) $(SIMULATION_SRCS) $(ISA_SRCS) $(REF_SRCS)

OBJS = $(patsubst %.c,build/obj/%.o,$(ALL_SRCS))
MAIN_OBJ = build/obj/main.o

NL_SRCS = $(filter-out core/gates/%,$(ALL_SRCS)) synth/netlist.c synth/gates_netlist.c
NL_OBJS = $(patsubst %.c,build/nl/%.o,$(NL_SRCS))
NL_TESTS = tests/test_synth tests/test_timing

TESTS = tests/test_gates tests/test_alu tests/test_arith tests/test_registers \
        tests/test_pipeline tests/test_interrupt tests/test_cosim \
        tests/test_timing \
        tests/test_uart \
        tests/test_cache tests/test_mmu tests/test_dram tests/test_replacement tests/test_cache_ctrl tests/test_display tests/test_pic tests/test_pipeline_regression tests/test_l2 tests/test_halt tests/test_synth tests/test_mmio tests/test_jal tests/test_stack tests/test_dma tests/test_primitives tests/test_irq_e2e tests/test_vm tests/test_dcache

all: aletheia

aletheia: $(MAIN_OBJ) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(MAIN_OBJ) $(OBJS)

build/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/tests/%: tests/%.c tests/test.h tests/refmodel.h ref/iss.h $(OBJS)
	@mkdir -p build/tests
	$(CC) $(CFLAGS) $< $(OBJS) $(LDFLAGS) -o $@

build/nl/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DALETHEIA_NETLIST -c $< -o $@

$(patsubst tests/%,build/tests/%,$(NL_TESTS)): build/tests/%: tests/%.c tests/test.h $(NL_OBJS)
	@mkdir -p build/tests
	$(CC) $(CFLAGS) -DALETHEIA_NETLIST $< $(NL_OBJS) $(LDFLAGS) -o $@

TEST_BINS = $(patsubst tests/%,build/tests/%,$(TESTS))

test: check-compliance $(TEST_BINS)
	@echo "Running test suite..."
	@rc=0; for t in $(TEST_BINS); do ./$$t || rc=1; done; \
	if [ $$rc -eq 0 ]; then echo "ALL TESTS PASSED"; else echo "TESTS FAILED"; fi; exit $$rc

test-san:
	@echo "Running tests under gcc UBSan (-fsanitize=undefined)..."
	@mkdir -p build; rc=0; for t in $(TESTS); do \
	  case " $(NL_TESTS) " in *" $$t "*) src="-DALETHEIA_NETLIST $(NL_SRCS)";; *) src="$(ALL_SRCS)";; esac; \
	  $(CC) -std=c11 -D_DEFAULT_SOURCE -Wall -Wextra -Werror -I. -fsanitize=undefined -fno-sanitize-recover=all -g \
	    $$t.c $$src -o build/san.bin 2>/dev/null && ./build/san.bin >/dev/null 2>&1 || { echo "SAN FAIL: $$t"; rc=1; }; \
	  rm -f build/san.bin; \
	done; \
	if [ $$rc -eq 0 ]; then echo "UBSAN CLEAN"; else echo "UBSAN ISSUES"; fi; exit $$rc

clean:
	rm -rf build aletheia

-include $(OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(NL_OBJS:.o=.d) $(TEST_BINS:=.d)

check-compliance:
	@python3 tools/check_hw.py

FORMAT_SRCS = $(filter-out core/% cpu/% memory/% bus/% io/% system/%,$(wildcard *.c */*.c */*.h tests/*.h include/aletheia/*.h))

format:
	@clang-format -i $(FORMAT_SRCS)

format-check:
	@clang-format --dry-run --Werror $(FORMAT_SRCS) && echo "format ok"

.PHONY: all clean check-compliance test test-san format format-check
