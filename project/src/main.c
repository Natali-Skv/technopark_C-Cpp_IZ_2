#include <errno.h>
#include <getopt.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FORMAT_STR_LEN 50
#define BUF_FOR_STRTOUL_LEN 20
#define BUF_FOR_STRTOUL_LEN_FMT "19"

int main(int argc, char *argv[]) {
  int core_num = 1;
  char *path_to_project = NULL;
  size_t path_len = 0;
  char *search_word = NULL;
  size_t word_len = 0;
  printf("Got %d args\n", argc);

  int opt;
  char *opts = "-:p:c:w:";

  while ((opt = getopt(argc, argv, opts)) != -1) {
    switch (opt) {
    case 'p':
      path_len = strlen(optarg);
      path_to_project = (char *)malloc((path_len + 1) * sizeof(char));
      if (!path_to_project) {
        printf("path to your C-project is too long or malloc() error has "
               "occurred\n");
        break;
      }
      memcpy(path_to_project, optarg, (path_len + 1) * sizeof(char));
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
      search_word = (char *)malloc((word_len + 1) * sizeof(char));
      if (!search_word) {
        printf(
            "word for searching is too long or malloc() error has occurred\n");
        break;
      }
      memcpy(path_to_project, optarg, (word_len + 1) * sizeof(char));
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
    default:
      break;
    }
  }

  if (!search_word) {
    char buf[BUF_FOR_STRTOUL_LEN];
    printf("input length of search word and the word: ");
    if (scanf("%" BUF_FOR_STRTOUL_LEN_FMT "s", buf) == 1) {
      word_len = strtoul(buf, NULL, 10);
      if (errno != ERANGE) {
        char format[FORMAT_STR_LEN];
        snprintf(format, FORMAT_STR_LEN, "%%%zus", (word_len));
        search_word = (char *)malloc((word_len + 1) * sizeof(char));
        if (search_word) {
          if (scanf(format, search_word) != 1) {
            perror("search word scanf() error has occurred\n");
          }
        } else {
          perror("search word malloc() error has occurred\n");
          scanf("%*s");
        }
      } else {
        perror("wrong input of search word length\n");
        scanf("%*s");
        errno = 0;
      }
    } else {
      perror(" scanf() of length of search word error has occurred");
    }
  }

  if (!path_to_project) {
    char buf[BUF_FOR_STRTOUL_LEN];
    printf("input length of path to project and the project: ");
    if (scanf("%" BUF_FOR_STRTOUL_LEN_FMT "s", buf) == 1) {
      path_len = strtoul(buf, NULL, 10);
      if (errno != ERANGE) {
        char format[FORMAT_STR_LEN];
        snprintf(format, FORMAT_STR_LEN, "%%%zus", (path_len));
        path_to_project = (char *)malloc((path_len + 1) * sizeof(char));
        if (path_to_project) {
          if (scanf(format, path_to_project) != 1) {
            perror("path to project scanf() error has occurred\n");
          }
        } else {
          perror("path to project malloc() error has occurred\n");
          scanf("%*s");
        }
      } else {
        perror("wrong input of path to project length\n");
        errno = 0;
        scanf("%*s");
      }
    }
  }
  struct file_and_num_of_word_occur_node *head = NULL;
  head = create_sorted_list_of_files_containing_word(path_to_project,search_word,"*.c");
  //fprintf_list_of_ ();
//delete_list_of ...()
  return 0;
}