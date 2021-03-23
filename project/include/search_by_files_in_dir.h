#ifndef IZ_2_SEARCH_BY_FILES_IN_DIR_H
#define IZ_2_SEARCH_BY_FILES_IN_DIR_H
#include <regex.h>
struct {
  char *filename;
  int num_of_word_occur;
  struct file_and_num_of_word_occur *next;
} file_and_num_of_word_occur;

struct file_and_num_of_word_occur *create_sorted_list_of_files_containing_word(
    char *directory_name, char *search_word, regex_t *file_pattern,
    size_t max_path);
// int delete_list_of_file_and_num_of_word_occur
#endif // IZ_2_SEARCH_BY_FILES_IN_DIR_H
