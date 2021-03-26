#include <dirent.h>
#include <errno.h>
#include <regex.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
// проверка сработал ли маллок

file_and_num_of_word_occur *
merge_two_sorted_lists(file_and_num_of_word_occur *head_1,
                       file_and_num_of_word_occur *head_2);

int regexec_with_perror(regex_t *regex, const char *str);

struct file_and_num_of_word_occur *
create_sorted_list_of_files_containing_word(char *dir_name, char *search_word,
                                            regex_t *file_name_regex,
                                            size_t max_path) {
  if (!dir_name || !search_word || !file_name_regex || !max_path) {
    fprintf(stderr, " expected not NULL pointers and max length above 0\n");
    return NULL;
  }
  struct file_and_num_of_word_occur *head = NULL;
  DIR *dir;
  struct dirent *entry;
  size_t dir_len = strlen(dir_name);

  if (!(dir = opendir(dir_name))) {
    fprintf(stderr, "path not found: %s: %s\n", dir_name, strerror(errno));
    return NULL;
  }

  while ((entry = readdir(dir)) != NULL) {
    char *name = entry->d_name;
    size_t name_len = strlen(name);
    if (dir_len + strlen(name) + 2u <= max_path) {
      if (entry->d_type == DT_DIR) {
        if (strcmp(name, ".") && strcmp(name, "..")) {
          dir_name[dir_len] = '/';
          strncpy(dir_name + dir_len + 1u, name, name_len + 1u);

          struct file_and_num_of_word_occur *next =
              create_sorted_list_of_files_containing_word(
                  dir_name, search_word, file_name_regex, max_path);

          head = merge_two_sorted_lists(head, next);
          dir_name[dir_len] = '\0';
        }

      } else if (entry->d_type == DT_REG) {
        if (!regexec_with_perror(file_name_regex, name)) {
          char *file_path =
              (char *)malloc((dir_len + name_len + 2u) * sizeof(char));
          strncpy(file_path, dir_name, dir_len + 1u);
          file_path[dir_len] = '/';
          strncpy(file_path + dir_len + 1u, name, name_len + 1u);

          int num_of_word_occur = prefix_search_in_file(file_path, search_word);
          if (num_of_word_occur > 0) {
            file_and_num_of_word_occur *curr =
                (file_and_num_of_word_occur *)malloc(
                    sizeof(file_and_num_of_word_occur));
            curr->filename = file_path;
            curr->num_of_word_occur = num_of_word_occur;
            curr->next = NULL;
            head = merge_two_sorted_lists(head, curr);
          } else {
            free(file_path);
          }
        }
      }
    } else {
      fprintf(stderr, "path too long: %s/%s\n", dir_name, name);
    }
  }
  closedir(dir);
  return head;
}

void delete_list_of_files_of_files_containing_word(
    file_and_num_of_word_occur *head) {
  file_and_num_of_word_occur *tmp = NULL;
  while (head) {
    tmp = head;
    head = head->next;
    free(tmp->filename);
    free(tmp);
  }
}

int regexec_with_perror(regex_t *regex, const char *str) {
  int result = regexec(regex, str, 0, NULL, 0);
  if (!result || result == REG_NOMATCH) {
    return result;
  } else {
    char msgbuf[100];
    regerror(result, regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match for %s failed: %s\n", str, msgbuf);
    return ERROR;
  }
}

file_and_num_of_word_occur *
merge_two_sorted_lists(file_and_num_of_word_occur *head_1,
                       file_and_num_of_word_occur *head_2) {
  if (!head_1 || !head_2) {
    return (head_1) ? head_1 : head_2;
  }
  if (head_1 == head_2) {
    return head_1;
  }

  file_and_num_of_word_occur *new_head = NULL;
  if (head_1->num_of_word_occur >= head_2->num_of_word_occur) {
    new_head = head_1;
    head_1 = head_1->next;
  } else {
    new_head = head_2;
    head_2 = head_2->next;
  }
  file_and_num_of_word_occur *iter = new_head;

  while (head_1 && head_2) {
    if (head_1->num_of_word_occur > head_2->num_of_word_occur) {
      iter->next = head_1;
      iter = iter->next;
      head_1 = head_1->next;
    } else {
      iter->next = head_2;
      iter = iter->next;
      head_2 = head_2->next;
    }
  }
  if (head_1) {
    iter->next = head_1;
  } else {
    iter->next = head_2;
  }
  return new_head;
}
