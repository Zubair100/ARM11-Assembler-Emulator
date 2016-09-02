#ifndef EMULATE_HELPER_FUNCTIONS_H
#define EMULATE_HELPER_FUNCTIONS_H

/* ******************************************************************** */
/*               PROTOTYPES FOR EMULATE HELPER FUNCTIONS                */
/* ******************************************************************** */

uint32_t endian_switcher(uint32_t word);
uint32_t get_mem_value(uint32_t location);
uint32_t set_mem_value(uint32_t location, uint32_t word);

void rotate_right(uint32_t *num, uint32_t rotate_amount);
void logical_shift_left(uint32_t *num, uint32_t shift_amount);
void logical_shift_right(uint32_t *num, uint32_t shift_amount);
void arithmetic_shift_right(uint32_t *num, uint32_t shift_amount);

uint32_t get_first_reg();
uint32_t get_second_reg();
uint32_t get_third_reg();
uint32_t get_fourth_reg();

void set_s_flag();

void compute_op2_immediate(void);
void compute_op2_shifted_reg(void);

#endif
