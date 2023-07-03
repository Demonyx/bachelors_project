#include "../include/btree.h"

#include <gtest/gtest.h>

GTEST_TEST(Btree, InsertionOne) {
  Btree *tree = new Btree;
  tree->insert(12);
  int result = tree->search(12);

  ASSERT_EQ(result, 12);
}

GTEST_TEST(Btree, Insert100Forwards) {
  Btree *tree = new Btree;
  for (int i = 1; i <= 100; i++) {
    tree->insert(i);
  }

  for (int i = 1; i <= 100; i++) {
    ASSERT_EQ(tree->search(i), i);
  }
}
