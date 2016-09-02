#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "tokeniser.h"

#define TYPE label_addr_pair
#include "binary_search.h"
#undef TYPE

char ***generate_token_tree(char *buffer, uint32_t *no_of_instr) {
  // PRE: buffer is string representing entire program to be assembled.
  //      *no_of_instr = 0
  // POST: Pointer to "tree" representing program is output.
  //       Tree is an array of arrays of strings.
  //       Each array of strings represents an instruction.
  //       Each string represents a "word" in the instruction.
  //       *no_of_instr variable will be updated to reflect the number of
  //       instructions in the program.

  int no_of_pairs = 0;
  label_addr_pair **table = create_symbol_table(buffer,
                                      &no_of_pairs, no_of_instr);
  sort_table(table, 0, no_of_pairs - 1);
  char ***tree = tokenise(buffer, table, no_of_pairs, *no_of_instr);
  destroy_symbol_table(table, no_of_pairs);

  Remove_null_instructions()

  return tree;
}

label_addr_pair *create_label_addr_pair(char *label, int16_t addr) {
  label_addr_pair *pair = (label_addr_pair *) malloc(sizeof(label_addr_pair));

  assert(pair != NULL);

  pair->label = malloc(strlen(label) + 1);
  if ((pair->label) != NULL) {
    strcpy((pair->label), (label));
  }
  pair->addr = addr;

  return pair;
}

void destroy_label_addr_pair(label_addr_pair *pair) {
  assert(pair != NULL);

  free(pair->label);
  free(pair);
}

int cmp_label_addr_pair(label_addr_pair* one, label_addr_pair* two) {
  // PRE: Pointers are non-NULL. Labels in pairs are non-NULL.
  // POST: If the label from pair one is lexographically less than the label
  //       from pair two then output is a negative integer.
  //       If the label from pair one is lexographically greater than the label
  //       from pair two then output is a positive integer.
  //       If strings are equal then output is zero.

  char* label_one = one->label;
  char* label_two = two->label;

  return strcmp(label_one, label_two);
}

label_addr_pair **create_symbol_table(char *buffer, int *no_of_pairs,
                                                        uint32_t *no_of_instr) {
  // PRE: buffer is string representing entire program to be assembled.
  //      *no_of_pairs = 0, *no_of_instr = 0.
  // POST: Array of pairs is output. Each pair consists of a label (string)
  //       with corresponding address (int16_t).
  //       *no_of_pairs updated to correspond to number of labels.
  //       *no_of_instr updated to correspond to number of instructions.

  Clone_buffer();

  // Initialise an array to contain label address pairs
  label_addr_pair **pairs = (label_addr_pair **)
                                          malloc(sizeof(label_addr_pair *));

  char *curr_word = strtok(buffer_clone, " ,"); // Find first word
  int16_t curr_addr = 0; // Initialise curr_addr

  while (curr_word != NULL) {
    int newline_idx = contains_newline(curr_word);

    while (newline_idx) {
      if (Curr_word_is_label) {
        remove_colon(curr_word);
        Insert_pair(); // to symbol table
      } else if (newline_idx == 1) {
        // do nothing
      } else {
        curr_addr += 4;
        (*no_of_instr)++;
      }

      curr_word += newline_idx;
      newline_idx = contains_newline(curr_word);
    }

    if (Curr_word_is_label) {
      remove_colon(curr_word);
      Insert_pair();
    }

    curr_word = strtok(NULL, " ,");
  }

  pairs = (label_addr_pair **)
                      realloc(pairs, *no_of_pairs * sizeof(label_addr_pair *));

  return pairs;
}

void destroy_symbol_table(label_addr_pair **table, int no_of_pairs) {
  for (int i = 0; i < no_of_pairs; i++) {
    destroy_label_addr_pair(table[i]);
  }
  free(table);
}

int contains_newline(char *str) {
  // PRE: None.
  // POST: If input string contains a newline character, output is index of
  //       newline character add one.
  //       Zero, otherwise.

  int length = strlen(str);

  for (int i = 0; i < length; i++) {
    if (str[i] == '\n') {
      str[i] = '\0';
      return i + 1;
    }
  }

  return 0;
}

void remove_colon(char *str) {
  // PRE: String has ':' in last position
  // POST: ':' replaced by '/0'.

  int length = strlen(str);
  str[length - 1] = '\0';
}

