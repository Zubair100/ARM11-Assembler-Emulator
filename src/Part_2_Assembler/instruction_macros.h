#ifndef INSTRUCTION_MACROS_H
#define INSTRUCTION_MACROS_H

/* ******************************************************************** */
/*             MACROS USED TO REPRESENT REGISTER POSITIONS              */
/* ******************************************************************** */

#define POS_REGISTER_1 16
#define POS_REGISTER_2 12

/* ******************************************************************** */
/*              MACROS USED TO REPRESENT DP BINARY TEMPLATES            */
/* ******************************************************************** */

#define COND_DP_INSTR 0xE0000000

#define ADD_TEMPLATE 0x00800000
#define SUB_TEMPLATE 0x00400000
#define RSB_TEMPLATE 0x00600000
#define AND_TEMPLATE 0x00000000
#define EOR_TEMPLATE 0x00200000
#define OR_TEMPLATE 0x01800000
#define MOV_TEMPLATE 0x01A00000
#define TST_TEMPLATE 0x01100000
#define TEQ_TEMPLATE 0x01300000
#define CMP_TEMPLATE 0x01500000

/* ******************************************************************** */
/*           MACROS USED TO REPRESENT BRANCH BINARY TEMPLATES           */
/* ******************************************************************** */

#define BEQ_TEMPLATE 0x0A000000
#define BNE_TEMPLATE 0x1A000000
#define BGE_TEMPLATE 0xAA000000
#define BLT_TEMPLATE 0xBA000000
#define BGT_TEMPLATE 0xCA000000
#define BLE_TEMPLATE 0xDA000000
#define B_TEMPLATE 0xEA000000

/* ******************************************************************** */
/*             MACROS USED TO REPRESENT MUL BINARY TEMPLATES            */
/* ******************************************************************** */

#define MLA_TEMPLATE 0xE0200090
#define MUL_TEMPLATE 0xE0000090

/* ******************************************************************** */
/*                MACROS USED TO REPRESENT SHIFT AMOUNTS                */
/* ******************************************************************** */

#define POS_SHIFT_OFFSET 2
#define POS_REG_D 12
#define POS_OP_CODE 5
#define POS_EXPR 7
#define POS_SHIFT_EXPR 8
#define POS_FIRST_REG 16
#define POS_SECOND_REG 12
#define POS_THIRD_REG 8

/* ******************************************************************** */
/*                MACROS USED TO REPRESENT WORD INDEXING                */
/* ******************************************************************** */

#define SECOND_WORD 1
#define THIRD_WORD 2
#define FOURTH_WORD 3
#define FIFTH_WORD 4

/* ******************************************************************** */
/*                  MACROS USED TO REPRESENT BIT MASKS                  */
/* ******************************************************************** */

#define BITMASK_RIGHT_24_BITS 0x00FFFFFF
#define BITMASK_RIGHT_26_BITS 0x03FFFFFF

/* ******************************************************************** */
/*                             HELPER MACROS                            */
/* ******************************************************************** */

#define CHECK_IF_EXPR(x) ((x == 1))
#define SET_IMMEDIATE_BIT(x) (x = x | 0x02000000);
#define CLEAR_IMMEDIATE_BIT(x) (x = x | 0x00000000);
#define POS_ROTATION_AMOUNT 8
#define LAST_8_BITS 0x000000FF
#define Remove_opening_square_brackets(x) x = x + 1
#define MOV_TEMPLATE 0x01A00000
#define TOTAL_NO_OF_MNEUMONICS 23

#define Determine_int_expr() \
char *equals_removed = (first_addr_op + 1); \
if (equals_removed[0] == '-') { \
num_expr = equals_removed + 1; \
\
if (check_if_hex(num_expr)) { \
int_expr = strtol(equals_removed, NULL, 0); \
\
} else { \
int_expr = atoi(equals_removed); \
\
} \
\
} else { \
num_expr = equals_removed; \
\
if (check_if_hex(num_expr)){ \
int_expr = strtol(num_expr, NULL, 0); \
\
} else { \
int_expr = atoi(num_expr); \
\
} \
\
}

#define Determine_int_expr_op2() \
  char *expression_shift = get_word(line, op2_start_index + 2); \
  char first_char = expression_shift[0]; \
  if (first_char == '#') { \
    char *immediate_expr = &(expression_shift[1]); \
    int_expr = atoi(immediate_expr) << 7; \
    int_expr = int_expr | 0x00000000; \
  } else { \
    int_expr = gen_reg_no(expression_shift) << 8; \
    int_expr = int_expr | 0x00000010; \
  }

#define Determine_binary_rep() \
  if (check_if_hex(op2_string_expr)) { \
    binary_rep = strtol(op2_string_expr,NULL,0); \
  } else { \
    binary_rep = atoi(op2_string_expr); \
  } \
 \
  rotation_amount = get_rotate_amount(binary_rep); \
  binary_rep = rotate_left(binary_rep, rotation_amount); \
  binary_rep = binary_rep | ((rotation_amount / 2) << POS_ROTATION_AMOUNT);

#define Determine_reg_and_shift_op() \
  uint32_t m = gen_reg_no(get_word(line, op2_start_index)); \
  uint32_t shift_op = gen_shift_op_code(get_word(line, op2_start_index + 1)) << 5;

#define generate_mov() \
binary_template = MOV_TEMPLATE | 0xE0000000; \
binary_template = binary_template | rd; \
uint32_t imm_value = generate_SDT_op2_immediate_expression (int_expr); \
binary_template = binary_template | rd | imm_value;

#endif
