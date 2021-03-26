#ifndef PROJECT_INCLUDE_SEARCH_BY_FILES_IN_DIR_H_
#define PROJECT_INCLUDE_SEARCH_BY_FILES_IN_DIR_H_
#include <regex.h>
typedef struct file_and_num_of_word_occur {
  char *filename;
  int num_of_word_occur;
  struct file_and_num_of_word_occur *next;
} file_and_num_of_word_occur;

int prefix_search_in_file(const char *filename, const char *pattern);
struct file_and_num_of_word_occur *create_sorted_list_of_files_containing_word(
    char *directory_name, char *search_word, regex_t *file_pattern,
    size_t max_path);

file_and_num_of_word_occur *
merge_two_sorted_lists(file_and_num_of_word_occur *head_1,
                       file_and_num_of_word_occur *head_2);
void delete_list_of_files_of_files_containing_word(
    file_and_num_of_word_occur *head);
#endif  // PROJECT_INCLUDE_SEARCH_BY_FILES_IN_DIR_H_
