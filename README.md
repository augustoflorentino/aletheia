# Aletheia

*A 32-bit computer, simulated at the logic-gate level, in C — no `if` or `for`
in the hardware, only gates, multiplexers and flip-flops.*

## Why this exists

This started with number theory. Some questions in my research kept circling
back to computation itself — to what a machine actually *does* with a number,
and to the von Neumann architecture I had used for years without ever looking
inside. I could have read my way to an answer. Instead I decided to rebuild the
thing in practice, and the smallest honest starting point was a simulator that
could multiply.

At first it was simple. But every answer opened another question, and the ones
that would not let go were about what I came to think of as *where data lives* —
the hiding place of information. Chasing that meant adding pieces: a register to
hold a value for one cycle, memory to hold it longer, addressing to find it
again, a cache to let it hide somewhere fast, an MMU to let one address quietly
become another. By the time I noticed, I was one component short of a whole
computer. Having come that far, finishing it was the only reasonable thing left
to do.

It was never meant to be the best, the most professional, or the solution to
anyone's problem. It came out of plain curiosity about logic gates, and it
stayed that way.

## The one rule

Use nothing the hardware itself would not have on its way down to silicon. The
hardware (`core/`, `cpu/`, `memory/`, `bus/`, `io/`, `system/`) is written in
pure C with zero libraries, not even the C standard library: only gates, wires,
and the structures they form. Forbidden inside the hardware: `if`, `for`,
`while`, `switch`, arithmetic and comparison operators. A conditional is a
multiplexer, a comparison is a comparator circuit, a loop is unrolled or made
sequential. Even the machine's own decisions (wait for power, hold for DMA)
come out of its signals, never out of C. `make check-compliance` enforces it.

Everything around the hardware (the assembler, the simulator, the library
API, the CLI and the tests) is ordinary C11 with the standard library. That is
the bench, not the machine.

## What it actually is

A working machine, not a diagram of one. It powers on, runs a self-test,
fetches real instructions from ROM, decodes them, drives the ALU, touches
memory and devices, and writes back, all propagating through gates.

- **ISA**: 32-bit, MIPS-like, 30 instructions: arithmetic and logic (`add`,
  `sub`, `and`, `or`, `xor`, `nor`, `slt[u]`, immediates, `lui`), shifts
  (`sll`, `srl`, `sra` and variable forms), `lw`/`sw`, `beq`/`bne`, `j`/`jal`/
  `jr`/`jalr`, `reti` and `halt`.
- **Three microarchitectures** running the same ISA: **single-cycle**,
  **multi-cycle** (FSM with IR/MAR/MDR) and **pipelined** (5 stages, forwarding,
  load-use stall, branch flush). All three are co-simulated against a reference
  instruction-set simulator (`ref/iss.c`).
- **Power and reset**: a power controller holds the machine in reset until the
  supply is stable; jumpers sampled at reset turn the L1/L2 caches and the MMU
  on or off and set the timer prescaler.
- **Memory**: SRAM of 256, 1024 or 4096 words (`make MEM=...`), a 1K-word ROM,
  a DRAM that needs refresh and leaks when it does not get it, L1 and L2 caches,
  and an MMU (TLB plus a 16-entry page table with a user bit).
- **Protection**: kernel and user mode. In user mode the MMU is forced on,
  pages without the user bit, the MMIO page and `reti` all trap.
- **Exceptions and interrupts**: undefined instruction, address error and IRQ,
  each with its own vector; EPC is saved and `reti` returns. A timer raises the
  IRQ through a PIC with a mask register.
- **I/O**, all memory-mapped at `0xFFFFFF00`: UART, GPIO, timer, PIC, an
  8-character display and a DMA engine.
- **Firmware**: a boot ROM (`firmware/boot.asm`) that sizes and tests RAM,
  checks the timer and the UART, and drops into a monitor with `m` (dump),
  `w` (write) and `g` (go).
- **Observability**: VCD waveforms (`-w`), breakpoints (`-b`), a live front
  panel (`--panel`), bit flips in the PC, a register or RAM at a chosen cycle
  (`-f`), and a power cut that shows what the DRAM kept (`--power-off`,
  `demos/coldboot.asm`).

## Build & run

```sh
make                       # builds ./aletheia (MEM=256 by default)
make test                  # compliance check + full test suite
make demos                 # every demo on the three cores, outputs must match
./aletheia -a demos/fib.asm -c 200000
./aletheia --boot          # power on into the boot ROM and the monitor
./aletheia --panel -a demos/sort.asm -m pipe
./aletheia --help          # every option
```

Demos in `demos/`: `counter` (timer interrupts), `fib`, `sort`, `pin` (reads
a PIN over the UART), `dma`, `user` (user mode and its traps) and `coldboot`.

Other targets: `make bench`, `make test-san` (UBSan), `make format`.

## Library

```sh
make lib                   # build/lib256/libaletheia.a and .so
make install PREFIX=/usr/local
cc app.c $(pkg-config --cflags --libs aletheia)
```

The interface is `include/aletheia/aletheia.h`, every name prefixed `alt_`. A
machine is an opaque `alt_machine_t` created with an optional allocator; you
load a ROM, power it on, then `alt_step` or `alt_run` it and read its state
(PC, registers, RAM, display, pipeline stages, UART output). Faults, power
cuts and VCD traces are part of the API. Each machine is independent, so one
process can run several. The CLI in `cli/` uses only this interface, and
`make check-api` verifies it.

---

*P.S. — The hardware turned out well. I never did find what I came looking for.
Life is sad.*
