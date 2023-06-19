#include <cstdlib>
#include <iostream>

class Node {
  int *keys;
  int order;
  int numElements;
  bool leaf;
  Node **children;

 public:
  Node(int order, bool leaf);

  void insert(int n);
};

class Btree {
  Node *root;
  int order;

 public:
  Btree(int _order) {
    root = NULL;
    order = _order;
  }

  void insert(int n);
};

int main() {
  // Insert some elements here
  return EXIT_SUCCESS;
}