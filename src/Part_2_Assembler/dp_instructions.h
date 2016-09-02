#ifndef DP_INSTRUCTIONS_H
#define DP_INSTRUCTIONS_H
#include <stdint.h>

/* ******************************************************************** */
/*                       PROTOTYPES FOR DP FUNCTIONS                    */
/* ******************************************************************** */

uint32_t get_op2_count(char **line, uint32_t number_of_regs_and_opcode);
uint32_t set_reg_result(uint32_t binary_template, char **line);
uint32_t generate_op2_binary_shift_by_reg(char **line, uint32_t op2_start_index);
uint32_t generate_op2_immediate_expression(char **line, uint32_t op2_start_index);
uint32_t generate_op2(char **line, uint32_t number_of_regs_and_opcode);
uint32_t set_reg_result_cpsr(uint32_t binary_template, char **line);
uint32_t gen_DP_2(char **line, uint32_t binary_template);
uint32_t gen_DP_1(char **line, uint32_t binary_template);

uint32_t generate_DP_add(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_sub(char **line, uint32_t line_no , uint32_t *length_of_program);
uint32_t generate_DP_rsub(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_and(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_eor(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_or(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_mov(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_tst(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_teq(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_DP_cmp(char **line, uint32_t line_no, uint32_t *length_of_program);

#endif