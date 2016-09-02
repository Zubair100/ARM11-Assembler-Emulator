#include "emulate.h"
#include <stdint.h>

uint32_t endian_switcher(uint32_t word) {
  // PRE: None
  // POST: Output is word with endianness reversed

  uint32_t byte1, byte2, byte3, byte4;
  byte1 = (word & BITMASK_LEFT_8_BITS) >> 24;
  byte2 = (word & BITMASK_MIDDLE_LEFT_8_BITS) >> 8;
  byte3 = (word & BITMASK_MIDDLE_RIGHT_8_BITS) << 8;
  byte4 = (word & BITMASK_RIGHT_8_BITS) << 24;

  return byte1 | byte2 | byte3 | byte4;
}

uint32_t get_mem_value(uint32_t location) {
  // PRE: Location is within bounds of memory
  // POST: Outputs value at memory[location] in big endian

  uint32_t byte1, byte2, byte3, byte4;
  uint32_t word_le;

  byte1 = machine_state.memory[location] << 24;
  byte2 = machine_state.memory[location + 1] << 16;
  byte3 = machine_state.memory[location + 2] << 8;
  byte4 = machine_state.memory[location + 3];

  word_le = byte1 | byte2 | byte3 | byte4;
  return endian_switcher(word_le);
}

void set_mem_value(uint32_t location, uint32_t word) {
  // PRE: Location is within bounds of memory
  //        and word is in big endian
  // POST: Stores word at memory[location] in little endian

  uint8_t byte1, byte2, byte3, byte4;

  byte4 = (word & BITMASK_LEFT_8_BITS) >> 24;
  machine_state.memory[location + 3] = byte4;

  byte3 = (word & BITMASK_MIDDLE_LEFT_8_BITS) >> 16;
  machine_state.memory[location + 2] = byte3;

  byte2 = (word & BITMASK_MIDDLE_RIGHT_8_BITS) >> 8;
  machine_state.memory[location + 1] = byte2;

  byte1 = (word & BITMASK_RIGHT_8_BITS);
  machine_state.memory[location] = byte1;
}

void rotate_right(uint32_t *num, uint32_t rotate_amount) {
  // PRE: None
  // POST: Input is rotated right by (rotate_amount % 32)
  //         and carry is equal to bit in position (rotate_amount % 32) - 1

  rotate_amount = rotate_amount % 32;

  int left_shift = 32 - rotate_amount;
  uint32_t left_bits = *num << left_shift;

  if (left_shift == 32) {
    left_bits = 0;
  }

  uint32_t right_bits = *num >> rotate_amount;
  *num = left_bits + right_bits;

  if (rotate_amount == 0) {
    execute_variables.c_bit = 0;
  } else {
    execute_variables.c_bit = (*num >> (rotate_amount - 1)) & BITMASK_RIGHT_1_BIT;
  }
}

void logical_shift_left(uint32_t *num, int shift_amount) {
  // PRE: None
  // POST: Input is rotated left by shift_amount
  //          and carry is equal to bit in position (32 - shift_amount) when
  //          shift_amount < 32, otherwise equal to 0

  if (shift_amount < 32) {
    *num = *num << shift_amount;
  } else {
    *num = 0;
  }


  if (shift_amount == 0 || shift_amount > 32) {
    execute_variables.c_bit = 0;
  } else {
    execute_variables.c_bit = (*num >> (32 - (shift_amount))) & BITMASK_RIGHT_1_BIT;
  }
}

void logical_shift_right(uint32_t *num, int shift_amount) {
  // PRE: None
  // POST: Input is rotated right by shift_amount
  //          and carry is equal to bit in position (shift_amount - 1) when
  //          shift_amount < 32, otherwise equal to 0

  if (shift_amount < 32) {
    *num = *num >> shift_amount;
  } else {
    *num = 0;
  }

  if (shift_amount == 0 || shift_amount > 32) {
    execute_variables.c_bit = 0;
  } else {
    execute_variables.c_bit = (*num >> (shift_amount - 1)) & BITMASK_RIGHT_1_BIT;
  }
}

void arithmetic_shift_right(uint32_t *num, int shift_amount) {
  // PRE: None
  // POST: Input is rotated right by shift_amount with leading ones if msb is 1,
  //          leading zeroes otherwise,
  //          and carry is equal to bit in position (shift_amount - 1) when
  //          shift_amount < 32, otherwise equal to msb

  uint32_t msb = (*num >> 31);
  *num = *num >> shift_amount;

  if (msb == 1) {
    uint32_t leading_ones = BITMASK_RIGHT_32_BITS << (31 - shift_amount);
    *num += leading_ones;
  }

  if (shift_amount == 0) {
    execute_variables.c_bit = 0;
  } else if (shift_amount > 32) {
    execute_variables.c_bit = msb;
  } else {
    execute_variables.c_bit = (*num >> (shift_amount - 1)) & BITMASK_RIGHT_1_BIT;
  }
}

uint32_t get_first_reg() {
  return (execute_variables.ir >> POS_FIRST_REG) &BITMASK_RIGHT_4_BITS;
}

uint32_t get_second_reg() {
  return (execute_variables.ir >> POS_SECOND_REG) &BITMASK_RIGHT_4_BITS;
}

uint32_t get_third_reg() {
  return (execute_variables.ir >> POS_THIRD_REG) &BITMASK_RIGHT_4_BITS;
}

uint32_t get_fourth_reg() {
  return (execute_variables.ir & BITMASK_RIGHT_4_BITS);
}

void set_s_flag() {
  uint32_t s_bit = (execute_variables.ir >> POS_S_BIT) & BITMASK_RIGHT_1_BIT;
  if (s_bit) {
    execute_variables.s_flag = 1;
  } else {
    execute_variables.s_flag = 0;
  }
}

void compute_op2_immediate(void) {
  uint32_t rotateAmount = ((execute_variables.ir >> POS_IMM_ROTATE)
                           & BITMASK_RIGHT_4_BITS) << 1;
  execute_variables.op2 = execute_variables.ir & BITMASK_RIGHT_8_BITS;
  rotate_right(&execute_variables.op2, rotateAmount);
}

void compute_op2_shifted_reg(void) {
  uint32_t shift_amount;
  uint32_t m = execute_variables.ir & BITMASK_RIGHT_4_BITS;

  execute_variables.op2 = machine_state.reg[m];
  uint32_t shift_op = (execute_variables.ir >> POS_SHIFT_TYPE) & BITMASK_RIGHT_2_BITS;

  if (IS_SHIFT_BY_REG) {
    uint32_t s = (execute_variables.ir >> POS_SHIFT_REGISTER) & BITMASK_RIGHT_4_BITS;
    shift_amount = machine_state.reg[s] & BITMASK_RIGHT_8_BITS;
  } else {
    shift_amount = (execute_variables.ir >> POS_REG_SHIFT) & BITMASK_RIGHT_5_BITS;
  }

  switch (shift_op) {
    case LSL:
      logical_shift_left(&execute_variables.op2, shift_amount);
      break;
    case LSR:
      logical_shift_right(&execute_variables.op2, shift_amount);
      break;
    case ASR:
      arithmetic_shift_right(&execute_variables.op2, shift_amount);
      break;
    case ROR:
      rotate_right(&execute_variables.op2, shift_amount);
      break;
    default:
      execute_variables.mode = INVALID_INSTRUCTION;
  }
}
