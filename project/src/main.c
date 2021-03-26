#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FORMAT_STR_LEN 50
#define BUF_FOR_STRTOUL_LEN 20
#define BUF_FOR_STRTOUL_LEN_FMT "19"
#define ERROR (-1)
#define OK 0

static int set_string_param(char **str_ptr);

int main(int argc, char *argv[]) {
  int core_num = 1;
  char *path_to_project = NULL;
  int path_max_len = 0;
  char *search_word = NULL;
  char *regexp_filename_str = NULL;

  printf("Got %d args\n", argc);

  int opt;
  char *opts = "-:p:c:w:r:l:";

  while ((opt = getopt(argc, argv, opts)) != -1) {
    switch (opt) {
    case 'p':
      if (path_to_project) {
        break;
      }
      if (!path_max_len) {
        printf("max length to file in project (option -l) expected before path "
               "to project (option -p)\n");
        break;
      }
      path_to_project = (char *)malloc((path_max_len + 1u) * sizeof(char));
      if (!path_to_project) {
        printf("max path to file too long or malloc() error has "
               "occurred\n");
        break;
      }
      strncpy(path_to_project, optarg, path_max_len + 1u);
      printf("path to your C-project: %s\n", path_to_project);
      break;
    case 'l':
      path_max_len = (int)strtol(optarg, NULL, 10);
      if (errno != ERANGE) {
        printf("max length of path to file in this project :%d", path_max_len);
      } else {
        printf("wrong path to file in this project\n");
      }
      break;
    case 'c':
      core_num = (int)strtol(optarg, NULL, 10);
      if (errno != ERANGE) {
        printf("number of CPU cores :%d", core_num);
      } else {
        printf("wrong number of CPU cores\n");
      }
      break;
    case 'w':
      if (search_word) {
        break;
      }
      size_t word_len = strlen(optarg);
      search_word = (char *)malloc((word_len + 1u) * sizeof(char));
      if (!search_word) {
        printf(
            "word for searching is too long or malloc() error has occurred\n");
        break;
      }
      strncpy(search_word, optarg, word_len);
      printf("searching word: %s\n", search_word);
      break;
    case 'r':
      if (regexp_filename_str) {
        break;
      }
      size_t regexp_len = strlen(optarg);
      regexp_filename_str =
          (char *)malloc((regexp_len + 1u) * sizeof(char));
      if (!regexp_filename_str) {
        printf(
            "regular expression is too long or malloc() error has occurred\n");
        break;
      }
      strncpy(regexp_filename_str, optarg, regexp_len);
      printf("searching word: %s\n", search_word);
      break;

    case '?':
      printf("Unknown option: %c\n", optopt);
      break;
    case ':':
      printf("Missing arg for %c\n", optopt);
      break;  // ':' in opts
    case 1:
      printf("Non-option arg: %s\n", optarg);
      break;  // '-' in opts

    default:
      break;
    }
  }

  if (!search_word) {
    printf("input info about search word\n");
    if (set_string_param(&search_word) == ERROR) {
      return ERROR;
    }
  }
  if (!path_to_project) {
    printf(
        "input max path length to file in this project and path to project \n");
    if ((path_max_len = set_string_param(&path_to_project)) == ERROR) {
      return ERROR;
    }
  }
  if (!regexp_filename_str) {
    printf("input info about regular expression for filename\n");
    if (set_string_param(&regexp_filename_str) == ERROR) {
      return ERROR;
    }
  }
  regex_t regexp;
  if (regcomp(&regexp, regexp_filename_str, 0)) {
    fprintf(stderr, "%s does not convert to regular expression",
            regexp_filename_str);
    return 0;
  }

  file_and_num_of_word_occur *head =
      create_sorted_list_of_files_containing_word(
          path_to_project, search_word, &regexp, (size_t)path_max_len);

  file_and_num_of_word_occur *iter = head;

  while (iter) {
    printf("%d  %s\n", iter->num_of_word_occur, iter->filename);
    iter = iter->next;
  }
  free(search_word);
  free(path_to_project);
  free(regexp_filename_str);
  regfree(&regexp);

  delete_list_of_files_of_files_containing_word(head);
  return OK;
}

static int set_string_param(char **str_ptr) {
  size_t str_len = 0u;
  char buf[BUF_FOR_STRTOUL_LEN];
  printf("\tinput length of string and the string:  ");
  if (scanf("%" BUF_FOR_STRTOUL_LEN_FMT "s", buf) == 1) {
    str_len = strtoul(buf, NULL, 10);
    if (errno != ERANGE) {
      char format[FORMAT_STR_LEN];
      snprintf(format, FORMAT_STR_LEN, "%%%zus", (str_len));
      *str_ptr = (char *)malloc((str_len + 1u) * sizeof(char));
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
  return str_len;
}
