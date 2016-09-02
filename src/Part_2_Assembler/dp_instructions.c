#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dp_instructions.h"
#include "utilities.h"
#include "tokeniser.h"
#include "instruction_macros.h"

uint32_t get_op2_count(char **line, uint32_t number_of_regs_and_opcode) {
  // PRE: No null pointer
  // POST: Counts the number of operands in operand2 for DP instructions

  int count = 0;

  while (get_word(line, count) != NULL) {
    count++;
  }

  return (count - number_of_regs_and_opcode);
}

uint32_t set_reg_result(uint32_t binary_template, char **line) {
  // PRE: No null pointer
  // POST: Sets register numbers in appropriate positions in binary
  //       representation of a given instruction

  uint32_t d = gen_reg_no(get_word(line, SECOND_WORD)) << POS_REGISTER_2;
  uint32_t n = gen_reg_no(get_word(line, THIRD_WORD)) << POS_REGISTER_1;

  return binary_template = binary_template | d | n;
}

uint32_t generate_op2_binary_shift_by_reg(char **line,
                                          uint32_t op2_start_index) {
  // PRE: No null pointer
  // POST: Generates the binary for a shift by register

  uint32_t int_expr;
  uint32_t shift_op;
  uint32_t m = gen_reg_no(get_word(line, op2_start_index));


  if (get_word(line, op2_start_index + 1) == NULL) {
    shift_op = 0;
    int_expr = 0;
  } else {
    shift_op = gen_shift_op_code(get_word(line, op2_start_index + 1)) << POS_OP_CODE;
    char *expression_shift = get_word(line, op2_start_index + 2);

    char first_char = expression_shift[0];

    if (first_char == '#') {
      if(check_if_hex(&(expression_shift[1]))){
        char *hex_expr = &(expression_shift[1]);
        int_expr = strtol(hex_expr, NULL, 0) << POS_EXPR;
      } else {
        char *immediate_expr = &(expression_shift[1]);
        int_expr = atoi(immediate_expr) << POS_EXPR;
      }
    } else {
      int_expr = gen_reg_no(expression_shift) << POS_SHIFT_EXPR;
      int_expr = int_expr | 0x00000010;
    }
  }

  return (int_expr | shift_op | m);
}

uint32_t generate_op2_immediate_expression(char **line, uint32_t op2_start_index) {
  // PRE: No null pointer
  // POST: Generates the binary for an immediate expression

  char *operand2 = get_word(line,op2_start_index);
  char *op2_string_expr = &(operand2[1]);
  uint32_t rotation_amount;
  uint32_t binary_rep;
  Determine_binary_rep()

  return binary_rep;
}

uint32_t generate_op2(char **line, uint32_t number_of_regs_and_opcode) {
  // PRE: No null pointers
  // POST: Generates operand2 for DP instructions

  uint32_t operand2_count = get_op2_count(line, number_of_regs_and_opcode);
  uint32_t operand2_start_index = number_of_regs_and_opcode;
  uint32_t op2;

  if (CHECK_IF_EXPR(operand2_count)) {
    if (get_word(line, operand2_start_index)[0] == '#') {
      op2  = generate_op2_immediate_expression(line, operand2_start_index);
      SET_IMMEDIATE_BIT(op2)
    } else {
      op2 = generate_op2_binary_shift_by_reg(line, operand2_start_index);
      CLEAR_IMMEDIATE_BIT(op2)
    }
  } else {
    op2 = generate_op2_binary_shift_by_reg(line, operand2_start_index);
    CLEAR_IMMEDIATE_BIT(op2)
  }

  return op2;
}

uint32_t set_reg_result_cpsr(uint32_t binary_template, char **line) {
  // PRE: No null pointers
  // POST: Sets register numbers for instructions that affect the CPSR

  uint32_t n = gen_reg_no(get_word(line, SECOND_WORD)) << POS_REGISTER_1;
  return (binary_template | n);
}

uint32_t gen_DP_2(char **line, uint32_t binary_template){
  // PRE: No null pointers
  // POST: Returns binary representation of DP instruction
  //       which only use both of the registers Rn and Rd

  uint32_t op2 = generate_op2(line, 3);

  binary_template = (binary_template | COND_DP_INSTR) | op2;
  return binary_template;
}

uint32_t gen_DP_1(char **line, uint32_t binary_template) {
  // PRE: No null pointers
  // POST: Returns binary representation of DP instruction
  //       which only use one of the registers Rn or Rd

  uint32_t op2 = generate_op2(line, 2);
  binary_template = (binary_template | COND_DP_INSTR) | op2;
  return binary_template;
}

uint32_t generate_DP_add(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = ADD_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_sub(char **line, uint32_t line_no , uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = SUB_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_rsub(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = RSB_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_and(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = AND_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_eor(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = EOR_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_or(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = OR_TEMPLATE;
  binary_template = set_reg_result(binary_template, line);
  return gen_DP_2(line, binary_template);
}

uint32_t generate_DP_mov(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = MOV_TEMPLATE;
  uint32_t d = gen_reg_no(get_word(line, SECOND_WORD)) << POS_REG_D;
  binary_template = binary_template | d;
  return gen_DP_1(line, binary_template);
}

uint32_t generate_DP_tst(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = TST_TEMPLATE;
  binary_template = set_reg_result_cpsr(binary_template, line);
  return gen_DP_1(line, binary_template);
}

uint32_t generate_DP_teq(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = TEQ_TEMPLATE;
  binary_template = set_reg_result_cpsr(binary_template, line);
  return gen_DP_1(line, binary_template);
}

uint32_t generate_DP_cmp(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  uint32_t binary_template = CMP_TEMPLATE;
  binary_template = set_reg_result_cpsr(binary_template, line);
  return gen_DP_1(line, binary_template);
}
