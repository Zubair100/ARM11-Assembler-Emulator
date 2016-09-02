#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "branch_instructions.h"
#include "utilities.h"
#include "tokeniser.h"
#include "instruction_macros.h"
#include "assemble.h"

int32_t calculate_offset(char **line) {
  // PRE: No null pointers
  // POST: Calculates the branch offset

  char *word = get_word(line, SECOND_WORD);
  int32_t offset;

  if (string_length(word) == 0) {
    assert("Branch offset must not be empty" && 0);
  }

  if (word[0] == '-') {
    offset = atoi(&(word[1])) * (-1);
  } else {
    offset = atoi(&(word[0]));
  }

  return offset = ((offset & BITMASK_RIGHT_32_BITS) >> POS_SHIFT_OFFSET) & BITMASK_RIGHT_24_BITS;
}

uint32_t generate_BRANCH_beq(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BEQ_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_bne(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BNE_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_bge(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BGE_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_blt(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BLT_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_bgt(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BGT_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_ble(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = BLE_TEMPLATE | offset;
  return binary_template;
}

uint32_t generate_BRANCH_b(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the binary representation

  int32_t offset = calculate_offset(line);
  uint32_t binary_template = B_TEMPLATE | offset;
  return binary_template;
}
