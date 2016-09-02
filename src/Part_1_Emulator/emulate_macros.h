#ifndef EMULATE_MACROS_H
#define EMULATE_MACROS_H

/* ******************************************************************** */
/*        MACRO USED TO REPRESENT SUCCESSFUL PROGRAM TERMINATION        */
/* ******************************************************************** */

#define EXIT_SUCCESS 0

/* ******************************************************************** */
/*          MACROS USED TO REPRESENT PROCESSOR STATE CONSTANTS          */
/* ******************************************************************** */

#define MAX_REG_NO 17
#define NUM_OF_GP_REGISTERS 13
#define MAX_MEM_SIZE 65536
#define MEM_ADDRESS_JUMP 4

/* ******************************************************************** */
/*          MACROS USED TO REPRESENT SPECIAL PURPOSE REGISTERS          */
/* ******************************************************************** */

#define PC (machine_state.reg[15])
#define CPSR (machine_state.reg[16])

/* ******************************************************************** */
/*                 MACROS USED TO REPRESENT CONDITIONS                  */
/* ******************************************************************** */

#define EQ 0
#define NE 1
#define GE 10
#define LT 11
#define GT 12
#define LE 13
#define AL 14

/* ******************************************************************** */
/*          MACRO USED TO DETERMINE IF HALT INSTRUCTION REACHED         */
/* ******************************************************************** */

#define INSTRUCTIONS_LEFT (execute_variables.halt_flag == 0)

/* ******************************************************************** */
/*             MACROS USED TO DETERMINE MODE OF INSTRUCTION             */
/* ******************************************************************** */

#define IS_HALT (execute_variables.ir == 0)

#define IS_BRANCH (((execute_variables.ir >> 24) & BITMASK_RIGHT_4_BITS) == 10)

#define IS_MULT ((((execute_variables.ir >> 4) & BITMASK_RIGHT_4_BITS) == 9) \
                 && (((execute_variables.ir >> 22) & 63) == 0))

#define IS_DP (((execute_variables.ir >> 26) & 3) == 0)
#define IS_AND 0
#define IS_EOR 1
#define IS_SUB 2
#define IS_RSUB 3
#define IS_ADD 4
#define IS_TEST 8
#define IS_TEQ 9
#define IS_CMP 10
#define IS_OR 12
#define IS_MOV 13

#define IS_SDT (((execute_variables.ir >> 26) & 3) == 1) \
                && (((execute_variables.ir >> 21) & 3) == 0)
#define IS_LOAD_PRE_UP (((execute_variables.ir >> 20) & 0x00000019) == 0x00000019)
#define IS_STORE_PRE_UP (((execute_variables.ir >> 20) & 0x00000018) == 0x00000018)
#define IS_LOAD_PRE_DOWN ((((execute_variables.ir >> 20) & 0x00000011)) == 0x00000011)
#define IS_STORE_PRE_DOWN ((((execute_variables.ir >> 20) & 0x00000010)) == 0x00000010)
#define IS_LOAD_POST_UP ((((execute_variables.ir >> 20) & 0x00000009)) == 0x00000009)
#define IS_STORE_POST_UP ((((execute_variables.ir >> 20) & 0x00000008)) == 0x00000008)
#define IS_LOAD_POST_DOWN ((((execute_variables.ir >> 20) & 0x00000001)) == 0x00000001)
#define IS_STORE_POST_DOWN ((((execute_variables.ir >> 20) & 0x00000000)) == 0x00000000)

/* ******************************************************************** */
/*         MACRO USED TO DETERMINE IF THE IMMEDIATE BIT IS SET          */
/* ******************************************************************** */

#define IS_I_SET (((execute_variables.ir >> 25) & 1) == 1)

/* ******************************************************************** */
/*     MACRO USED TO DETERMINE IF THE SHIFT BY REGISTER BIT IS SET      */
/* ******************************************************************** */

#define IS_SHIFT_BY_REG ((((execute_variables.ir >> 4) & 1) == 1) \
                         && (((execute_variables.ir >> 7) & 1) == 0))

/* ******************************************************************** */
/*            MACROS USED TO DETERMINE TYPE OF SHIFT IN DP/SDT          */
/* ******************************************************************** */

