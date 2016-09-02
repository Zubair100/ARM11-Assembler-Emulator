#ifndef SDT_INSTRUCTIONS_H
#define SDT_INSTRUCTIONS_H

/* ******************************************************************** */
/*                      PROTOTYPES FOR SDT FUNCTIONS                    */
/* ******************************************************************** */

char *remove_closing_square_brackets(char *str);
uint32_t gen_dec_expression_no(char *str, uint32_t index);
uint32_t gen_hex_expression_no(char *str, uint32_t index);
uint32_t generate_SDT_op2_immediate_expression(uint32_t binary_rep);
uint32_t calculate_PC_offset(uint32_t line_no, uint32_t length_of_program);
uint32_t compute_rd(char **line);
uint32_t compute_rn(char **line);
uint32_t compute_U(char *word, uint32_t index);
uint32_t compute_offset_expression(char *word);
uint32_t compute_offset_reg(char *rm, char *shift_type, char *shift_amount);
uint32_t compute_address(char **line);

uint32_t generate_SDT_ldr(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_SDT_str(char **line, uint32_t line_no, uint32_t *length_of_program);

#endif