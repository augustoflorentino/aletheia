# libaletheia API

The whole machine as a C library: create a computer, load a program, power it
on, run it cycle by cycle, look inside, and break it on purpose.

```c
#include <aletheia/aletheia.h>
```

```sh
make lib                          # build/lib256/libaletheia.a and libaletheia.so.0.9.0
make install PREFIX=/usr/local    # header, both libraries, aletheia.pc
cc app.c $(pkg-config --cflags --libs aletheia)
```

The header compiles as C99 and C++. Every public name starts with `alt_`
(macros with `ALT_`); nothing else is exported from the shared library
(`make check-api` checks it).

## A first program

```c
#include <aletheia/aletheia.h>
#include <stdio.h>

static void print_byte(void *ctx, int byte) {
    (void)ctx;
    putchar(byte);
}

int main(void) {
    uint32_t rom[ALT_ROM_WORDS];
    size_t count;
    char error[256];
    if (alt_assemble_file("demos/fib.asm", rom, ALT_ROM_WORDS, &count, error, sizeof error) != ALT_OK) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    alt_machine_t *m;
    if (alt_machine_create(NULL, &m) != ALT_OK) return 1;
    alt_set_arch(m, ALT_ARCH_PIPE);
    alt_load_rom(m, rom, count);
    alt_set_output(m, print_byte, NULL);

    alt_power_on(m);
    alt_stop_t stop = alt_run(m, 200000);

    printf("\nstop %d after %llu cycles, $t0 = %u\n", stop,
           (unsigned long long)alt_cycles(m), alt_reg(m, 8));
    alt_machine_destroy(m);
    return 0;
}
```

## Concepts

**Machine.** `alt_machine_t` is opaque. Each one is a complete, independent
computer: CPU, memory, caches, MMU, devices and the console attached to its
UART. A process can hold as many as memory allows.

**Cycles.** Time is counted in clock cycles of the machine. `alt_step` runs
exactly one; `alt_run` runs until a limit, a `halt` or a breakpoint.

**Power.** A new machine is unpowered. Configure it, load the ROM, then call
`alt_power_on`. After power is applied the machine stays in reset for a few
cycles (7 with the default board) before it fetches the first instruction.

**Addresses.** Addresses are word addresses: the PC, breakpoints, RAM indexes
and ROM indexes all count 32-bit words, not bytes.

**Console.** Every machine has a terminal wired to its UART. Bytes the program
sends arrive at the output callback; bytes given to `alt_send_key` are shifted
in serially, one at a time, as the program is ready to read them.

**Errors.** Functions that can fail return `alt_status_t`; `ALT_OK` is zero.
Detail, when there is any, comes separately (an error buffer, a log).

**Ownership.** What `alt_machine_create` creates, only `alt_machine_destroy`
frees. No function takes ownership of its arguments. Pointers returned by the
library (`alt_output`, `alt_version`, `alt_status_name`) belong to it and stay
valid while the machine (or the program, for static strings) lives.

**Threads.** Different machines can run in different threads. One machine must
not be used from two threads at once. The exception is the `aletheia-fault`
build, where the stuck gate is process-wide (see `alt_stuck_gate`).

**Build variants.** The library is built for one memory size (`make MEM=256`,
`1024` or `4096`); `alt_ram_words()` says which. Snapshots only load into a
library built with the same size.

## Types and constants

| Name | Meaning |
|---|---|
| `alt_machine_t` | opaque machine |
| `alt_status_t` | `ALT_OK`, `ALT_ERR_NOMEM`, `ALT_ERR_ARG`, `ALT_ERR_IO`, `ALT_ERR_FORMAT`, `ALT_ERR_FULL`, `ALT_ERR_UNSUPPORTED` |
| `alt_arch_t` | `ALT_ARCH_SINGLE`, `ALT_ARCH_MULTI`, `ALT_ARCH_PIPE` |
| `alt_stop_t` | why `alt_run` returned: `ALT_STOP_LIMIT`, `ALT_STOP_HALT`, `ALT_STOP_BREAK` |
| `alt_fault_target_t` | `ALT_FAULT_PC`, `ALT_FAULT_REG`, `ALT_FAULT_RAM` |
| `alt_stage_t` | pipeline latch: `ALT_STAGE_ID`, `ALT_STAGE_EX`, `ALT_STAGE_MEM`, `ALT_STAGE_WB` |
| `ALT_STRAP_L1`, `ALT_STRAP_L2`, `ALT_STRAP_MMU` | jumper bits; `ALT_STRAP_DEFAULT` is L1 + MMU |
| `ALT_SIGNAL_IRQ`, `ALT_SIGNAL_EXCEPTION`, `ALT_SIGNAL_MMU`, `ALT_SIGNAL_USER` | bits returned by `alt_signals` |
| `ALT_ROM_WORDS` | 1024, size of the ROM |
| `ALT_DISPLAY_CHARS` | 8, characters on the display |
| `ALT_OUTPUT_MAX` | 4096, UART bytes kept by the machine |
| `ALT_VERSION_MAJOR`, `_MINOR`, `_PATCH` | version of the header |

