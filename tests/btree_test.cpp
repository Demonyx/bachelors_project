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

GTEST_TEST(Btree, CheckSize) {
  Btree *tree = new Btree;
  for (int i = 1; i <= 10; i++) {
    tree->insert(i);
  }
  int result = tree->getSize(tree->root);

  ASSERT_EQ(result, 10);
}

GTEST_TEST(Btree, CheckSize0) {
  Btree *tree = new Btree;
  int result = tree->getSize(tree->root);

  ASSERT_EQ(result, 0);
}

GTEST_TEST(Btree, Insert10Remove1) {
  Btree *tree = new Btree;
  for (int i = 1; i <= 10; i++) {
    tree->insert(i);
  }

  tree->remove(5);

  ASSERT_NE(tree->search(5), 5);
}

GTEST_TEST(Btree, Insert10Remove10) {
  Btree *tree = new Btree;
  for (int i = 1; i <= 10; i++) {
    tree->insert(i);
  }

  for (int i = 1; i <= 10; i++) {
    tree->remove(i);
  }
}
