#include "snapshot.h"
#include <string.h>

#define SNAP_MAGIC "ALTHSNP1"

typedef struct {
    char magic[8];
    unsigned long long board_size;
    unsigned long long ram_words;
    unsigned long long cycles;
    unsigned long long physics_ticks;
    int arch;
    int console_prev_valid;
    int key_in_flight;
} snap_header_t;

typedef struct {
    snap_header_t header;
    board_t board;
    uart_rx_t console_rx;
    uart_tx_t console_tx;
} snap_image_t;

size_t snapshot_size(void) {
    return sizeof(snap_image_t);
}

static void fill_header(const simulator_t *sim, snap_header_t *h) {
    memset(h, 0, sizeof *h);
    memcpy(h->magic, SNAP_MAGIC, sizeof h->magic);
    h->board_size = sizeof sim->board;
    h->ram_words = RAM_WORDS;
    h->cycles = sim->cycles;
    h->physics_ticks = sim->physics_ticks;
    h->arch = (int)sim->arch;
    h->console_prev_valid = sim->console_prev_valid;
    h->key_in_flight = sim->key_in_flight;
}

int snapshot_write(const simulator_t *sim, unsigned char *out, size_t out_len) {
    if (out_len < sizeof(snap_image_t)) return -1;
    snap_image_t *img = (snap_image_t *)out;
    memset(img, 0, sizeof *img);
    fill_header(sim, &img->header);
    memcpy(&img->board, &sim->board, sizeof img->board);
    memcpy(&img->console_rx, &sim->console_rx, sizeof img->console_rx);
    memcpy(&img->console_tx, &sim->console_tx, sizeof img->console_tx);
    return 0;
}

static int header_matches(const snap_header_t *h) {
    return memcmp(h->magic, SNAP_MAGIC, sizeof h->magic) == 0 && h->board_size == sizeof(board_t) && h->ram_words == RAM_WORDS && h->arch >= SIM_ARCH_SINGLE && h->arch <= SIM_ARCH_PIPE;
}

int snapshot_read(simulator_t *sim, const unsigned char *in, size_t in_len) {
    if (in_len != sizeof(snap_image_t)) return -1;
    const snap_image_t *img = (const snap_image_t *)in;
    if (!header_matches(&img->header)) return -1;
    memcpy(&sim->board, &img->board, sizeof sim->board);
    memcpy(&sim->console_rx, &img->console_rx, sizeof sim->console_rx);
    memcpy(&sim->console_tx, &img->console_tx, sizeof sim->console_tx);
    sim->cycles = img->header.cycles;
    sim->physics_ticks = img->header.physics_ticks;
    sim->arch = (sim_arch_t)img->header.arch;
    sim->console_prev_valid = img->header.console_prev_valid;
    sim->key_in_flight = img->header.key_in_flight;
    return 0;
}
