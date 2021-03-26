#include <cstdlib>
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
extern "C" {
#include <search_by_files_in_dir.h>
}

TEST(prefix_search_in_file, test1) {
  const char *file_name = "../tests/input_emulator/f1.c";
  EXPECT_EQ(prefix_search_in_file(file_name, "1"), 40);
}

TEST(prefix_search_in_file, test2) {
  const char *file_name = "../tests/input_emulator/f2.c";
  EXPECT_EQ(prefix_search_in_file(file_name, "1"), 125);
}

TEST(prefix_search_in_file, test3) {
  const char *file_name = "../tests/input_emulator/f3.c";
  EXPECT_EQ(prefix_search_in_file(file_name, "int"), 22);
  EXPECT_EQ(prefix_search_in_file(file_name, "Int"), 22);
  EXPECT_EQ(prefix_search_in_file(file_name, "INT"), 22);
  EXPECT_EQ(prefix_search_in_file(file_name, "InT"), 22);
  EXPECT_EQ(prefix_search_in_file(file_name, "inT"), 22);
}

TEST(prefix_search_in_file, test4) {
  const char *file_name = "../tests/input_emulator/f4.c";

  EXPECT_EQ(prefix_search_in_file(file_name, "#string"), 21);
  EXPECT_EQ(prefix_search_in_file(file_name, "#StRiNg"), 21);
  EXPECT_EQ(prefix_search_in_file(file_name, "#STRING"), 21);
  EXPECT_EQ(prefix_search_in_file(file_name, "#strINg"), 21);
  EXPECT_EQ(prefix_search_in_file(file_name, "#STRIng"), 21);
}

TEST(prefix_search_in_file, test5) {
  const char *file_name = "../tests/input_emulator/f5.c";
  EXPECT_EQ(prefix_search_in_file(file_name, "qwerty"), 0);
  EXPECT_EQ(prefix_search_in_file(file_name, "by ns"), 0);
}

TEST(prefix_search_in_file, test6) {
  const char *file_name = "../tests/input_emulator/f6.c";
  EXPECT_EQ(prefix_search_in_file(file_name, "1"), -1);
}

TEST(merge_two_sorted_lists, test1) {
  file_and_num_of_word_occur *head_1 = new file_and_num_of_word_occur;
  file_and_num_of_word_occur *iter_1 = head_1;
  for (int i = 10; i > 0; i -= 2) {
    iter_1->num_of_word_occur = i;
    iter_1->next = new file_and_num_of_word_occur;
    iter_1 = iter_1->next;
  }
  iter_1->num_of_word_occur = -2;
  iter_1->next = NULL;

  file_and_num_of_word_occur *head_2 = new file_and_num_of_word_occur;
  file_and_num_of_word_occur *iter_2 = head_2;
  for (int i = 21; i >= 0; i -= 3) {
    iter_2->num_of_word_occur = i;
    iter_2->next = new file_and_num_of_word_occur;
    iter_2 = iter_2->next;
  }
  iter_2->num_of_word_occur = 0;
  iter_2->next = NULL;
  file_and_num_of_word_occur *head_result =
      merge_two_sorted_lists(head_1, head_2);

  int i3 = 7;
  int i2 = 5;
  file_and_num_of_word_occur *iter_result = head_result;
  while (iter_result) {
    EXPECT_EQ((i3 * 3 > i2 * 2) ? (i3--) * 3 : (i2--) * 2,
              iter_result->num_of_word_occur);
    iter_result = iter_result->next;
  }

  while (head_result) {
    iter_result = head_result;
    head_result = head_result->next;
    delete (iter_result);
  }
}

TEST(merge_two_sorted_lists, test2) {
  file_and_num_of_word_occur *head = new file_and_num_of_word_occur;
  file_and_num_of_word_occur *iter = head;
  for (int i = 50; i > 0; --i) {
    iter->num_of_word_occur = i;
    iter->next = new file_and_num_of_word_occur;
    iter = iter->next;
  }
  iter->num_of_word_occur = 0;
  iter->next = NULL;

  file_and_num_of_word_occur *head_result = merge_two_sorted_lists(head, head);

  int i = 50;
  file_and_num_of_word_occur *iter_result = head_result;
  while (iter_result) {
    EXPECT_EQ(i--, iter_result->num_of_word_occur);
    iter_result = iter_result->next;
  }

  while (head_result) {
    iter_result = head_result;
    head_result = head_result->next;
    delete (iter_result);
  }
}

TEST(merge_two_sorted_lists, test3) {
  file_and_num_of_word_occur *head_1 = new file_and_num_of_word_occur;
  file_and_num_of_word_occur *iter_1 = head_1;
  for (int i = 0; i < 20; ++i) {
    iter_1->num_of_word_occur = -10;
    iter_1->next = new file_and_num_of_word_occur;
    iter_1 = iter_1->next;
  }
  iter_1->num_of_word_occur = -10;
  iter_1->next = NULL;

  file_and_num_of_word_occur *head_2 = new file_and_num_of_word_occur;
  file_and_num_of_word_occur *iter_2 = head_2;
  for (int i = 0; i < 10; ++i) {
    iter_2->num_of_word_occur = 10;
    iter_2->next = new file_and_num_of_word_occur;
    iter_2 = iter_2->next;
  }
  iter_2->num_of_word_occur = 10;
  iter_2->next = NULL;
  file_and_num_of_word_occur *head_result =
      merge_two_sorted_lists(head_1, head_2);

  file_and_num_of_word_occur *iter_result = head_result;
  for (int i = 0; i <= 10; ++i) {
    EXPECT_EQ(10, iter_result->num_of_word_occur);
    iter_result = iter_result->next;
  }
  for (int i = 0; i <= 20; ++i) {
    EXPECT_EQ(-10, iter_result->num_of_word_occur);
    iter_result = iter_result->next;
  }
  EXPECT_TRUE(iter_result == NULL);
  while (head_result) {
    iter_result = head_result;
    head_result = head_result->next;
    delete (iter_result);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
