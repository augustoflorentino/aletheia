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
  8-character display, a DMA engine and an expansion slot that can drive it.
- **Firmware**: a boot ROM (`firmware/boot.asm`) that sizes and tests RAM,
  checks the timer and the UART, and drops into a monitor with `m` (dump),
  `w` (write) and `g` (go).
- **Size**: about 500,000 NAND2 gates are evaluated per cycle, at roughly
  0.6 ms per cycle on one core of a desktop CPU.

## How it grew

Finishing the computer did not end the curiosity, it moved it. *Where data
lives* turned into *how data leaks*: if every bit has a physical home, what
happens when the power is cut, a gate is broken, a device reads memory behind
the CPU's back, or someone watches how much the circuit switches? Answering
that changed what the project is for, from a computer you can read to a
hardware security lab you can break on purpose, with nothing hidden from you.

What was added along the way:

- **Capacity**: memory went from 256 words to selectable 256/1K/4K, the ROM to
  1K words, and the MMIO page moved to the top of the address space.
- **Fault injection**:
  - flip any bit of the PC, a register or RAM at a chosen cycle (`-f`);
  - tie one NAND gate to 0 or 1 for the whole run (`--stuck`, in the
    `aletheia-fault` build);
  - run 63 machines at once, each with its own stuck gate, by bit-slicing
    (`build/lanes`): every wire becomes a 64-bit word, one lane per machine.
- **Physical attacks**:
  - cold boot: cut the power for N cycles and see what the DRAM kept
    (`--power-off`, `--dram-leak`, `demos/coldboot.asm`);
  - power analysis: count the state bits that toggle every cycle
    (`--power FILE`), against a PIN check that is easy to attack
    (`demos/pin.asm`);
  - DMA attack: a card on the expansion slot copies kernel memory past the MMU
    (`--dma-inject`, `demos/dmaattack.asm`).
- **Observability**: VCD waveforms with the bus, the pipeline and every
  register (`-w`), breakpoints, a live front panel (`--panel`), snapshots you
  can save and resume from (`--save`, `--load`), and `--ref` to compare a run
  against the reference model (it exits with 4 on any difference).
- **A library**: the whole machine is available as `libaletheia`, so other
  programs can drive it (see below).

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
a PIN over the UART), `dma`, `user` (user mode and its traps), `dmaattack` and
`coldboot`.

Other targets: `make aletheia-fault` (stuck-at gates), `make build/lanes`
(bit-sliced fault campaigns), `make bench`, `make test-san` (UBSan),
`make format`.

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
cuts, DMA injection, VCD and power traces, snapshots in memory or on disk, and
the reference check are all part of the API. Each machine is independent, so
one process can run several. The CLI in `cli/` uses only this interface, and
`make check-api` verifies it. The full reference is in [docs/api.md](docs/api.md).

---

*P.S. — The hardware turned out well. I never did find what I came looking for.
Life is sad.*