#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/* ******************************************************************** */
/*                    MACROS USED TO REPRESENT MODES                    */
/* ******************************************************************** */

#define HALT 0
#define BRANCH 1
#define DP_AND 2
#define DP_EOR 3
#define DP_SUB 4
#define DP_RSUB 5
#define DP_ADD 6
#define DP_TEST 7
#define DP_TEQ 8
#define DP_CMP 9
#define DP_OR 10
#define DP_MOV 11
#define SDT_LOAD_PRE_UP 12
#define SDT_STORE_PRE_UP 13
#define SDT_LOAD_POST_UP 14
#define SDT_STORE_POST_UP 15
#define SDT_LOAD_PRE_DOWN 16
#define SDT_STORE_PRE_DOWN 17
#define SDT_LOAD_POST_DOWN 18
#define SDT_STORE_POST_DOWN 19
#define MUL_ACC 20
#define MUL_NORMAL 21
#define INVALID_INSTRUCTION 22

/* ******************************************************************** */
/*                   MACRO USED TO SET MODE FOR EXECUTE                 */
/* ******************************************************************** */

#define SET_MODE(m) execute_variables.mode = m;

/* ******************************************************************** */
/*                   MACRO USED TO DETERMINE SDT TYPE                   */
/* ******************************************************************** */

#define SET_SDT_MODE() \
  if (IS_LOAD_PRE_UP) { \
    SET_MODE(SDT_LOAD_PRE_UP) \
  } else if (IS_STORE_PRE_UP) { \
    SET_MODE(SDT_STORE_PRE_UP) \
  } else if (IS_LOAD_PRE_DOWN) { \
    SET_MODE(SDT_LOAD_PRE_DOWN) \
  } else if (IS_STORE_PRE_DOWN) { \
    SET_MODE(SDT_STORE_PRE_DOWN) \
  } else if (IS_LOAD_POST_UP) { \
    SET_MODE(SDT_LOAD_POST_UP) \
  } else if (IS_STORE_POST_UP) { \
    SET_MODE(SDT_STORE_POST_UP) \
  } else if (IS_LOAD_POST_DOWN) { \
    SET_MODE(SDT_LOAD_POST_DOWN) \
  } else if (IS_STORE_POST_DOWN) { \
    SET_MODE(SDT_STORE_POST_DOWN) \
  } else { \
    SET_MODE(INVALID_INSTRUCTION) \
  }

/* ******************************************************************** */
/*                   MACRO USED TO DETERMINE MUL TYPE                   */
/* ******************************************************************** */

#define SET_MUL_MODE() \
  uint32_t accumulate = (execute_variables.ir >> POS_ACC_BIT) & BITMASK_RIGHT_1_BIT; \
  if (accumulate) { \
    SET_MODE(MUL_ACC) \
  } else { \
    SET_MODE(MUL_NORMAL) \
  }

/* ******************************************************************** */
/*                   MACRO USED TO DETERMINE DP TYPE                    */
/* ******************************************************************** */

#define SET_DP_MODE() \
    uint32_t op_code = (execute_variables.ir >> POS_OP_CODE) & BITMASK_RIGHT_4_BITS; \
  switch (op_code) { \
    case IS_AND: \
      SET_MODE(DP_AND) \
      break; \
    case IS_EOR: \
      SET_MODE(DP_EOR) \
      break; \
    case IS_SUB: \
      SET_MODE(DP_SUB) \
      break; \
    case IS_RSUB: \
      SET_MODE(DP_RSUB) \
      break; \
    case IS_ADD: \
      SET_MODE(DP_ADD) \
      break; \
    case IS_TEST: \
      SET_MODE(DP_TEST) \
      break; \
    case IS_TEQ: \
      SET_MODE(DP_TEQ) \
      break; \
    case IS_CMP: \
      SET_MODE(DP_CMP) \
      break; \
    case IS_OR: \
      SET_MODE(DP_OR) \
      break; \
    case IS_MOV: \
      SET_MODE(DP_MOV) \
      break; \
    default: \
      SET_MODE(INVALID_INSTRUCTION) \
  }

