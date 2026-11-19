# Changelog

All notable changes to this project are recorded here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project
follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- The machine: a 32-bit MIPS-like computer built only from gates,
  multiplexers and flip-flops, with single-cycle, multi-cycle and pipelined
  cores co-simulated against a reference instruction-set simulator.
- Memory system shared by the three cores: SRAM of 256, 1024 or 4096 words
  (`make MEM=...`), a 1K-word ROM, DRAM with refresh and leakage, L1 and L2
  caches, and an MMU with a TLB and a page table.
- Kernel and user mode, with traps for protected pages, the MMIO page and
  `reti`.
- Devices at `0xFFFFFF00`: UART, GPIO, timer, PIC, display, DMA engine and an
  expansion slot.
- Boot ROM with a self-test and a monitor (`--boot`).
- `libaletheia`, with the `alt_` interface in `include/aletheia/aletheia.h`,
  a pkg-config file and an `install` target that also installs the CLI. The
  CLI uses only this interface.
- Fault injection: bit flips at a chosen cycle (`-f`), stuck-at gates
  (`--stuck`, in the `aletheia-fault` build) and bit-sliced campaigns of 63
  faulty machines (`build/lanes`).
- Physical attacks: cold boot (`--power-off`, `--dram-leak`), power traces
  (`--power`) and DMA injection through the slot (`--dma-inject`).
- Observability: VCD waveforms, breakpoints, a live front panel (`--panel`),
  snapshots (`--save`, `--load`) and a reference check (`--ref`).
- Demos: `counter`, `fib`, `sort`, `pin`, `dma`, `user`, `dmaattack` and
  `coldboot`; `make demos` requires the three cores to agree on each.
- `tools/check_hw.py`, which rejects control flow, arithmetic and comparison
  operators in the hardware directories.
- `make test-san`, which runs the suite under ASan and UBSan, and `make
  check`, which runs everything the CI runs.

[Unreleased]: https://github.com/augustoflorentino/aletheia/commits/main
