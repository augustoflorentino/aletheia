#include "assembler.h"
#include <stdio.h>
#include <string.h>

static int read_line(FILE *f, char *line, int cap, int *too_long) {
    if (!fgets(line, cap, f)) return 0;
    int len = (int)strlen(line);
    *too_long = 0;
    if (len == cap - 1 && line[len - 1] != '\n' && !feof(f)) {
        int c;
        while ((c = fgetc(f)) != EOF && c != '\n') {
        }
        *too_long = 1;
    }
    return 1;
}

static int report(assembler_t *as, const char *file, int line_no) {
    char msg[MAX_ERROR_LEN];
    memcpy(msg, as->error, sizeof msg);
    snprintf(as->error, MAX_ERROR_LEN, "%.200s:%d: error: %.127s", file, line_no, msg);
    return -1;
}

static int place_line(assembler_t *as, asm_line_t *l, unsigned int *output, int max_words, int emit) {
    if (l->kind == LINE_ORG) {
        if (l->org < as->current_address) {
            snprintf(as->error, MAX_ERROR_LEN, ".org 0x%x is behind the current address 0x%x", l->org, as->current_address);
            return -1;
        }
        if (l->org > (unsigned int)max_words) {
            snprintf(as->error, MAX_ERROR_LEN, ".org 0x%x is beyond the ROM (%d words)", l->org, max_words);
            return -1;
        }
        while (emit && as->current_address < l->org) output[as->current_address++] = 0;
        as->current_address = l->org;
    }
    if (!emit && l->label[0] && assembler_add_label(as, l->label, as->current_address)) return -1;
    if (l->kind != LINE_INSTR && l->kind != LINE_WORD) return 0;
    if (as->current_address >= (unsigned int)max_words) {
        snprintf(as->error, MAX_ERROR_LEN, "program does not fit in the ROM (%d words)", max_words);
        return -1;
    }
    unsigned int word = 0;
    if (emit && l->kind == LINE_WORD) word = l->org;
    if (emit && l->kind == LINE_INSTR && assembler_assemble_line(as, l->text, &word)) return -1;
    if (emit) output[as->current_address] = word;
    as->current_address++;
    return 0;
}

static int assemble_pass(assembler_t *as, FILE *f, const char *filename,
                         unsigned int *output, int max_words, int emit) {
    char line[MAX_LINE_LEN];
    int line_no = 0;
    int too_long;
    as->current_address = 0;
    while (read_line(f, line, MAX_LINE_LEN, &too_long)) {
        line_no++;
        if (too_long) {
            snprintf(as->error, MAX_ERROR_LEN, "line too long");
            return report(as, filename, line_no);
        }
        asm_line_t l;
        if (assembler_split_line(as, line, &l)) return report(as, filename, line_no);
        if (place_line(as, &l, output, max_words, emit)) return report(as, filename, line_no);
    }
    return (int)as->current_address;
}

int assembler_assemble_file(assembler_t *as, const char *filename,
                            unsigned int *output, int max_words) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        snprintf(as->error, MAX_ERROR_LEN, "%.300s: cannot open file", filename);
        return -1;
    }
    int count = assemble_pass(as, f, filename, output, max_words, 0);
    if (count >= 0) {
        rewind(f);
        count = assemble_pass(as, f, filename, output, max_words, 1);
    }
    fclose(f);
    return count;
}