/* ******************************************************************** */
/*                  MACROS USED TO REPRESENT BIT MASKS                  */
/* ******************************************************************** */

#define BITMASK_RIGHT_8_BITS 0x000000FF
#define BITMASK_MIDDLE_RIGHT_8_BITS 0x0000FF00
#define BITMASK_LEFT_8_BITS 0xFF000000
#define BITMASK_MIDDLE_LEFT_8_BITS 0x00FF0000
#define BITMASK_RIGHT_1_BIT 0x00000001
#define BITMASK_RIGHT_2_BITS 0x00000003
#define BITMASK_RIGHT_4_BITS 0x0000000F
#define BITMASK_RIGHT_5_BITS 0x0000001F
#define BITMASK_RIGHT_12_BITS 0x00000FFF
#define BITMASK_RIGHT_24_BITS 0x00FFFFFF
#define BITMASK_RIGHT_32_BITS 0xFFFFFFFF
#define BITMASK_LEFT_6_BITS 0xFC000000

/* ******************************************************************** */
/*                MACROS USED TO REPRESENT SHIFT AMOUNTS                */
/* ******************************************************************** */
#define POS_IMM_ROTATE 8
#define POS_COND 28
#define POS_FIRST_REG 16
#define POS_SECOND_REG 12
#define POS_THIRD_REG 8
#define POS_S_BIT 20
#define POS_ACC_BIT 21
#define POS_OP_CODE 21
#define POS_SHIFT_TYPE 5
#define POS_SHIFT_REGISTER 8
#define POS_REG_SHIFT 7
#define POS_V_BIT 28
#define POS_Z_BIT 30
#define POS_N_BIT 31

/* ******************************************************************** */
/*            MACRO USED FOR MEMORY LOCATION ERROR HANDLING             */
/* ******************************************************************** */

#define CHECK_LOCATION_ERROR() \
  if (location == 0x20200008) { \
    printf("One GPIO pin from 20 to 29 has been accessed\n"); \
    machine_state.reg[execute_variables.d] = 0x20200008; \
    break; \
  } else if (location == 0x20200004) { \
    printf("One GPIO pin from 10 to 19 has been accessed\n"); \
    machine_state.reg[execute_variables.d] = 0x20200004; \
    break; \
  } else if (location == 0x20200000) { \
    printf("One GPIO pin from 0 to 9 has been accessed\n"); \
    machine_state.reg[execute_variables.d] = 0x20200000; \
    break; \
  } else if (location == 0x20200028) { \
    printf("PIN OFF\n"); \
    break; \
  } else if (location == 0x2020001c) { \
    printf("PIN ON\n"); \
    break; \
  } else if (location < 0 || location >= MAX_MEM_SIZE) { \
    printf("Error: Out of bounds memory access at address 0x%08x\n", location); \
    break; \
  }

/* ******************************************************************** */
/*        MACRO USED TO DETERMINE CONDITION FROM INSTRUCTION            */
/* ******************************************************************** */

#define SET_CONDITION() \
  execute_variables.cond = execute_variables.ir >> POS_COND;

/* ******************************************************************** */
/*        MACRO USED TO COMPUTE OP2 WHEN DECODING DP INSTRUCTION        */
/* ******************************************************************** */

#define COMPUTE_DP_OP2() \
  if (IS_I_SET) { \
    compute_op2_immediate(); \
  } else { \
    compute_op2_shifted_reg(); \
  }

/* ******************************************************************** */
/*      MACRO USED TO COMPUTE OFFSET WHEN DECODING SDT INSTRUCTION      */
/* ******************************************************************** */

#define COMPUTE_SDT_OFFSET() \
  if (IS_I_SET) { \
    compute_op2_shifted_reg(); \
    execute_variables.SDT_offset = execute_variables.op2; \
  } else { \
      execute_variables.SDT_offset = execute_variables.ir & BITMASK_RIGHT_12_BITS; \
  }

/* ******************************************************************** */
/*             MACROS USED WHEN DECODING BRANCH INSTRUCTIONS            */
/* ******************************************************************** */

#define BRANCH_OFFSET_SIGNED (execute_variables.branch_offset & 0x02000000) != 0

