#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "tokeniser.h"
#include "generate_binary_instructions.h"
#include "assemble.h"
#include "instruction_macros.h"


int main(int argc, char **argv) {
  char *buffer = read_file(argv[1]);

  if (buffer == NULL) {
    fprintf(stderr, "Invalid file.\n");
    return 1;
  }

  uint32_t no_of_instr = 0;
  char ***token_tree = generate_token_tree(buffer, &no_of_instr);
  uint32_t *bin_array = generate_binary(&no_of_instr, token_tree);
  // Generate array of int32s from token tree
  // Note that these numbers are in big endian

  to_little_endian(bin_array, no_of_instr);

  write_file(argv[2], bin_array, no_of_instr);
  return EXIT_SUCCESS;
}

void write_file(char *filename, uint32_t *bin_array, uint32_t no_of_instr) {
  // PRE: No null pointers and must be a valid filename
  // POST: Writes contents of the array to the given file

  FILE *binary_file  = fopen(filename, "wb");
  fwrite(bin_array, 4, no_of_instr, binary_file);
  fclose(binary_file);
}

char *read_file(char *filename) {
  // PRE: No null pointers and must be a valid filename
  // POST: String containing file's contents returned. NULL if file doesn't exist

  char *buffer;
  long length;
  FILE *f = fopen(filename, "r");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);

    if (buffer) {
      fread(buffer, 1, length, f);
    }

    fclose(f);
    return buffer;
  } else {
    return NULL;
  }
}

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

void to_little_endian(uint32_t *bin_array, uint32_t length) {
  // PRE:  None.
  // POST: If run on big endian architecture, endianness of each uint32_t in
  //       bin_array will be reversed. Endianness will be left alone on little
  //       endian architecture.
  if (IS_BIG_ENDIAN) {
    for (int i = 0; i < length; i++) {
      bin_array[i] = endian_switcher(bin_array[i]);
    }
  }
}
