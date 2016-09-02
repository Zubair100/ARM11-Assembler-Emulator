#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "emulate_macros.h"
#include "emulate.h"
#include "emulate_helper_functions.h"

struct arm11 machine_state;
struct pipeline execute_variables;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  clear_memory();
  clear_registers();
  load_memory(argv[1]);

  fetch();
  decode();
  while (INSTRUCTIONS_LEFT) {
    execute();
  }

  print_registers();
  print_memory();
  return EXIT_SUCCESS;
}

void clear_memory(void) {
  for (int i = 0; i < MAX_MEM_SIZE; i++) {
    machine_state.memory[i] = 0;
  }
}

void clear_registers(void) {
  for (int i = 0; i < MAX_REG_NO; i++) {
    machine_state.reg[i] = 0;
  }
}

void load_memory(char *ptr) {
  FILE *bin_file = fopen(ptr, "rb");
  fread(machine_state.memory, 1, MAX_MEM_SIZE, bin_file);
  fclose(bin_file);
}

void fetch(void) {
  execute_variables.ir = get_mem_value(PC);
  PC += MEM_ADDRESS_JUMP;
}

void decode(void) {
  if (IS_HALT) {
    SET_MODE(HALT)
    return;
  }

  SET_CONDITION()

  if (IS_BRANCH) {
    SET_MODE(BRANCH)
    SET_BRANCH_OFFSET()
  } else if (IS_SDT) {
    execute_variables.n = get_first_reg();
    execute_variables.d = get_second_reg();
    SET_SDT_MODE()
    COMPUTE_SDT_OFFSET()
  } else if (IS_MULT) {
    execute_variables.d = get_first_reg();
    execute_variables.n = get_second_reg();
    execute_variables.s = get_third_reg();
    execute_variables.m = get_fourth_reg();
    SET_MUL_MODE()
    set_s_flag();
  } else if (IS_DP) {
    SET_DP_MODE()
    set_s_flag();
    execute_variables.n = get_first_reg();
    execute_variables.d = get_second_reg();
    COMPUTE_DP_OP2()
  } else {
    SET_MODE(INVALID_INSTRUCTION)
  }
  fetch();
}

