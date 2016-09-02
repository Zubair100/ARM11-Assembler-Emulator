#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "dp_instructions.h"
#include "sdt_instructions.h"
#include "branch_instructions.h"
#include "mul_instructions.h"
#include "special_instructions.h"
#include "instruction_macros.h"
#include "utilities.h"
#include "tokeniser.h"
#include "global_variables.h"

typedef uint32_t (*gen_machine)(char **line, uint32_t line_no, uint32_t *length_of_program);

typedef struct {
    char* mneumonic;
    gen_machine fnc_ptr;
} assemble_functions;

#define TYPE assemble_functions
#include "binary_search.h"
#undef TYPE

assemble_functions **generate_array_of_functions(void);
gen_machine determine_function(char *mneumonic, assemble_functions **array_of_pairs);
void swap_functions(assemble_functions **a, assemble_functions **b);
void sort_function_table(assemble_functions* ps[], int first_idx, int last_idx);
int cmp_function_pair(assemble_functions* one, assemble_functions* two);
assemble_functions **generate_array_of_functions(void);

uint32_t *machine_code;

uint32_t* generate_binary(uint32_t *number_of_lines, char ***token_ptr) {
  // PRE: Pointer is not null
  // POST: Generate binary encoding and write it to an array of ints from token tree

  machine_code = malloc(sizeof(uint32_t) * (*number_of_lines));
  assemble_functions **array_of_pairs = generate_array_of_functions();
  sort_function_table(array_of_pairs, 0, TOTAL_NO_OF_MNEUMONICS - 1);
  char **current_line;
  char *first_word;
  uint32_t no_of_instructions = *number_of_lines;

  for (uint32_t i = 0; i < no_of_instructions; i++) {
    current_line = get_line(token_ptr, i);
    first_word = get_word(current_line, 0);
    gen_machine generate_function = determine_function(first_word, array_of_pairs);
    uint32_t test = generate_function(current_line, i, number_of_lines);
    machine_code[i] = test;
  }

  return machine_code;
}

gen_machine determine_function(char *mneumonic, assemble_functions **array_of_pairs) {
  // PRE: Mneumonic exists in the array of pairs
  // POST: Returns pointer to corresponding matcing function

  assemble_functions temp_pair;
  temp_pair.mneumonic = mneumonic;
  temp_pair.fnc_ptr = generate_SDT_ldr;
  uint32_t idx = binary_search_assemble_functions(array_of_pairs, &temp_pair, cmp_function_pair, 0, TOTAL_NO_OF_MNEUMONICS - 1);
  assemble_functions *found_struct = array_of_pairs[idx];
  return found_struct->fnc_ptr;
}

void swap_functions(assemble_functions **a, assemble_functions **b) {
  // PRE: Pointers are not null
  // POST: Functions pointed at are swapped

  assemble_functions *temp = *a;
  *a = *b;
  *b = temp;
}


void sort_function_table(assemble_functions* ps[], int first_idx, int last_idx) {
  // PRE: On the first call, first_idx must be zero and last_idx must be the
  //      total number of mneumonics - 1
  // POST: Mneumonic function pairs are arranged in lexicographical order of
  //       mneumonics

  if (first_idx < last_idx) {
    int pivot_idx = first_idx;
    int i = first_idx;
    int j = last_idx;

    while (i < j) {
      while (cmp_function_pair(ps[i], ps[pivot_idx]) <= 0 && i < last_idx) {
        i++;
      }

      while (cmp_function_pair(ps[j], ps[pivot_idx]) > 0) {
        j--;
      }

      if (i < j) {
        swap_functions(&ps[i], &ps[j]);
      }
    }

    swap_functions(&ps[pivot_idx], &ps[j]);
    sort_function_table(ps, first_idx, j - 1);
    sort_function_table(ps, j + 1, last_idx);
  }
}

int cmp_function_pair(assemble_functions* one, assemble_functions* two) {
  // PRE: Pointers and mneumonics in pairs are not NULL
  // POST: If the mneumonic from pair one is lexographically less than the
  //       neumonic from pair two then output is a negative integer.
  //       If the mneumonic from pair one is lexographically greater than the
  //       mneumonic from pair two then output is a positive integer.
  //       If strings are equal then output is zero.

  char* mneumonic_one = one->mneumonic;
  char* mneumonic_two = two->mneumonic;
  return strcmp(mneumonic_one, mneumonic_two);
}

