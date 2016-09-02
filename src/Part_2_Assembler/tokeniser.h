// Structs
typedef struct {
  char* label;
  int16_t addr;
} label_addr_pair;

// Prototypes
char ***generate_token_tree(char *buffer, uint32_t *no_of_lines);
label_addr_pair **create_symbol_table(char *buffer, int *no_of_pairs,
                                                         uint32_t *no_of_instr);
void destroy_symbol_table(label_addr_pair **table, int no_of_pairs);
char ***tokenise(char* buffer, label_addr_pair **table, int no_of_pairs,
                                                          uint32_t no_of_instr);

char **get_line(char ***token_tree, int line_no);
char *get_word(char **line, int word_no);

label_addr_pair *create_label_addr_pair(char *label, int16_t addr);
void destroy_label_addr_pair(label_addr_pair *pair);
int cmp_label_addr_pair(label_addr_pair* one, label_addr_pair* two);

int is_label_to_replace(char *label, label_addr_pair **symbol_table,
                                                               int no_of_pairs);
int contains_newline(char *str);
void remove_colon(char *str);

void sort_table(label_addr_pair* ps[], int first_idx, int last_idx);
void swap(label_addr_pair **a, label_addr_pair **b);

char *get_offset(char *label, label_addr_pair **symbol_table, int no_of_pairs,
                                                        int current_line_index);

// Constants
#define MAX_NO_OF_OPERANDS 6
#define MEM_ADDRESS_JUMP 4
#define PC_OFFSET 8

// Macros
#define Insert_pair() \
  label_addr_pair *new_pair = create_label_addr_pair(curr_word, curr_addr); \
  pairs[*no_of_pairs] = new_pair; \
  *no_of_pairs += 1; \
  pairs = (label_addr_pair **) realloc(pairs, \
    (*no_of_pairs + 1) * sizeof(label_addr_pair *))

#define Curr_word_is_label \
  (curr_word[0] != '\0') && (curr_word[strlen(curr_word) - 1] == ':')

#define Clone_buffer() \
  char *buffer_clone= malloc(strlen(buffer) + 1); \
  \
  if (buffer_clone == NULL) { \
    fprintf(stderr, "Fatal error: Cloning buffer failed.\n"); \
    assert("Fatal error: Cloning buffer failed." && 0); \
  } \
  \
  strcpy(buffer_clone, buffer)

#define No_colon(s) \
  (s)[strlen(s) -1] != ':'

#define Go_to_next_instr_of_tree() \
  curr_line_idx += 1; \
  curr_word_idx = 0; \
  token_tree[curr_line_idx] = calloc(sizeof(char*), 7)

#define Remove_null_instructions() \
  while (tree[*no_of_instr - 1][1] == NULL) { \
    (*no_of_instr)--; \
  }
