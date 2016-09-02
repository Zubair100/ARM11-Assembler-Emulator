#include <assert.h>

#define concat(x,y) x ## y

#define binary_search(TYPE) concat(binary_search_, TYPE)
#define sort_comparator(TYPE) concat(sort_comparator_, TYPE)

typedef int (*sort_comparator(TYPE))(TYPE *first, TYPE *second);

int binary_search(TYPE)(TYPE **array, TYPE *search_item, sort_comparator(TYPE) cmp, int start_index, int end_index);

int binary_search(TYPE)(TYPE **array, TYPE *search_item, sort_comparator(TYPE) cmp, int start_index, int end_index) {
  // PRE: No null pointers
  // POST: Performs binary search on array and returns index of found item,
  //       else -1 if item is not found

  if (start_index > end_index) {
    return -1;
  }

  int middle_index = (start_index + end_index) / 2;
  TYPE *current_item = array[middle_index];
  int comparison = cmp(current_item, search_item);

  if (comparison == 0) {
    return middle_index;
  } else if (comparison >= 1) {
    return binary_search(TYPE)(array, search_item, cmp, start_index, middle_index - 1);
  } else {
    return binary_search(TYPE)(array, search_item, cmp, middle_index + 1, end_index);
  }

  assert(!"Code should not reach this point");
  return -1;
}
