#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "instruction_macros.h"

uint32_t string_length(char *str) {
  // PRE: Pointer not null
  // POST: Returns length of string

  uint32_t length = 0;

  while (str[length] != '\0') {
    length++;
  }

  return length;
}

uint32_t gen_reg_no(char *str) {
  //PRE: String must be of the format rx where x can be any integer number
  //POST: Returns value of register number

  return (atoi(&(str[1])));
}

uint32_t check_if_hex(char *word) {
  //PRE: The input string has to be a number, not #number or =number
  //POST: Returns true if hex, else returns false

  if (word[1] == 'x') {
    return 1;
  }

  return 0;
}

uint32_t rotate_left(uint32_t num, uint32_t rotate_amount) {
  // PRE: None
  // POST: Input is rotated left by (rotate_amount % 32)

  rotate_amount = rotate_amount % 32;
  uint32_t right_shift = 32 - rotate_amount;
  uint32_t right_bits = num >> right_shift;

  if (right_shift == 32) {
    right_bits = 0;
  }

  uint32_t left_bits = num << rotate_amount;
  return left_bits + right_bits;
}

uint32_t get_rotate_amount(uint32_t binary_number) {
  // PRE: None
  // POST: Returns how many rotations required to fit a number into the last
  //       8 bits of a 32-bit integer

  uint32_t rotate_op2;
  for (int i = 0; i < 31; i += 2) {
    rotate_op2 = rotate_left(binary_number, i);

    if ((rotate_op2 & LAST_8_BITS) == rotate_op2) {
      return i;
    }
  }

  assert("Numeric constant cannot be represented" && 0);
}

uint32_t gen_shift_op_code(char *string_op_code) {
  // PRE: Pointer not null
  // POST: Generates corresponding opcode from shift name

    uint32_t shift_op;

    if (strcmp(string_op_code, "lsl") == 0) {
        shift_op = 0;
    } else if (strcmp(string_op_code, "lsr") == 0) {
        shift_op = 1;
    } else if (strcmp(string_op_code, "asr") == 0) {
        shift_op = 2;
    } else if (strcmp(string_op_code, "ror") == 0) {
        shift_op = 3;
    } else {
        assert("Invalid Shift" && 0);
    }

    return shift_op;
}
