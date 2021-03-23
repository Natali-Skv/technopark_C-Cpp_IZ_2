#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FORMAT_STR_LEN 50
#define BUF_FOR_STRTOUL_LEN 20
#define BUF_FOR_STRTOUL_LEN_FMT "19"
#define MAX_PATH_TO_FILE_LEN 1024u
#define ERROR -1
#define OK 0

static int set_string_param(char **str_ptr, size_t *str_len_ptr);

int main(int argc, char *argv[]) {
  int core_num = 1;
  char *path_to_project = NULL;
  size_t path_len = 0u;
  char *search_word = NULL;
  size_t word_len = 0u;
  char *regexp_filename_str = NULL;
  size_t regexp_len = 0u;

  printf("Got %d args\n", argc);

  int opt;
  char *opts = "-:p:c:w:r:";

  while ((opt = getopt(argc, argv, opts)) != -1) {
    switch (opt) {
    case 'p':
      path_len = strlen(optarg);
      path_to_project = (char *)malloc((path_len + 1u) * sizeof(char));
      if (!path_to_project) {
        printf("path to your C-project is too long or malloc() error has "
               "occurred\n");
        break;
      }
      memcpy(path_to_project, optarg, (path_len + 1u) * sizeof(char));
      printf("path to your C-project: %s\n", path_to_project);
      break;
    case 'c':
      core_num = (int)strtol(optarg, NULL, 10);
      if (errno != ERANGE) { // decimal number system
        printf("number of CPU cores :%d", core_num);
      } else {
        printf("wrong number of CPU cores\n");
      }
      break;
    case 'w':
      word_len = strlen(optarg);
      search_word = (char *)malloc((word_len + 1u) * sizeof(char));
      if (!search_word) {
        printf(
            "word for searching is too long or malloc() error has occurred\n");
        break;
      }
      memcpy(search_word, optarg, (word_len + 1u) * sizeof(char));
      printf("searching word: %s\n", search_word);
      break;
    case 'r':
      regexp_len = strlen(optarg);
      regexp_filename_str = (char *)malloc((regexp_len + 1u) * sizeof(char));
      if (!regexp_filename_str) {
        printf(
            "regular expression is too long or malloc() error has occurred\n");
        break;
      }
      memcpy(regexp_filename_str, optarg, (regexp_len + 1u) * sizeof(char));
      printf("searching word: %s\n", search_word);
      break;

    case '?':
      printf("Unknown option: %c\n", optopt);
      break;
    case ':':
      printf("Missing arg for %c\n", optopt);
      break; // ':' in opts
    case 1:
      printf("Non-option arg: %s\n", optarg);
      break; // '-' in opts
    }
  }

  if (!search_word) {
    printf("  input info about search word\n");
    if (set_string_param(&search_word, &word_len) == ERROR) {
      return ERROR;
    }
  }

  if (!path_to_project) {
    printf("  input info about path to project\n");
    if (set_string_param(&path_to_project, &path_len) == ERROR) {
      return ERROR;
    }
  }

  if (!regexp_filename_str) {
    printf("  input info about regular expression for filename\n");
    if (set_string_param(&regexp_filename_str, &regexp_len) == ERROR) {
      return ERROR;
    }
  }

  regex_t regexp;
  if (regcomp(&regexp, regexp_filename_str, 0)) {
    fprintf(stderr, "%s does not convert to regular expression",
            regexp_filename_str);
    return 0;
  }

  struct file_and_num_of_word_occur *head = NULL;

  head = create_sorted_list_of_files_containing_word(
      path_to_project, search_word, &regexp, (size_t)MAX_PATH_TO_FILE_LEN);
  if (head) {
    printf("OK");
  }
  // fprintf_list_of_ ();
  // delete_list_of ...()
  // regfree(&regexp);
  return OK;
}

static int set_string_param(char **str_ptr, size_t *str_len_ptr) {
  char buf[BUF_FOR_STRTOUL_LEN];
  printf("input length of string and the string:  ");
  if (scanf("%" BUF_FOR_STRTOUL_LEN_FMT "s", buf) == 1) {
    *str_len_ptr = strtoul(buf, NULL, 10);
    if (errno != ERANGE) {
      char format[FORMAT_STR_LEN];
      snprintf(format, FORMAT_STR_LEN, "%%%zus", (*str_len_ptr));
      *str_ptr = (char *)malloc((*str_len_ptr + 1u) * sizeof(char));
      if (*str_ptr) {
        if (scanf(format, *str_ptr) != 1) {
          perror("string scanf() error has occurred\n");
          return ERROR;
        }
      } else {
        perror("string malloc() error has occurred\n");
        scanf("%*s");
        return ERROR;
      }
    } else {
      perror("wrong input of string length\n");
      errno = 0;
      scanf("%*s");
      return ERROR;
    }
  } else {
    perror(" scanf() of string length error has occurred");
    return ERROR;
  }

  return OK;
}