```c
typedef struct {
    void *(*alloc)(void *ctx, size_t size);
    void (*free)(void *ctx, void *ptr);
    void *ctx;
} alt_allocator_t;

typedef void (*alt_output_fn)(void *ctx, int byte);
typedef void (*alt_cycle_fn)(void *ctx, alt_machine_t *machine);
```

## Reference

### Library

`const char *alt_version(void)`
: Version of the compiled library, `"MAJOR.MINOR.PATCH"`.

`const char *alt_status_name(alt_status_t status)`
: Short English text for a status, for logs.

### Lifecycle

`alt_status_t alt_machine_create(const alt_allocator_t *allocator, alt_machine_t **out_machine)`
: Creates an unpowered machine: single-cycle core, default jumpers, timer
  divider 1, DRAM leak period 64, no clock limit, empty ROM. `allocator` may be
  `NULL` for `malloc`/`free`; if given, both functions are required and every
  allocation of this machine goes through them. Returns `ALT_ERR_NOMEM` or
  `ALT_ERR_ARG`; `*out_machine` is `NULL` on failure.

`void alt_machine_destroy(alt_machine_t *machine)`
: Closes any open VCD or power trace and frees the machine. Accepts `NULL`.

### Configuration

Call these before `alt_power_on` unless noted.

`alt_status_t alt_set_arch(alt_machine_t *machine, alt_arch_t arch)`
: Chooses the core. `ALT_ERR_ARG` for an unknown value.

`void alt_set_straps(alt_machine_t *machine, unsigned straps)`
: Sets the L1, L2 and MMU jumpers (`ALT_STRAP_*`). With the MMU jumper off,
  writes to the MMU enable register are ignored.

`alt_status_t alt_set_timer_div(alt_machine_t *machine, unsigned div)`
: Timer prescaler: 1, 2, 4 or 8. `ALT_ERR_ARG` otherwise.

`void alt_set_dram_leak(alt_machine_t *machine, unsigned long period)`
: Every `period` cycles the DRAM loses charge; cells that were not refreshed
  in time decay. 0 disables leaking.

`void alt_set_hz(alt_machine_t *machine, long hz)`
: Limits the machine to `hz` cycles per second of real time (the library
  sleeps). 0 runs as fast as possible. Can change at any time.

`void alt_set_output(alt_machine_t *machine, alt_output_fn fn, void *ctx)`
: Called with every byte the program sends over the UART. `NULL` stops the
  calls; the bytes are still kept for `alt_output`.

`void alt_set_cycle_hook(alt_machine_t *machine, alt_cycle_fn fn, void *ctx)`
: Called after every cycle run by `alt_step` or `alt_run`, with the machine.
  The hook may read state, write RAM, send keys or save snapshots. `NULL`
  removes it.

### Programs

`alt_status_t alt_assemble_file(const char *path, uint32_t *words, size_t words_len, size_t *out_count, char *out_error, size_t out_error_len)`
: Assembles a source file into `words` (at most `words_len`, at most
  `ALT_ROM_WORDS`) and stores the number of words in `*out_count`. On a syntax
  error returns `ALT_ERR_FORMAT` and writes `file:line: error: message` to
  `out_error` (may be `NULL`).

`alt_status_t alt_disassemble(uint32_t word, char *out_text, size_t out_text_len)`
: One instruction to text, e.g. `add $t2, $t0, $t1`.

`alt_status_t alt_load_rom(alt_machine_t *machine, const uint32_t *words, size_t words_len)`
: Writes the ROM from address 0 and clears the rest. At most `ALT_ROM_WORDS`.

### Input and attacks

`alt_status_t alt_send_key(alt_machine_t *machine, unsigned char key)`
: Queues a byte for the UART (256 bytes of queue). `ALT_ERR_FULL` when the
  queue is full. The first 255 keys are also remembered for `alt_ref_check`.

