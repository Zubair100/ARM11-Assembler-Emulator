#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "utilities.h"
#include "tokeniser.h"
#include "instruction_macros.h"
#include "dp_instructions.h"

uint32_t generate_SPECIAL_andeq(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns zero which represents HALT

  return 0x00000000;
}

uint32_t generate_SPECIAL_lsl(char **line, uint32_t line_no, uint32_t *length_of_program) {
  // PRE: No null pointers
  // POST: Returns the generated machine code

  char *shift = get_word(line, 0);
  char *rn = get_word(line, 1);
  char *expr = get_word(line, 2);
  char *modified_line[7] = {"mov", rn, rn, shift, expr, NULL, NULL};

  return generate_DP_mov(modified_line, line_no, length_of_program);
}
