#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mul_instructions.h"
#include "utilities.h"
#include "tokeniser.h"
#include "instruction_macros.h"

uint32_t get_reg_MUL(char **line) {
  // PRE: No null pointer
  // POST: Returns the registers in the correct positions

  uint32_t d = gen_reg_no(get_word(line, SECOND_WORD)) << POS_FIRST_REG;
  uint32_t m = gen_reg_no(get_word(line, THIRD_WORD));
  uint32_t s = gen_reg_no(get_word(line, FOURTH_WORD)) << POS_THIRD_REG;

  return (d | m | s);
}

uint32_t generate_MUL_mla(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the generated machine code

  return (MLA_TEMPLATE | get_reg_MUL(line) | (gen_reg_no(get_word(line, FIFTH_WORD)) << POS_SECOND_REG));
}

uint32_t generate_MUL_mul(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the generated machine code

  return (MUL_TEMPLATE | get_reg_MUL(line));
}