`alt_status_t alt_add_fault(alt_machine_t *machine, uint64_t cycle, alt_fault_target_t target, unsigned index, unsigned bit)`
: Flips one flip-flop just before `cycle`: a PC bit, bit `bit` of register
  `index`, or bit `bit` of RAM word `index`. Up to 16 faults per machine
  (`ALT_ERR_FULL`).

`alt_status_t alt_add_fault_spec(alt_machine_t *machine, const char *spec)`
: Same, from text: `CYCLE:pc:BIT`, `CYCLE:rN:BIT` or `CYCLE:ramADDR:BIT`
  (ADDR in hex). `ALT_ERR_FORMAT` if it does not parse.

`alt_status_t alt_stuck_gate(alt_machine_t *machine, long gate, int value)`
: Ties NAND gate number `gate` of every core step to `value` (0 or 1). Only
  available when the program links the `aletheia-fault` gate set; the normal
  library returns `ALT_ERR_UNSUPPORTED`. In that build the stuck gate applies
  to every machine of the process.

`unsigned long alt_gates_per_step(const alt_machine_t *machine)`
: NAND gates evaluated in the last core step, in the `aletheia-fault` build;
  0 in the normal library. Gate numbers for `alt_stuck_gate` go from 0 to this
  value minus one.

`void alt_power_cut(alt_machine_t *machine, uint64_t at_cycle, unsigned long off_cycles)`
: When the machine reaches `at_cycle`, cuts the power for `off_cycles` cycles
  and powers it on again. SRAM, registers and caches are lost; the DRAM keeps
  whatever survives the leak (cold boot). `off_cycles` 0 disables it.

`void alt_slot_dma(alt_machine_t *machine, uint64_t at_cycle, uint32_t src, uint32_t dst, uint32_t count)`
: From `at_cycle`, a card on the expansion slot programs the DMA to copy
  `count` words from `src` to `dst`. The DMA uses physical addresses, so it
  goes past the MMU and user/kernel protection. `at_cycle` 0 disables it.

`alt_status_t alt_ram_write(alt_machine_t *machine, uint32_t addr, uint32_t word)`
: Writes a RAM word between cycles, through the memory's write port, and
  updates the L1 and L2 lines that hold it, so the program sees the new value.
  `ALT_ERR_ARG` past the end of RAM.

### Debugging and recording

`alt_status_t alt_break_add(alt_machine_t *machine, uint32_t pc)`
: `alt_run` stops before executing `pc` (up to 32 breakpoints). Calling
  `alt_run` again continues from there.

`alt_status_t alt_vcd_open(alt_machine_t *machine, const char *path)`
: Records a VCD waveform from now on: clock, halt, PC, current instruction,
  IRQ, exception, MMU and user mode, the bus (address, data, read/write,
  store, DMA hold), every register and, on the pipelined core, the pipeline
  latches. Open it after `alt_set_arch`. Readable with GTKWave.

`void alt_vcd_close(alt_machine_t *machine)`
: Stops recording and closes the file.

`alt_status_t alt_power_trace_open(alt_machine_t *machine, const char *path)`
: Writes `cycle,toggles` to a CSV, where toggles is how many bits of the
  machine's state changed in that cycle: a rough model of power consumption.

`void alt_power_trace_close(alt_machine_t *machine)`
: Closes the trace.

### Running

`void alt_power_on(alt_machine_t *machine)`
: Applies power. The board runs its reset sequence and then starts at
  address 0.

`void alt_step(alt_machine_t *machine)`
: Runs one cycle, then calls the cycle hook.

`alt_stop_t alt_run(alt_machine_t *machine, uint64_t max_cycles)`
: Runs up to `max_cycles` more cycles. Returns `ALT_STOP_HALT` when the
  program executes `halt`, `ALT_STOP_BREAK` at a breakpoint, `ALT_STOP_LIMIT`
  otherwise. After a halt it runs 12 more cycles so the last UART byte
  reaches the console; they are counted in `alt_cycles`.

### State

Reading state does not advance time.

`uint64_t alt_cycles(const alt_machine_t *machine)`
: Cycles run since creation (or since the snapshot that was loaded).

`int alt_halted(const alt_machine_t *machine)`
: Non-zero after `halt`.

`alt_arch_t alt_arch(const alt_machine_t *machine)`, `long alt_hz(const alt_machine_t *machine)`
: Current core and clock limit.

`uint32_t alt_pc(alt_machine_t *machine)`
: Program counter (word address).