#define SET_BRANCH_OFFSET() \
  execute_variables.branch_offset = (execute_variables.ir & BITMASK_RIGHT_24_BITS); \
  execute_variables.branch_offset = execute_variables.branch_offset << 2; \
   \
  if (BRANCH_OFFSET_SIGNED) { \
    uint32_t leading_ones = BITMASK_LEFT_6_BITS; \
    execute_variables.branch_offset = execute_variables.branch_offset | leading_ones; \
  }

/* ******************************************************************** */
/*           MACRO USED TO CHECK IF CONDITIONS ARE SATISFIED            */
/* ******************************************************************** */

#define CHECK_CONDITIONS() \
  switch(execute_variables.cond) { \
    case EQ: \
      if((CPSR & (1 << POS_Z_BIT)) == 0) { \
        decode(); \
        return; \
      } \
      break; \
    case NE: \
      if((CPSR & (1 << POS_Z_BIT)) != 0) { \
        decode(); \
        return; \
      } \
      break; \
    case GE: \
      if((CPSR & (1 << POS_N_BIT)) != (CPSR & (1 << POS_V_BIT))) { \
        decode(); \
        return; \
      } \
      break; \
    case LT: \
      if((CPSR & (1 << POS_N_BIT)) == (CPSR & (1 << POS_V_BIT))) { \
        decode(); \
        return; \
      } \
      break; \
    case GT: \
      if(((CPSR & (1 << POS_Z_BIT)) != 0) || ((CPSR & (1 << POS_N_BIT)) != (CPSR & (1 << POS_V_BIT)))) { \
        decode(); \
        return; \
      } \
      break; \
    case LE: \
      if(((CPSR & (1 << POS_Z_BIT)) == 0) && ((CPSR & (1 << POS_N_BIT)) == (CPSR & (1 << POS_V_BIT)))) { \
        decode(); \
        return; \
      } \
      break; \
    case AL: \
      break; \
    default: \
      printf("Error: Invalid condition specified at address 0x%08x\n", PC - 8); \
      break; \
  }

/* ******************************************************************** */
/*                  MACROS USED TO SET THE CPSR FLAGS                   */
/* ******************************************************************** */

#define SET_CPSR_C_TO_0() CPSR = CPSR & 0xDFFFFFFF;

#define SET_CPSR_C_TO_1() CPSR = CPSR | 0x20000000;

#define SET_CPSR_C_LOGICAL() \
  if(execute_variables.s_flag != 0) { \
    if(execute_variables.c_bit == 1) { \
      SET_CPSR_C_TO_1() \
    } else { \
      SET_CPSR_C_TO_0() \
    } \
  }

#define SET_CPSR_C_ADD_MULT() \
  if(execute_variables.s_flag != 0) { \
     if ((result_64 & 0xFFFFFFFF00000000) != 0) { \
        SET_CPSR_C_TO_1() \
      } else { \
        SET_CPSR_C_TO_0() \
      } \
  }

#define SET_CPSR_C_SUB() \
  if (execute_variables.s_flag != 0) { \
    if (execute_variables.op2 > *rn) { \
      SET_CPSR_C_TO_0() \
    } else { \
      SET_CPSR_C_TO_1() \
    } \
  }

#define SET_CPSR_C_RSUB() \
  if (execute_variables.s_flag != 0) { \
    if (execute_variables.op2 < *rn) { \
      SET_CPSR_C_TO_0() \
    } else { \
      SET_CPSR_C_TO_1() \
    } \
  }

#define SET_CPSR_Z_AND_N() \
  if(execute_variables.s_flag != 0) { \
    /* Sets the Z bit */ \
    if (result_32 == 0) { \
      CPSR = CPSR | (1 << 30); \
    } else { \
      CPSR = CPSR & (0xBFFFFFFF); \
    } \
 \
    /* Sets the N bit */ \
    most_sig_bit = result_32 >> 31; \
    if (most_sig_bit == 0) { \
      CPSR = CPSR & 0x7FFFFFFF; \
    } else { \
      CPSR = CPSR | 0x80000000; \
    } \
  }

#endif
