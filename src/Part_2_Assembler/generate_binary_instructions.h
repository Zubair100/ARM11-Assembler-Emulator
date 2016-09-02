#ifndef GENERATE_BINARY_INSTRUCTIONS_H
#define GENERATE_BINARY_INSTRUCTIONS_H

/* ******************************************************************** */
/*        PROTOTYPES FOR GENERATE BINARY INSTRUCTIONS FUNCTIONS         */
/* ******************************************************************** */

typedef uint32_t (*gen_machine)(char **line, uint32_t line_no, uint32_t *length_of_program);

typedef struct {
    char* mneumonic;
    gen_machine fnc_ptr;
} assemble_functions;

uint32_t* generate_binary(uint32_t *number_of_lines, char ***token_ptr);
gen_machine determine_function(char *mneumonic, assemble_functions **array_of_pairs);
void sort_function_table(assemble_functions* ps[], int first_idx, int last_idx);
int cmp_function_pair(assemble_functions* one, assemble_functions* two);
assemble_functions **generate_array_of_functions(void);
void swap_functions(assemble_functions **a, assemble_functions **b);

#endif