`uint32_t alt_reg(alt_machine_t *machine, unsigned index)`
: Register 0 to 31 (`$zero` .. `$ra`). 0 for an index above 31.

`alt_status_t alt_ram_read(alt_machine_t *machine, uint32_t addr, uint32_t *out_word)`
: Reads a word straight from the SRAM. The caches are write-through, so this
  is always the current value.

`size_t alt_ram_words(void)`
: RAM size of this build: 256, 1024 or 4096 words.

`unsigned char alt_display_char(alt_machine_t *machine, unsigned index)`
: Byte shown at position 0 to 7 of the display.

`unsigned alt_signals(const alt_machine_t *machine)`
: `ALT_SIGNAL_IRQ` (interrupt request pending), `ALT_SIGNAL_EXCEPTION` (inside
  a handler), `ALT_SIGNAL_MMU` (translation enabled), `ALT_SIGNAL_USER` (user
  mode).

`alt_status_t alt_stage(const alt_machine_t *machine, alt_stage_t stage, uint32_t *out_value, int *out_valid)`
: Contents of a pipeline latch and whether it holds a valid instruction:
  `ID` gives the instruction word, `EX` its PC, `MEM` its PC + 1, `WB` the
  destination register. Meaningful on `ALT_ARCH_PIPE`.

`size_t alt_output(const alt_machine_t *machine, const char **out_bytes)`
: Everything the program sent over the UART so far (first `ALT_OUTPUT_MAX`
  bytes). Returns the length; `*out_bytes` points into the machine.

### Snapshots

A snapshot holds the whole board (every gate output and flip-flop), the
console and the cycle count. Loading one and running gives exactly the same
result as the original run: the simulation is deterministic.

`size_t alt_snapshot_size(void)`
: Bytes needed for one snapshot in this build.

`alt_status_t alt_snapshot_save(const alt_machine_t *machine, void *out_buf, size_t out_buf_len)`
: Copies the state into `out_buf`. `ALT_ERR_ARG` if it is too small.

`alt_status_t alt_snapshot_load(alt_machine_t *machine, const void *buf, size_t buf_len)`
: Replaces the machine's state. `ALT_ERR_FORMAT` if the buffer is not a
  snapshot of this build. Load the same ROM first; the ROM is part of the
  board, but the copy used by `alt_ref_check` is not. The loaded machine is
  already powered if the original was. The UART bytes kept for `alt_output`
  and the remembered keys are not part of a snapshot: they start empty in the
  new machine.

`alt_status_t alt_snapshot_save_file(const alt_machine_t *machine, const char *path)`,
`alt_status_t alt_snapshot_load_file(alt_machine_t *machine, const char *path)`
: Same, to and from a file. `ALT_ERR_IO` if the file cannot be opened or
  written.

### Reference check

`alt_status_t alt_ref_check(alt_machine_t *machine, uint64_t max_steps, FILE *log, unsigned *out_differences)`
: Runs the same ROM and the same keys on the reference instruction-set
  simulator (up to `max_steps` instructions) and compares UART output,
  registers 1 to 31, all of RAM and the halt flag with the gate-level machine.
  The number of differences goes to `*out_differences`; if `log` is not
  `NULL`, a report is written to it. Call it after the run.

## Examples

Fault campaign on one bit:

```c
for (unsigned bit = 0; bit < 32; bit++) {
    alt_machine_t *m;
    alt_machine_create(NULL, &m);
    alt_load_rom(m, rom, count);
    alt_add_fault(m, 3000, ALT_FAULT_REG, 8, bit);
    alt_power_on(m);
    alt_run(m, 200000);
    unsigned diffs;
    alt_ref_check(m, 200000, NULL, &diffs);
    printf("r8 bit %2u: %u differences\n", bit, diffs);
    alt_machine_destroy(m);
}
```

Fork a run from a snapshot:

```c
size_t size = alt_snapshot_size();
void *snap = malloc(size);
alt_run(a, 3000);
alt_snapshot_save(a, snap, size);

alt_machine_t *b;
alt_machine_create(NULL, &b);
alt_load_rom(b, rom, count);
alt_snapshot_load(b, snap, size);
alt_send_key(b, 'x');
alt_run(a, 100000);
alt_run(b, 100000);
```

Change memory under a running program:

```c
static void poke(void *ctx, alt_machine_t *m) {
    if (alt_cycles(m) == 3000) alt_ram_write(m, 0x40, 5);
}

alt_set_cycle_hook(m, poke, NULL);
```
