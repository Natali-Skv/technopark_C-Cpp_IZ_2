#include <search_by_files_in_dir.h>
#include <stdio.h>
#include <string.h>

int prefix_find(const char *str, const char *pattern, size_t *pi) {
  pi[0] = 0u;
  size_t l;
  int num_of_occur = 0;

  for (l = 1u; pattern[l]; ++l) {
    size_t j =
        pi[l - 1u];
    while ((j > 0u) && (pattern[l] != pattern[j])) // не равны
      j = pi[j - 1u];
    if (pattern[l] == pattern[j])
      ++j;
    pi[l] = j;
  }
  size_t j = 0u;

  for (size_t i = 0u; str[i] != EOF; ++i) { // EOF???

    while ((j > 0u) && (str[i] != pattern[j]))
      j = pi[j - 1u];

    if (str[i] == pattern[j])
      ++j;
    if (j == l)
      ++num_of_occur;
  }
  return num_of_occur;
}
