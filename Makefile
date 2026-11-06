CC = gcc
MEM ?= 256
MEM_FLAG_1024 = -DALETHEIA_MEM_1K
MEM_FLAG_4096 = -DALETHEIA_MEM_4K
CFLAGS = -std=c11 -D_DEFAULT_SOURCE -O2 -Wall -Wextra -Werror -MMD -MP -fPIC -fvisibility=hidden -I. $(MEM_FLAG_$(MEM))
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

MEMORY_SRCS = memory/cell.c memory/sram.c memory/sram256.c memory/sram1k.c memory/sram4k.c memory/rom.c memory/rom256.c memory/rom1k.c memory/dram.c \
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
                  simulation/simulator.c simulation/fault.c simulation/refcheck.c

REF_SRCS = ref/iss.c
ISA_SRCS = isa/optable.c isa/encoding.c isa/assembler.c isa/disassembler.c isa/asm_file.c
API_SRCS = api/aletheia.c
CLI_SRCS = cli/main.c cli/panel.c

ALL_SRCS = $(CORE_SRCS) $(CPU_SRCS) $(MEMORY_SRCS) $(BUS_SRCS) $(IO_SRCS) \
           $(SYSTEM_SRCS) $(SIMULATION_SRCS) $(ISA_SRCS) $(REF_SRCS) $(API_SRCS)

OBJS = $(patsubst %.c,build/obj$(MEM)/%.o,$(ALL_SRCS))
CLI_OBJS = $(patsubst %.c,build/obj$(MEM)/%.o,$(CLI_SRCS))

VERSION := $(shell sed -n 's/^\#define ALT_VERSION_\(MAJOR\|MINOR\|PATCH\) //p' include/aletheia/aletheia.h | paste -sd.)
MAJOR := $(firstword $(subst ., ,$(VERSION)))
LIBDIR = build/lib$(MEM)
STATIC_LIB = $(LIBDIR)/libaletheia.a
SHARED_LIB = $(LIBDIR)/libaletheia.so.$(VERSION)
PREFIX ?= /usr/local

NL_SRCS = $(filter-out core/gates/%,$(ALL_SRCS)) synth/netlist.c synth/gates_netlist.c
NL_OBJS = $(patsubst %.c,build/nl$(MEM)/%.o,$(NL_SRCS))
NL_TESTS = tests/test_synth tests/test_timing

TESTS = tests/test_gates tests/test_alu tests/test_arith tests/test_registers \
        tests/test_pipeline tests/test_interrupt tests/test_cosim \
        tests/test_timing \
        tests/test_uart \
        tests/test_cache tests/test_mmu tests/test_dram tests/test_replacement tests/test_cache_ctrl tests/test_display tests/test_pic tests/test_pipeline_regression tests/test_l2 tests/test_halt tests/test_synth tests/test_mmio tests/test_jal tests/test_stack tests/test_dma tests/test_primitives tests/test_irq_e2e tests/test_vm tests/test_dcache

all: aletheia

MEM_STAMP = build/mem.$(MEM)

$(MEM_STAMP):
	@mkdir -p build
	@rm -f build/mem.*
	@touch $@

aletheia: $(CLI_OBJS) $(STATIC_LIB) $(MEM_STAMP)
	$(CC) $(LDFLAGS) -o $@ $(CLI_OBJS) $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	rm -f $@
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(CC) -shared -Wl,-soname,libaletheia.so.$(MAJOR) $(LDFLAGS) -o $@ $^
	ln -sf libaletheia.so.$(VERSION) $(LIBDIR)/libaletheia.so.$(MAJOR)
	ln -sf libaletheia.so.$(MAJOR) $(LIBDIR)/libaletheia.so

lib: $(STATIC_LIB) $(SHARED_LIB)

check-api: $(SHARED_LIB) $(CLI_OBJS)
	@bad=$$(nm -D --defined-only $(SHARED_LIB) | awk '{print $$3}' | grep -v '^alt_' || true); \
	if [ -n "$$bad" ]; then echo "exported outside alt_: $$bad"; exit 1; fi
	@$(CC) -o build/cli-shared $(CLI_OBJS) -L$(LIBDIR) -l:libaletheia.so.$(VERSION) && rm -f build/cli-shared
	@echo "api ok: $$(nm -D --defined-only $(SHARED_LIB) | grep -c ' T alt_') functions, cli uses only the public API"

install: lib
	install -d $(DESTDIR)$(PREFIX)/include/aletheia $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -m 644 include/aletheia/aletheia.h $(DESTDIR)$(PREFIX)/include/aletheia/
	install -m 644 $(STATIC_LIB) $(DESTDIR)$(PREFIX)/lib/
	install -m 755 $(SHARED_LIB) $(DESTDIR)$(PREFIX)/lib/
	ln -sf libaletheia.so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/libaletheia.so.$(MAJOR)
	ln -sf libaletheia.so.$(MAJOR) $(DESTDIR)$(PREFIX)/lib/libaletheia.so
	sed -e 's|@PREFIX@|$(PREFIX)|' -e 's|@VERSION@|$(VERSION)|' aletheia.pc.in > $(DESTDIR)$(PREFIX)/lib/pkgconfig/aletheia.pc

