#ifndef BRANCH_INSTRUCTIONS_H
#define BRANCH_INSTRUCTIONS_H

/* ******************************************************************** */
/*                    PROTOTYPES FOR BRANCH FUNCTIONS                   */
/* ******************************************************************** */

int32_t calculate_offset(char **line);
uint32_t generate_BRANCH_beq(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_bne(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_bge(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_blt(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_bgt(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_ble(char **line, uint32_t line_no, uint32_t *length_of_program);
uint32_t generate_BRANCH_b(char **line, uint32_t line_no, uint32_t *length_of_program);

#endif
