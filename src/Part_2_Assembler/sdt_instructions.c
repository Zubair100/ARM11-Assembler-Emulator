#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "utilities.h"
#include "tokeniser.h"
#include "instruction_macros.h"
#include "global_variables.h"



char *remove_closing_square_brackets(char *str) {
    //PRE: None
    //POST: Returns string with closing square bracket removed

    uint32_t count = 0;

    while (str[count] != '\0') {
        if (str[count] == ']') {
            str[count] = '\0';
            return str;
        }
        count++;
    }

    return str;
}

uint32_t gen_dec_expression_no(char *str, uint32_t index) {
    //PRE: String must be of the format #x where x can be any positive or negative integer
    //POST: Returns magnitude of value as an integer

    return (atoi(&(str[index])));
}

uint32_t gen_hex_expression_no(char *str, uint32_t index) {
    //PRE: String must be of the format #x where x can be any positive or negative hex value
    //POST: Returns magnitude of value as an integer

    return (uint32_t)strtol(&(str[index]), NULL, 0);
}

uint32_t generate_SDT_op2_immediate_expression(uint32_t binary_rep) {
  // PRE: No null pointer
  // POST: Generates the binary for an immediate expression

  uint32_t rotation_amount;
  rotation_amount = get_rotate_amount(binary_rep);
  binary_rep = rotate_left(binary_rep, rotation_amount);
  binary_rep = binary_rep | ((rotation_amount / 2) << POS_ROTATION_AMOUNT);
  SET_IMMEDIATE_BIT(binary_rep)

  return binary_rep;
}

uint32_t calculate_PC_offset(uint32_t line_no, uint32_t length_of_program) {
  // PRE: None
  // POST: Returns the offset between the current PC index and a data value
  //       to be loaded into a register

  uint32_t curr_PC_val;
  uint32_t offset;
  uint32_t last_addr;
  curr_PC_val = line_no * 4 + 8;
  last_addr = length_of_program * 4;
  offset = last_addr - curr_PC_val;

  return offset;
}

uint32_t compute_rd(char **line) {
  // PRE: The input is an array of strings
  // POST: Returns value of d shifted to correct place in a 32-bit binary int

  char *word = get_word(line, 1);
  return (gen_reg_no(word)) << POS_SECOND_REG;
}

uint32_t compute_rn(char **line) {
  // PRE: The input is an array of strings
  // POST: Returns value of n shifted to correct place in a 32-bit binary int

  char *word = get_word(line, 2);
  Remove_opening_square_brackets(word);
  word = remove_closing_square_brackets(word);
  return (gen_reg_no(word) << POS_FIRST_REG);
}

uint32_t compute_U(char *word, uint32_t index) {
  // PRE: Index must be 0 or 1
  // POST: Returns U bit shifted to the right position

  if (word[index] == '-') {
    return 0;
  } else {
    return 0x00800000;
  }
}

uint32_t compute_offset_expression(char *word) {
  // PRE: No null pointer
  // POST: Returns the value of the offset

  uint32_t offset;

  if (word[1] == '-') {
  //Value is negative

    if ((string_length(word) > 3) && (word[3] == 'x')) {
      //Value is hex format

      offset = gen_hex_expression_no(word, 2);
    } else {
      //Value is dec format

      offset = gen_dec_expression_no(word, 2);
    }
  } else {
    //Value is positive

    if ((string_length(word) > 2) && (word[2] == 'x')) {
      //Value is hex format

      offset = gen_hex_expression_no(word, 1);
    } else {
      //Value is dec format

      offset = gen_dec_expression_no(word, 1);
    }
  }

  assert((offset <= 0x00000FFF) && "Offset value is too large");
  return offset & 0x00000FFF;
}

