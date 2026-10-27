#ifndef ALETHEIA_ALETHEIA_H
#define ALETHEIA_ALETHEIA_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALT_VERSION_MAJOR 0
#define ALT_VERSION_MINOR 9
#define ALT_VERSION_PATCH 0

#if defined(_WIN32) && defined(ALT_BUILD_SHARED)
#define ALT_API __declspec(dllexport)
#elif defined(_WIN32) && defined(ALT_SHARED)
#define ALT_API __declspec(dllimport)
#elif defined(__GNUC__)
#define ALT_API __attribute__((visibility("default")))
#else
#define ALT_API
#endif

typedef struct alt_machine alt_machine_t;

typedef enum {
    ALT_OK = 0,
    ALT_ERR_NOMEM,
    ALT_ERR_ARG,
    ALT_ERR_IO,
    ALT_ERR_FORMAT,
    ALT_ERR_FULL
} alt_status_t;

typedef enum {
    ALT_ARCH_SINGLE,
    ALT_ARCH_MULTI,
    ALT_ARCH_PIPE
} alt_arch_t;

typedef enum {
    ALT_STOP_LIMIT,
    ALT_STOP_HALT,
    ALT_STOP_BREAK
} alt_stop_t;

typedef enum {
    ALT_FAULT_PC,
    ALT_FAULT_REG,
    ALT_FAULT_RAM
} alt_fault_target_t;

typedef enum {
    ALT_STAGE_ID,
    ALT_STAGE_EX,
    ALT_STAGE_MEM,
    ALT_STAGE_WB
} alt_stage_t;

#define ALT_STRAP_L1         0x1u
#define ALT_STRAP_L2         0x2u
#define ALT_STRAP_MMU        0x4u
#define ALT_STRAP_DEFAULT    (ALT_STRAP_L1 | ALT_STRAP_MMU)

#define ALT_SIGNAL_IRQ       0x1u
#define ALT_SIGNAL_EXCEPTION 0x2u
#define ALT_SIGNAL_MMU       0x4u
#define ALT_SIGNAL_USER      0x8u

#define ALT_ROM_WORDS        1024u
#define ALT_DISPLAY_CHARS    8u
#define ALT_OUTPUT_MAX       4096u

typedef struct {
    void *(*alloc)(void *ctx, size_t size);
    void (*free)(void *ctx, void *ptr);
    void *ctx;
} alt_allocator_t;

typedef void (*alt_output_fn)(void *ctx, int byte);
typedef void (*alt_cycle_fn)(void *ctx, alt_machine_t *machine);

ALT_API const char *alt_version(void);
ALT_API const char *alt_status_name(alt_status_t status);

ALT_API alt_status_t alt_machine_create(const alt_allocator_t *allocator, alt_machine_t **out_machine);
ALT_API void alt_machine_destroy(alt_machine_t *machine);

ALT_API alt_status_t alt_set_arch(alt_machine_t *machine, alt_arch_t arch);
ALT_API void alt_set_straps(alt_machine_t *machine, unsigned straps);
ALT_API alt_status_t alt_set_timer_div(alt_machine_t *machine, unsigned div);
ALT_API void alt_set_dram_leak(alt_machine_t *machine, unsigned long period);
ALT_API void alt_set_hz(alt_machine_t *machine, long hz);
ALT_API void alt_set_output(alt_machine_t *machine, alt_output_fn fn, void *ctx);
ALT_API void alt_set_cycle_hook(alt_machine_t *machine, alt_cycle_fn fn, void *ctx);

ALT_API alt_status_t alt_assemble_file(const char *path, uint32_t *words, size_t words_len,
                                       size_t *out_count, char *out_error, size_t out_error_len);
ALT_API alt_status_t alt_disassemble(uint32_t word, char *out_text, size_t out_text_len);
ALT_API alt_status_t alt_load_rom(alt_machine_t *machine, const uint32_t *words, size_t words_len);

ALT_API alt_status_t alt_send_key(alt_machine_t *machine, unsigned char key);
ALT_API alt_status_t alt_add_fault(alt_machine_t *machine, uint64_t cycle, alt_fault_target_t target,
                                   unsigned index, unsigned bit);
ALT_API alt_status_t alt_add_fault_spec(alt_machine_t *machine, const char *spec);
ALT_API void alt_power_cut(alt_machine_t *machine, uint64_t at_cycle, unsigned long off_cycles);
ALT_API alt_status_t alt_break_add(alt_machine_t *machine, uint32_t pc);

ALT_API alt_status_t alt_vcd_open(alt_machine_t *machine, const char *path);
ALT_API void alt_vcd_close(alt_machine_t *machine);

ALT_API void alt_power_on(alt_machine_t *machine);
ALT_API void alt_step(alt_machine_t *machine);
ALT_API alt_stop_t alt_run(alt_machine_t *machine, uint64_t max_cycles);

ALT_API uint64_t alt_cycles(const alt_machine_t *machine);
ALT_API int alt_halted(const alt_machine_t *machine);
ALT_API alt_arch_t alt_arch(const alt_machine_t *machine);
ALT_API long alt_hz(const alt_machine_t *machine);
ALT_API uint32_t alt_pc(alt_machine_t *machine);
ALT_API uint32_t alt_reg(alt_machine_t *machine, unsigned index);
ALT_API alt_status_t alt_ram_read(alt_machine_t *machine, uint32_t addr, uint32_t *out_word);
ALT_API alt_status_t alt_ram_write(alt_machine_t *machine, uint32_t addr, uint32_t word);
ALT_API size_t alt_ram_words(void);
ALT_API unsigned char alt_display_char(alt_machine_t *machine, unsigned index);
ALT_API unsigned alt_signals(const alt_machine_t *machine);
ALT_API alt_status_t alt_stage(const alt_machine_t *machine, alt_stage_t stage, uint32_t *out_value, int *out_valid);
ALT_API size_t alt_output(const alt_machine_t *machine, const char **out_bytes);

#ifdef __cplusplus
}
#endif

#endif
