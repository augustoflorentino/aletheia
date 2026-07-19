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

Use nothing the hardware itself would not have on its way down to silicon. So
the hardware core — and the ISA tooling — is written in pure C with zero
libraries, not even the C standard library: only gates, wires, and the
structures they form. Libraries appear in exactly one place, the testbench and
CLI, which use `stdio` to print results and drive the tests — the honest
boundary between the machine and the bench that measures it. Everything inside
that boundary is built from primitives, so the flow of information stays
faithful to what a real circuit would do. Forbidden inside the hardware:
`if`, `for`, `while`, `switch`, arithmetic and comparison operators — a
conditional is a multiplexer, a comparison is a comparator circuit, a loop is
unrolled or made sequential. `make check-compliance` enforces it.

## What it actually is

A working machine, not a diagram of one. It fetches real instructions from ROM,
decodes them, reads registers, drives the ALU, touches memory, and writes back —
all propagating through gates.

- **ISA**: 32-bit, MIPS-like (R/I/J types, `jal`/`jr`, memory-mapped I/O,
  interrupts).
- **Three microarchitectures** executing the same ISA, all co-simulated against
  a pure-C golden model: **single-cycle**, **multi-cycle** (FSM with IR/MAR/MDR),
  and **pipelined** (5 stages with forwarding, load-use stall, branch flush).
- **Memory hierarchy**: SRAM and ROM (256 words, hierarchical banks), DRAM with
  a real refresh model, L1/L2 caches, and an MMU (TLB + two-level page table).
- **Interrupts & exceptions** wired into the datapath: a hardware timer raises an
  IRQ through the PIC, the CPU saves EPC/cause, vectors to a handler, and `reti`
  returns.
- **I/O**: UART, GPIO, timer, an 8-char display and a seven-segment decoder, all
  memory-mapped; plus numeric I/O built in gates (binary↔BCD via double dabble,
  a signed formatter, a parser).
- **Verification**: ~200k assertions across directed vectors and randomized
  fuzz, golden-model co-simulation of the whole datapath, static timing analysis
  over the real netlists (critical path, F_max, glitch detection), and
  technology mapping to NAND2 for area/timing — everything in-repo, UBSan-clean,
  zero third-party dependencies.

## Build & run

```sh
make                       # builds ./aletheia
make test                  # compliance check + full test suite
./aletheia -a demos/counter.asm -c 3000
```

The demo is a timer-interrupt-driven counter: the timer fires, a handler
increments a value and re-arms the device, and the main loop prints each digit
over the (gate-level) UART and to the display — the whole path in gates.

---

*P.S. — The hardware turned out well. I never did find what I came looking for.
Life is sad.*