void execute(void) {
  if (execute_variables.mode == HALT) {
    execute_variables.halt_flag = 1;
    decode();
    fetch();
    return;
  }

  CHECK_CONDITIONS()

  uint32_t *rn = &machine_state.reg[execute_variables.n];
  uint32_t *rd = &machine_state.reg[execute_variables.d];
  uint32_t result_32;
  uint64_t result_64;
  uint32_t location;
  uint32_t reg_value;
  uint32_t mem_value;
  int32_t new_instruction;
  uint32_t most_sig_bit;

  switch (execute_variables.mode) {
    case BRANCH:
      new_instruction = PC;
      new_instruction += execute_variables.branch_offset;
      PC = new_instruction;
      fetch();
      break;
    case DP_AND:
      result_32 = *rn & execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      *rd = result_32;
      break;
    case DP_EOR:
      result_32 = *rn ^ execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      *rd = result_32;
      break;
    case DP_SUB:
      result_32 = *rn - execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_SUB()
      *rd = result_32;
      break;
    case DP_RSUB:
      result_32 = execute_variables.op2 - *rn;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_RSUB()
      *rd = result_32;
      break;
    case DP_ADD:
      result_64 = *rn + execute_variables.op2;
      result_32 = result_64;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_ADD_MULT()
      *rd = result_64;
      break;
    case DP_TEST:
      result_32 = *rn & execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      break;
    case DP_TEQ:
      result_32 = *rn ^ execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      break;
    case DP_CMP:
      result_32 = *rn - execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_SUB()
      break;
    case DP_OR:
      result_32 = *rn | execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      *rd = result_32;
      break;
    case DP_MOV:
      result_32 = execute_variables.op2;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_LOGICAL()
      *rd = result_32;
      break;
    case SDT_LOAD_PRE_UP:
      location = machine_state.reg[execute_variables.n] + execute_variables.SDT_offset;
      CHECK_LOCATION_ERROR()
      mem_value = get_mem_value(location);
      machine_state.reg[execute_variables.d] = mem_value;
      break;
    case SDT_STORE_PRE_UP:
      location = machine_state.reg[execute_variables.n] + execute_variables.SDT_offset;
      CHECK_LOCATION_ERROR()
      reg_value = machine_state.reg[execute_variables.d];
      set_mem_value(location, reg_value);
      break;
    case SDT_LOAD_POST_UP:
      location = machine_state.reg[execute_variables.n];
      CHECK_LOCATION_ERROR()
      mem_value = get_mem_value(location);
      machine_state.reg[execute_variables.d] = mem_value;
      machine_state.reg[execute_variables.n] += execute_variables.SDT_offset;
      break;
    case SDT_STORE_POST_UP:
      location = machine_state.reg[execute_variables.n];
      CHECK_LOCATION_ERROR()
      reg_value = machine_state.reg[execute_variables.d];
      set_mem_value(location, reg_value);
      machine_state.reg[execute_variables.n] += execute_variables.SDT_offset;
      break;
    case SDT_LOAD_PRE_DOWN:
      location = machine_state.reg[execute_variables.n] - execute_variables.SDT_offset;
      CHECK_LOCATION_ERROR()
      mem_value = get_mem_value(location);
      machine_state.reg[execute_variables.d] = mem_value;
      break;
    case SDT_STORE_PRE_DOWN:
      location = machine_state.reg[execute_variables.n] - execute_variables.SDT_offset;
      CHECK_LOCATION_ERROR()
      reg_value = machine_state.reg[execute_variables.d];
      set_mem_value(location, reg_value);
      break;
    case SDT_LOAD_POST_DOWN:
      location = machine_state.reg[execute_variables.n];
      CHECK_LOCATION_ERROR()
      mem_value = get_mem_value(location);
      machine_state.reg[execute_variables.d] = mem_value;
      machine_state.reg[execute_variables.n] -= execute_variables.SDT_offset;
      break;
    case SDT_STORE_POST_DOWN:
      location = machine_state.reg[execute_variables.n];
      CHECK_LOCATION_ERROR()
      reg_value = machine_state.reg[execute_variables.d];
      set_mem_value(location, reg_value);
      machine_state.reg[execute_variables.n] -= execute_variables.SDT_offset;
      break;
    case MUL_ACC:
      result_64 = machine_state.reg[execute_variables.m]
                  * machine_state.reg[execute_variables.s]
                  + machine_state.reg[execute_variables.n];
      result_32 = result_64;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_ADD_MULT()
      machine_state.reg[execute_variables.d] = result_64;
      break;
    case MUL_NORMAL:
      result_64 = machine_state.reg[execute_variables.m]
                  * machine_state.reg[execute_variables.s];
      result_32 = result_64;
      SET_CPSR_Z_AND_N()
      SET_CPSR_C_ADD_MULT()
      machine_state.reg[execute_variables.d] = result_64;
      break;
    case INVALID_INSTRUCTION:
      printf("Error: Invalid instruction at address 0x%08x\n", PC - 8);
      break;
    default:
      assert("Execution should not reach this point" && 0);
  }

  decode();
}

void print_registers(void) {
  puts("Registers:");

  for (int i = 0; i < NUM_OF_GP_REGISTERS; i++) {
    printf("$%d%*s: ", i, 2 - (i / 10), "");
    printf("%*d", 10, (int32_t) machine_state.reg[i]);
    printf(" (0x%08x)\n", machine_state.reg[i]);
  }

  printf("PC  : ");
  printf("%*d", 10, (int32_t) PC);
  printf(" (0x%08x)\n", PC);

  printf("CPSR: ");
  printf("%*d", 10, (int32_t) CPSR);
  printf(" (0x%08x)\n", CPSR);
}

void print_memory(void) {
  puts("Non-zero memory:");

  uint32_t word_le;
  uint32_t word_be;

  for (int i = 0; i < MAX_MEM_SIZE; i += MEM_ADDRESS_JUMP) {
    word_be = get_mem_value(i);
    word_le = endian_switcher(word_be);

    if (word_le != 0) {
      printf("0x%08x: 0x%08x\n", i, word_le);
    }
  }
}
