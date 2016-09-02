#ifndef EMULATE_H
#define EMULATE_H

#include <stdint.h>
#include "emulate_macros.h"

/* ******************************************************************** */
/*                PROTOTYPES FOR CORE EMULATE FUNCTIONS                 */
/* ******************************************************************** */

void clear_memory(void);
void clear_registers(void);
void load_memory(char *ptr);
void fetch(void);
void decode(void);
void execute(void);
void print_registers(void);
void print_memory(void);

/* ******************************************************************** */
/*                              STRUCTURES                              */
/* ******************************************************************** */

struct arm11 {
    uint32_t reg[MAX_REG_NO];
    uint8_t memory[MAX_MEM_SIZE];
};

struct pipeline {
    uint32_t ir;
    uint32_t n;
    uint32_t s;
    uint32_t d;
    uint32_t m;
    uint32_t mode;
    int32_t branch_offset;
    uint32_t SDT_offset;
    uint32_t op2;
    uint32_t halt_flag;
    uint32_t c_bit;
    uint32_t s_flag;
    uint32_t cond;
};

extern struct arm11 machine_state;
extern struct pipeline execute_variables;

#endif