uint32_t compute_offset_reg(char *rm, char *shift_type, char *shift_amount) {
  uint32_t offset;
  // PRE: No null pointers
  // POST: Returns the value of the offset for the register shift cases

  if (rm[0] == '-') {
    offset = gen_reg_no(&(rm[1]));
  } else {
    offset = gen_reg_no(rm);
  }

  if (shift_type != NULL) {
    uint32_t shift_op = (gen_shift_op_code(shift_type) << 5) & 0x00000060;
    char *amount = remove_closing_square_brackets(shift_amount);

    if (amount[0] == '#') {
      uint32_t const_shift_amt = (atoi(&(amount[1])) << 7) & 0x00000F80;
      offset = offset | const_shift_amt | shift_op;
    } else {
      uint32_t reg_s = (gen_reg_no(amount) << 8) & 0x00000F00;
      offset = offset | reg_s | shift_op | 0x00000010;
    }
  }

  return offset;
}

uint32_t compute_address(char **line) {
  // PRE: Takes in an array of strings which represents the instruction
  // POST: Returns a binary template which takes care of I, P, U, Rn, and OFFSET

  uint32_t IP;
  uint32_t U;
  uint32_t offset;

  //Gets [Rn] or [Rn, ...]
  char *start_of_addr_op = get_word(line, 2);
  //Calculates the length of start_of_addr_op. Used so we can indexstart_of_addr_op
  uint32_t word_length = string_length(start_of_addr_op);
  //Gets <#expression>, Rm or NULL
  char *second_addr_opr = get_word(line, 3);
  //Gets shift_type or NULL
  char *shift_type = get_word(line, 4);
  //Gets shift_amount or NULL
  char *shift_amount = get_word(line, 5);

  if (start_of_addr_op[word_length - 1] == ']') {
    //If we're in this case that means it could be either case 1, 4 or 5

    if (second_addr_opr == NULL) {
      //If we're in this case that means we're in case 1

      IP = 0x01000000; //I = 0 and P = 1
      U = 0x00800000; //U = 1
      offset = 0; //There is no offset in case 1
    } else {
      //If we're in this case that means it could be either case 4 or 5

      if (second_addr_opr[0] == '#') {
        //If we're in this case that means we're in case 4

        IP = 0; //I = 0 and P = 0
        U = compute_U(second_addr_opr, 1);
        offset = compute_offset_expression(second_addr_opr);
      } else {
        //If we're in this case that means we're in case 5

        IP = 0x02000000; //I = 1 and P = 0
        U = compute_U(second_addr_opr, 0);
        offset = compute_offset_reg(second_addr_opr, shift_type, shift_amount);
      }
    }
  } else {
    //If we're in this case that means it could be either case 2 or 3

    if (second_addr_opr[0] == '#') {
      //If we're in this case that means we're in case 2

      IP = 0x01000000; //I = 0 and P = 1
      U = compute_U(second_addr_opr, 1);
      offset = compute_offset_expression(remove_closing_square_brackets(second_addr_opr));
    } else {
      //If we're in this case that means we're in case 3

      IP = 0x03000000; //I = 1 and P = 1
      U = compute_U(second_addr_opr, 0);
      offset = compute_offset_reg(second_addr_opr, shift_type, shift_amount);
    }
  }

  return (IP | U | compute_rn(line) | offset);
}

uint32_t generate_SDT_ldr(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the generated machine code

  uint32_t binary_template;
  uint32_t rd = compute_rd(line);
  char *first_addr_op = get_word(line,2);

  if (first_addr_op[0] == '=') {
    char *num_expr;
    uint32_t int_expr;
    uint32_t offset;
    binary_template = 0xE59F0000;
    Determine_int_expr()

    if (int_expr <= 0x000000FF) {
      generate_mov()
      return binary_template;
    } else {
      offset = calculate_PC_offset(line_no, *length_of_program);
      *length_of_program = *length_of_program + 1;
      machine_code = realloc(machine_code, sizeof(uint32_t) * (*length_of_program));
      machine_code[(*length_of_program - 1)] = int_expr;
    }

    return (binary_template | rd | offset);
  } else {
    return (0xE4100000 | rd | compute_address(line));
  }
}

uint32_t generate_SDT_str(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the generated machine code

  return (0xE4000000 | compute_rd(line) | compute_address(line));
}