assemble_functions **generate_array_of_functions(void) {
  // PRE: None
  // POST: Generates array of string-function pairs

    assemble_functions **array_of_pairs = malloc(sizeof(assemble_functions *) * TOTAL_NO_OF_MNEUMONICS);

    assemble_functions add_struct;
    add_struct.mneumonic = "add";
    add_struct.fnc_ptr = generate_DP_add;
    assemble_functions *add_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *add_struct_ptr = add_struct;
    array_of_pairs[0] = add_struct_ptr;

    assemble_functions sub_struct;
    sub_struct.mneumonic = "sub";
    sub_struct.fnc_ptr = generate_DP_sub;
    assemble_functions *sub_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *sub_struct_ptr = sub_struct;
    array_of_pairs[1] = sub_struct_ptr;

    assemble_functions rsub_struct;
    rsub_struct.mneumonic = "rsb";
    rsub_struct.fnc_ptr = generate_DP_rsub;
    assemble_functions *rsub_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *rsub_struct_ptr = rsub_struct;
    array_of_pairs[2] = rsub_struct_ptr;

    assemble_functions and_struct;
    and_struct.mneumonic = "and";
    and_struct.fnc_ptr = generate_DP_and;
    assemble_functions *and_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *and_struct_ptr = and_struct;
    array_of_pairs[3] = and_struct_ptr;

    assemble_functions eor_struct;
    eor_struct.mneumonic = "eor";
    eor_struct.fnc_ptr = generate_DP_eor;
    assemble_functions *eor_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *eor_struct_ptr = eor_struct;
    array_of_pairs[4] = eor_struct_ptr;

    assemble_functions or_struct;
    or_struct.mneumonic = "orr";
    or_struct.fnc_ptr = generate_DP_or;
    assemble_functions *or_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *or_struct_ptr = or_struct;
    array_of_pairs[5] = or_struct_ptr;

    assemble_functions mov_struct;
    mov_struct.mneumonic = "mov";
    mov_struct.fnc_ptr = generate_DP_mov;
    assemble_functions *mov_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *mov_struct_ptr = mov_struct;
    array_of_pairs[6] = mov_struct_ptr;

    assemble_functions tst_struct;
    tst_struct.mneumonic = "tst";
    tst_struct.fnc_ptr = generate_DP_tst;
    assemble_functions *tst_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *tst_struct_ptr = tst_struct;
    array_of_pairs[7] = tst_struct_ptr;

    assemble_functions teq_struct;
    teq_struct.mneumonic = "teq";
    teq_struct.fnc_ptr = generate_DP_teq;
    assemble_functions *teq_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *teq_struct_ptr = teq_struct;
    array_of_pairs[8] = teq_struct_ptr;

    assemble_functions cmp_struct;
    cmp_struct.mneumonic = "cmp";
    cmp_struct.fnc_ptr = generate_DP_cmp;
    assemble_functions *cmp_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *cmp_struct_ptr = cmp_struct;
    array_of_pairs[9] = cmp_struct_ptr;

    assemble_functions mul_struct;
    mul_struct.mneumonic = "mul";
    mul_struct.fnc_ptr = generate_MUL_mul;
    assemble_functions *mul_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *mul_struct_ptr = mul_struct;
    array_of_pairs[10] = mul_struct_ptr;

    assemble_functions mla_struct;
    mla_struct.mneumonic = "mla";
    mla_struct.fnc_ptr = generate_MUL_mla;
    assemble_functions *mla_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *mla_struct_ptr = mla_struct;
    array_of_pairs[11] = mla_struct_ptr;

    assemble_functions ldr_struct;
    ldr_struct.mneumonic = "ldr";
    ldr_struct.fnc_ptr = generate_SDT_ldr;
    assemble_functions *ldr_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *ldr_struct_ptr = ldr_struct;
    array_of_pairs[12] = ldr_struct_ptr;

    assemble_functions str_struct;
    str_struct.mneumonic = "str";
    str_struct.fnc_ptr = generate_SDT_str;
    assemble_functions *str_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *str_struct_ptr = str_struct;
    array_of_pairs[13] = str_struct_ptr;

    assemble_functions beq_struct;
    beq_struct.mneumonic = "beq";
    beq_struct.fnc_ptr = generate_BRANCH_beq;
    assemble_functions *beq_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *beq_struct_ptr = beq_struct;
    array_of_pairs[14] = beq_struct_ptr;

    assemble_functions bne_struct;
    bne_struct.mneumonic = "bne";
    bne_struct.fnc_ptr = generate_BRANCH_bne;
    assemble_functions *bne_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *bne_struct_ptr = bne_struct;
    array_of_pairs[15] = bne_struct_ptr;

    assemble_functions bge_struct;
    bge_struct.mneumonic = "bge";
    bge_struct.fnc_ptr = generate_BRANCH_bge;
    assemble_functions *bge_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *bge_struct_ptr = bge_struct;
    array_of_pairs[16] = bge_struct_ptr;

    assemble_functions blt_struct;
    blt_struct.mneumonic = "blt";
    blt_struct.fnc_ptr = generate_BRANCH_blt;
    assemble_functions *blt_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *blt_struct_ptr = blt_struct;
    array_of_pairs[17] = blt_struct_ptr;

    assemble_functions bgt_struct;
    bgt_struct.mneumonic = "bgt";
    bgt_struct.fnc_ptr = generate_BRANCH_bgt;
    assemble_functions *bgt_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *bgt_struct_ptr = bgt_struct;
    array_of_pairs[18] = bgt_struct_ptr;


    assemble_functions ble_struct;
    ble_struct.mneumonic = "ble";
    ble_struct.fnc_ptr = generate_BRANCH_ble;
    assemble_functions *ble_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *ble_struct_ptr = ble_struct;
    array_of_pairs[19] = ble_struct_ptr;


    assemble_functions b_struct;
    b_struct.mneumonic = "b";
    b_struct.fnc_ptr = generate_BRANCH_b;
    assemble_functions *b_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *b_struct_ptr = b_struct;
    array_of_pairs[20] = b_struct_ptr;

    assemble_functions lsl_struct;
    lsl_struct.mneumonic = "lsl";
    lsl_struct.fnc_ptr = generate_SPECIAL_lsl;
    assemble_functions *lsl_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *lsl_struct_ptr = lsl_struct;
    array_of_pairs[21] = lsl_struct_ptr;

    assemble_functions andeq_struct;
    andeq_struct.mneumonic = "andeq";
    andeq_struct.fnc_ptr = generate_SPECIAL_andeq;
    assemble_functions *andeq_struct_ptr = malloc(sizeof(assemble_functions) * 1);
    *andeq_struct_ptr = andeq_struct;
    array_of_pairs[22] = andeq_struct_ptr;

    return array_of_pairs;
}
