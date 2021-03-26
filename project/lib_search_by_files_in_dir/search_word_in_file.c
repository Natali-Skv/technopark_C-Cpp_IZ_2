#include <ctype.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1

//функция пропуска комментариев
static int skip_comments(FILE *file);

int prefix_search_in_file(const char *filename, const char *pattern) {
  if (!pattern) {
    return ERROR;
  }
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "error of opening file %s", filename);
    return ERROR;
  }

  size_t pattern_len = strlen(pattern);
  size_t *pi = (size_t *)malloc(pattern_len * sizeof(size_t));
  pi[0] = 0u;
  char *pattern_low_case = (char *)malloc((pattern_len + 1) * sizeof(char));
  for (size_t i = 0u; (pattern_low_case[i] = tolower(pattern[i])); ++i) {
  }

  int num_of_occur = 0;

  for (size_t i = 1u; i < pattern_len; ++i) {
    size_t j = pi[i - 1u];
    while ((j > 0u) && (pattern_low_case[i] != pattern_low_case[j])) {
      j = pi[j - 1u];
    }
    if (pattern_low_case[i] == pattern_low_case[j]) {
      ++j;
    }
    pi[i] = j;
  }

  size_t j = 0u;
  int curr_char = 0;

  for (size_t i = 0u; (curr_char = getc(file)) != EOF; ++i) {
    while ((j > 0u) && tolower((char)curr_char) != pattern_low_case[j]) {
      j = pi[j - 1u];
    }
    if (tolower((char)curr_char) == pattern_low_case[j]) {
      ++j;
    }
    if (j == pattern_len) {
      ++num_of_occur;
    }

    if ((char)curr_char == '/') {
      if (skip_comments(file) == EOF) {
        break;
      }
    }
  }

  if (fclose(file) == EOF) {
    fprintf(stderr, "error of closing file %s", filename);
  }
  free(pi);
  free(pattern_low_case);
  return num_of_occur;
}

static int skip_comments(FILE *file) {
  int curr_char = 0;
  if ((curr_char = getc(file)) == EOF) {
    return EOF;
  }
  if ((char)curr_char == '/') {
    while (((curr_char = getc(file)) != EOF) && (char)curr_char != '\n') {
    }
  } else if ((char)curr_char == '*') {
    while ((curr_char = getc(file)) != EOF) {
      if ((char)curr_char == '*') {
        if (((curr_char = getc(file)) == EOF) || ((char)curr_char == '/')) {
          break;
        }
        ungetc(curr_char, file);
      }
    }
  } else {
    ungetc(curr_char, file);
  }
  return curr_char;
}