build/obj$(MEM)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/tests$(MEM)/%: tests/%.c tests/test.h tests/refmodel.h ref/iss.h $(OBJS)
	@mkdir -p build/tests$(MEM)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDFLAGS) -o $@

build/nl$(MEM)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DALETHEIA_NETLIST -c $< -o $@

$(patsubst tests/%,build/tests$(MEM)/%,$(NL_TESTS)): build/tests$(MEM)/%: tests/%.c tests/test.h $(NL_OBJS)
	@mkdir -p build/tests$(MEM)
	$(CC) $(CFLAGS) -DALETHEIA_NETLIST $< $(NL_OBJS) $(LDFLAGS) -o $@

TEST_BINS = $(patsubst tests/%,build/tests$(MEM)/%,$(TESTS))

test: check-compliance $(TEST_BINS)
	@echo "Running test suite..."
	@rc=0; for t in $(TEST_BINS); do ./$$t || rc=1; done; \
	if [ $$rc -eq 0 ]; then echo "ALL TESTS PASSED"; else echo "TESTS FAILED"; fi; exit $$rc

test-san:
	@echo "Running tests under gcc UBSan (-fsanitize=undefined)..."
	@mkdir -p build; rc=0; for t in $(TESTS); do \
	  case " $(NL_TESTS) " in *" $$t "*) src="-DALETHEIA_NETLIST $(NL_SRCS)";; *) src="$(ALL_SRCS)";; esac; \
	  $(CC) -std=c11 -D_DEFAULT_SOURCE -Wall -Wextra -Werror -I. $(MEM_FLAG_$(MEM)) -fsanitize=undefined -fno-sanitize-recover=all -g \
	    $$t.c $$src -o build/san.bin 2>/dev/null && ./build/san.bin >/dev/null 2>&1 || { echo "SAN FAIL: $$t"; rc=1; }; \
	  rm -f build/san.bin; \
	done; \
	if [ $$rc -eq 0 ]; then echo "UBSAN CLEAN"; else echo "UBSAN ISSUES"; fi; exit $$rc

clean:
	rm -rf build aletheia

-include $(OBJS:.o=.d) $(CLI_OBJS:.o=.d) $(NL_OBJS:.o=.d) $(TEST_BINS:=.d) build/bench.d build/bench-gates.d

check-compliance:
	@python3 tools/check_hw.py

DEMOS = demos/counter.asm demos/fib.asm demos/sort.asm demos/pin.asm demos/dma.asm demos/user.asm demos/dmaattack.asm

demos: aletheia
	@mkdir -p build
	@rc=0; for d in $(DEMOS); do \
	  for a in single multi pipe; do \
	    ./aletheia -m $$a -a $$d -i 4719 -c 200000 > build/demo.$$a 2>&1; echo "exit $$?" >> build/demo.$$a; \
	    sed -n '/^Starting simulation/,/^Simulation finished/p' build/demo.$$a | sed '1d;$$d' > build/demo.$$a.uart; \
	    tail -1 build/demo.$$a >> build/demo.$$a.uart; \
	  done; \
	  if cmp -s build/demo.single.uart build/demo.multi.uart && cmp -s build/demo.single.uart build/demo.pipe.uart \
	     && grep -q 'exit 0' build/demo.single.uart; then echo "demo ok: $$d"; else echo "DEMO FAIL: $$d"; rc=1; fi; \
	done; rm -f build/demo.*; exit $$rc

BENCH_CYCLES ?= 5000

build/bench: tools/bench.c $(OBJS) $(MEM_STAMP)
	@mkdir -p build
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

build/bench-gates: tools/bench.c $(NL_OBJS) $(MEM_STAMP)
	@mkdir -p build
	$(CC) $(CFLAGS) -DALETHEIA_NETLIST $< $(NL_OBJS) -o $@

bench: build/bench build/bench-gates aletheia
	@./build/bench-gates
	@./build/bench $(BENCH_CYCLES)
	@printf "%-18s %8s %8s %8s %7s %7s\n" program single multi pipe CPI-mc CPI-pp
	@for d in demos/fib.asm demos/sort.asm demos/pin.asm; do \
	  s=$$(./aletheia -m single -a $$d -i 4719 -c 200000 | sed -n 's/^Cycles executed: //p'); \
	  m=$$(./aletheia -m multi -a $$d -i 4719 -c 200000 | sed -n 's/^Cycles executed: //p'); \
	  p=$$(./aletheia -m pipe -a $$d -i 4719 -c 200000 | sed -n 's/^Cycles executed: //p'); \
	  awk -v d=$$d -v s=$$s -v m=$$m -v p=$$p 'BEGIN { printf "%-18s %8d %8d %8d %7.2f %7.2f\n", d, s, m, p, m/s, p/s }'; \
	done

FORMAT_SRCS = $(filter-out core/% cpu/% memory/% bus/% io/% system/%,$(wildcard *.c */*.c */*.h tests/*.h include/aletheia/*.h))

format:
	@clang-format -i $(FORMAT_SRCS)

format-check:
	@clang-format --dry-run --Werror $(FORMAT_SRCS) && echo "format ok"

.PHONY: all lib check-api install clean check-compliance test test-san bench demos format format-check
