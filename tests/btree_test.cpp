#include "../include/btree.h"

#include <gtest/gtest.h>

GTEST_TEST(Btree, InsertionOne) {
  Btree *tree = new Btree;
  tree->insert(12);
  int result = tree->search(12);

  ASSERT_EQ(result, 12);
}