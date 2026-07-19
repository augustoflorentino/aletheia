#ifndef ALETHEIA_REGISTER_FILE_H
#define ALETHEIA_REGISTER_FILE_H

#include "../../core/types.h"
#include "register.h"

#define REG_COUNT 32

typedef struct {
    register_32_t regs[REG_COUNT];
    bit read_addr1[5];
    bit read_addr2[5];
    bit write_addr[5];
    bit write_enable;
    bit write_data[32];
    bit read_data1[32];
    bit read_data2[32];
} register_file_t;

void regfile_init(register_file_t *rf);
void regfile_reset(register_file_t *rf);
void regfile_set_read_addr1(register_file_t *rf, bit *addr);
void regfile_set_read_addr2(register_file_t *rf, bit *addr);
void regfile_set_write_addr(register_file_t *rf, bit *addr);
void regfile_set_write_data(register_file_t *rf, bit *data);
void regfile_set_write_enable(register_file_t *rf, bit enable);
void regfile_clock_rise(register_file_t *rf);
void regfile_clock_fall(register_file_t *rf);
void regfile_propagate(register_file_t *rf);

#endif
