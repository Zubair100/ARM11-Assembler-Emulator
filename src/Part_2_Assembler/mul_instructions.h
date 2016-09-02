#ifndef MUL_INSTRUCTIONS_H
#define MUL_INSTRUCTIONS_H

/* ******************************************************************** */
/*                      PROTOTYPES FOR MUL FUNCTIONS                    */
/* ******************************************************************** */

uint32_t get_reg_MUL(char **line);
uint32_t generate_MUL_mla(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_MUL_mul(char **line, uint32_t line_no, uint32_t *length_of_program);

#endif