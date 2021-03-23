#include <dirent.h>
#include <errno.h>
#include <regex.h>
#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int regexec_with_perror(regex_t *regex, const char *str);

struct file_and_num_of_word_occur *
create_sorted_list_of_files_containing_word(char *dir_name, char *search_word,
                                            regex_t *file_name_regex,
                                            size_t max_path) {
  DIR *dir;
  struct dirent *entry;
  size_t len = strlen(dir_name);

  if (!(dir = opendir(dir_name))) {
    fprintf(stderr, "path not found: %s: %s\n", dir_name, strerror(errno));
    return NULL;
  }

  while ((entry = readdir(dir)) != NULL) {
    char *name = entry->d_name;
    if (entry->d_type == DT_DIR) {
      if (strcmp(name, ".") && strcmp(name, "..")) {
        if (len + strlen(name) + 2 > max_path) {
          fprintf(stderr, "path too long: %s/%s\n", dir_name, name);
        } else {
          dir_name[len] = '/';
          strcpy(dir_name + len + 1, name);
          create_sorted_list_of_files_containing_word(
              dir_name, search_word, file_name_regex, max_path);
          dir_name[len] = '\0';
        }
      }
    } else if (entry->d_type == DT_REG) {
      if (!regexec_with_perror(file_name_regex, name)) {
        printf("%s/%s\n", dir_name, name);

        //new
      }
    }
  }
  closedir(dir);
  return NULL;
}

int regexec_with_perror(regex_t *regex, const char *str) {
  int result = regexec(regex, str, 0, NULL, 0);
  if (!result || result == REG_NOMATCH) {
    return result;
  } else {
    char msgbuf[100];
    regerror(result, regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match for %s failed: %s\n", str, msgbuf);
    return -1;
  }
}
/*
  int main(void) {
    char path[1024] = ".";
    listdir(path, sizeof path);
    return 0;
  }


*/