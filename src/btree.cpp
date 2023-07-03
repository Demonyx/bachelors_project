#include "../include/btree.h"

void Btree::insert(int n) {
  if (this->root == NULL) {
    this->root = new Node;
    this->root->keys[0] = n;
    this->root->IS_LEAF = true;
    this->root->numElements = 1;
  } else {
    Node *current = root;
    Node *parent;
    while (!current->IS_LEAF) {
      parent = current;
      for (int i = 0; i < current->numElements; i++) {
        if (n < current->keys[i]) {
          current = current->children[i];
          break;
        }
        if (i == current->numElements - 1) {
          current = current->children[i + 1];
          break;
        }
      }
    }

    if (current->numElements < DEGREE) {
      int i = 0;
      while (n > current->keys[i] && i < current->numElements) {
        i++;
      }

      for (int j = current->numElements; j > i; j--) {
        current->keys[j] = current->keys[j - 1];
      }

      current->keys[i] = n;
      current->numElements++;
      current->children[current->numElements] =
          current->children[current->numElements - 1];
      current->children[current->numElements - 1] = NULL;
    } else {
      Node *newLeaf = new Node;
      int tempNode[DEGREE + 1];

      for (int i = 0; i < DEGREE; i++) {
        tempNode[i] = current->keys[i];
      }

      int i = 0;
      while (n > tempNode[i] && i < DEGREE) {
        i++;
      }

      for (int j = DEGREE + 1; j > i; j--) {
        tempNode[j] = tempNode[j - 1];
      }

      tempNode[i] = n;
      newLeaf->IS_LEAF = true;

      current->numElements = (DEGREE + 1) / 2;
      newLeaf->numElements = DEGREE + 1 - (DEGREE + 1) / 2;
      current->children[current->numElements] = newLeaf;
      newLeaf->children[newLeaf->numElements] = current->children[DEGREE];
      current->children[DEGREE] = NULL;

      for (i = 0; i < current->numElements; i++) {
        current->keys[i] = tempNode[i];
      }

      int j;
      for (i = 0, j = current->numElements; i < newLeaf->numElements;
           i++, j++) {
        newLeaf->keys[i] = tempNode[j];
      }

      if (current == root) {
        Node *newRoot = new Node;
        newRoot->keys[0] = newLeaf->keys[0];
        newRoot->children[0] = current;
        newRoot->children[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->numElements = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->keys[0], parent, newLeaf);
      }
    }
  }
}

void Btree::insertInternal(int n, Node *current, Node *child) {
  if (current->numElements < DEGREE) {
    int i = 0;
    while (n > current->keys[i] && i < current->numElements) {
      i++;
    }

    for (int j = current->numElements; j > i; j--) {
      current->keys[j] = current->keys[j - 1];
    }

    for (int j = current->numElements + 1; j > i; j--) {
      current->children[j] = current->children[j - 1];
    }

    current->keys[i] = n;
    current->numElements++;
    current->children[i + 1] = child;
  } else {
    Node *newInternal = new Node;
    int tempKeys[DEGREE + 1];
    Node *tempChildren[DEGREE + 2];

    for (int i = 0; i < DEGREE; i++) {
      tempKeys[i] = current->keys[i];
    }

    for (int i = 0; i < DEGREE + 1; i++) {
      tempChildren[i] = current->children[i];
    }

    int i = 0;
    while (n > tempKeys[i] && i < DEGREE) {
      i++;
    }

    for (int j = DEGREE + 1; j > i; j--) {
      tempKeys[j] = tempKeys[j - 1];
    }
    tempKeys[i] = n;

    for (int j = DEGREE + 2; j > i + 1; j--) {
      tempChildren[j] = tempChildren[j - 1];
    }

    tempChildren[i + 1] = child;
    newInternal->IS_LEAF = false;
    current->numElements = (DEGREE + 1) / 2;
    newInternal->numElements = DEGREE - (DEGREE + 1) / 2;

    int j;
    for (i = 0, j = current->numElements + 1; i < newInternal->numElements;
         i++, j++) {
      newInternal->keys[i] = tempKeys[j];
    }
    for (i = 0, j = current->numElements + 1; i < newInternal->numElements + 1;
         i++, j++) {
      newInternal->children[i] = tempChildren[j];
    }
    if (current == root) {
      Node *newRoot = new Node;
      newRoot->keys[0] = current->keys[current->numElements];
      newRoot->children[0] = current;
      newRoot->children[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->numElements = 1;
      root = newRoot;
    } else {
      insertInternal(current->keys[current->numElements],
                     findParent(root, current), newInternal);
    }
  }
}

Node *Btree::findParent(Node *current, Node *child) {
  Node *parent;
  if (current->IS_LEAF || (current->children[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < current->numElements + 1; i++) {
    if (current->children[i] == child) {
      parent = current;
      return parent;
    } else {
      parent = findParent(current->children[i], child);
      if (parent != NULL) return parent;
    }
  }
  return parent;
}

void Btree::printTree(Node *node) {
  if (node != NULL) {
    for (int i = 0; i < node->numElements; i++) {
      cout << node->keys[i] << " ";
    }
    cout << "\n";
    if (node->IS_LEAF != true) {
      for (int i = 0; i < node->numElements + 1; i++) {
        printTree(node->children[i]);
      }
    }
  }
}

Node *leaf_search(int val, Node *node) {
  if (node->IS_LEAF) {
    return node;
  }
  // TODO: Make binary search
  for (int i = 0; i < node->numElements - 2; i++) {
    if (val <= node->keys[i]) {
      return leaf_search(val, node->children[i]);
    }
  }
  return leaf_search(val, node->children[node->numElements - 1]);
}

int Btree::search(int val) {
  Node *leaf = leaf_search(val, root);
  // More efficient search here?
  for (int i = 0; i <= leaf->numElements - 1; i++) {
    if (val == leaf->keys[i]) {
      return leaf->keys[i];
    }
  }
  return -1;  // TODO: make proper error or something else. Options?
}

// int tree_main() {
//   srand(time(NULL));

//   Btree *tree = new Btree;

//   for (int i = 0; i < 100000; i++) {
//     int x = rand();
//     // cout << x << "\n";
//     tree->insert(x);
//   }
//   tree->insert(12);

//   cout << "Got: " << tree->search(12);
//   return EXIT_SUCCESS;
// }
