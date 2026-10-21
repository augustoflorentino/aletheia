#ifndef ALETHEIA_PANEL_H
#define ALETHEIA_PANEL_H

#include "../include/aletheia/aletheia.h"

#define PANEL_TERM_ROWS 10
#define PANEL_TERM_COLS 78

typedef struct {
    const uint32_t *rom;
    char term[PANEL_TERM_ROWS][PANEL_TERM_COLS + 1];
    int row;
    int col;
    int paused;
    double rate;
} panel_t;

void panel_init(panel_t *p, const uint32_t *rom);
void panel_output(void *ctx, int byte);
void panel_open(void);
void panel_close(void);
void panel_draw(panel_t *p, alt_machine_t *m);

#endif
