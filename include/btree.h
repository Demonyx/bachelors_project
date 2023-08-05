#ifndef BTREE_H

#define BTREE_H

#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <optional>
using namespace std;

const int DEGREE = 64;

class Node {
 public:
  int *keys;
  Node **children;
  bool IS_LEAF;
  int numElements;

  Node();

  friend class BTree;
};

class Btree {
 public:
  Node *root;

  Btree();
  void insertInternal(int, Node *, Node *);

  void insert(int n);
  Node *findParent(Node *, Node *);
  int search(int value);
  void remove(int value);
  void removeInternal(int value, Node *current, Node *child);

  void printTree(Node *node);
};

inline Node::Node() {
  keys = new int[DEGREE];
  children = new Node *[DEGREE + 1];
}

inline Btree::Btree() { root = NULL; }
#endif