void sort_table(label_addr_pair* ps[], int first_idx, int last_idx) {

  if (first_idx < last_idx) {
    int pivot_idx = first_idx;
    int i = first_idx;
    int j = last_idx;

    while (i < j) {
      while (cmp_label_addr_pair(ps[i], ps[pivot_idx]) <= 0 && i < last_idx) {
        i++;
      }
      while (cmp_label_addr_pair(ps[j], ps[pivot_idx]) > 0) {
        j--;
      }
      if (i < j) {
        swap(&ps[i], &ps[j]);
      }
   }

   swap(&ps[pivot_idx], &ps[j]);

   sort_table(ps, first_idx, j - 1);
   sort_table(ps, j + 1, last_idx);
  }
}

void swap(label_addr_pair **a, label_addr_pair **b) {
  label_addr_pair *temp = *a;
  *a = *b;
  *b = temp;
}

int is_label_to_replace(char *label, label_addr_pair **symbol_table,
                                                          int no_of_pairs) {
  // PRE: None.
  // POST: Returns one if label appears in symbol table.
  //       Zero, otherwise.

  label_addr_pair *temp_pair = create_label_addr_pair(label, 0);
  int lookup_idx = binary_search_label_addr_pair(symbol_table, temp_pair,
                                       cmp_label_addr_pair, 0, no_of_pairs - 1);
  destroy_label_addr_pair(temp_pair);

  if (lookup_idx == -1) {
    return 0;
  } else {
    return 1;
  }
}

char *get_offset(char *label, label_addr_pair **symbol_table, int no_of_pairs,
                                                       int current_line_index) {
  // PRE: label appears with corresponding address in symbol_table.
  // POST: Offset between label's address and current PC is output.
  //       (Taking PC + 8 offset into account).

  label_addr_pair *temp_pair = create_label_addr_pair(label, 0);
  int lookup_idx = binary_search_label_addr_pair(symbol_table, temp_pair,
                                           cmp_label_addr_pair, 0, no_of_pairs);
  label_addr_pair *found_pair = symbol_table[lookup_idx];

  int addr = found_pair->addr;
  int curr_PC = (current_line_index * MEM_ADDRESS_JUMP) + PC_OFFSET;
  int length = snprintf(NULL, 0, "%d", addr - curr_PC);

  char *offset = malloc((length+ 1) * sizeof(char));
  snprintf(offset, length +1, "%d", addr - curr_PC);

  destroy_label_addr_pair(temp_pair);

  return offset;
}

char ***tokenise(char* buffer, label_addr_pair **table, int no_of_pairs,
                                                         uint32_t no_of_instr) {
  // PRE: buffer is string representing entire program to be assembled.
  //      table contains all labels that appear in program with corresponding
  //      addresses.
  //      no_of_pairs is equal to number of pairs in table.
  //      no_of_instr is equal to number of instructions in programs.
  // POST: Pointer to "tree" representing program is output.
  //       Tree is an array of arrays of strings.
  //       Each array of strings represents an instruction.
  //       Each string represents a "word" in the instruction.
  //       Labels in branch instructions will be replaced with appropriate
  //       offsets. (Taking into account the PC + 8 offset).

  char ***token_tree = (char ***) malloc(no_of_instr * sizeof(char **));

  char *curr_word = strtok(buffer, " ,"); // get first word

  int curr_line_idx = 0;
  int curr_word_idx = 0;
  token_tree[curr_line_idx] = calloc(sizeof(char*), MAX_NO_OF_OPERANDS + 1);

  while (curr_word != NULL) {
     int newline_idx = contains_newline(curr_word);

     while (newline_idx) {
      if (is_label_to_replace(curr_word, table, no_of_pairs)) {
        char* offset = get_offset(curr_word, table, no_of_pairs,
                                                             curr_line_idx);
        token_tree[curr_line_idx][curr_word_idx] = offset;
        Go_to_next_instr_of_tree();
      } else if (newline_idx == 1) {
        // do nothing
      } else if (No_colon(curr_word)) {
        token_tree[curr_line_idx][curr_word_idx] = curr_word;
        Go_to_next_instr_of_tree();
      }

     curr_word += newline_idx;
     newline_idx = contains_newline(curr_word);
     }

     if ((No_colon(curr_word))) {
      token_tree[curr_line_idx][curr_word_idx] = curr_word;
      curr_word_idx += 1;
     }

     curr_word = strtok(NULL, " ,");
  }

  return token_tree;
}

char **get_line(char ***token_tree, int line_no) {
  return token_tree[line_no];
}

char *get_word(char **line, int word_no) {
  return line[word_no];
}
