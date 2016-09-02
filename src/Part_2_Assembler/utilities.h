#ifndef UTILITIES_H
#define UTILITIES_H

/* ******************************************************************** */
/*                    PROTOTYPES FOR UTILITY FUNCTIONS                  */
/* ******************************************************************** */

uint32_t string_length(char *str);
uint32_t gen_reg_no(char *str);
uint32_t check_if_hex(char *word);

uint32_t rotate_left(uint32_t num, uint32_t rotate_amount);
uint32_t get_rotate_amount(uint32_t binary_number);
uint32_t gen_shift_op_code(char* string_op_code);

#endif
