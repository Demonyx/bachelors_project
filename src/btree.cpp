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


Node *leaf_search(int val, Node *node) {
  if (node->IS_LEAF) {
    return node;
  }
  // TODO: Make binary search
  for (int i = 0; i <= node->numElements - 1; i++) {
    if (val < node->keys[i]) {
      return leaf_search(val, node->children[i]);
    }
  }
  return leaf_search(val, node->children[node->numElements]);
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

void Btree::remove(int val) {
  if (root == NULL) {
    // TODO: figure out error
  } else {
    Node *current = root;
    Node *parent;

    int left, right;
    while (!current->IS_LEAF) {
      for (int i = 0; i < current->numElements; i++) {
        parent = current;
        left = i - 1;
        right = i + 1;
        if (val < current->keys[i]) {
          current = current->children[i];
          break;
        }
        if (i == current->numElements - 1) {
          left = i;
          right = i + 2;
          current = current->children[i + 1];
          break;
        }
      }
    }
    bool found = false;
    int pos;
    for (pos = 0; pos < current->numElements; pos++) {
      if (current->keys[pos] == val) {
        found = true;
        break;
      }
    }

    if (!found) {
      // TODO: sensible error or something
      return;
    }

    for (int i = pos; i < current->numElements; i++) {
      current->keys[i] = current->keys[i + 1];
    }
    current->numElements--;

    if (current == root) {
      for (int i = 0; i < DEGREE - 1; i++) {
        current->children[i] = NULL;
      }

      if (current->numElements == 0) {
        // TODO: Cleanup. Should the tree be deleted?
      }
      return;
    }

    current->children[current->numElements] =
        current->children[current->numElements + 1];
    current->children[current->numElements + 1] = NULL;

    if (current->numElements >= (DEGREE + 1) / 2) {
      // We still satisfy the fullness requirement
      return;
    }
    // The fullness requirement is not satisfied, we have to shuffle nodes
    if (left >= 0) {
      Node *leftNode = parent->children[left];
      if (leftNode->numElements >= (DEGREE + 1) / 2 + 1) {
        // Make space for new key
        for (int i = current->numElements; i > 0; i--) {
          current->keys[i] = current->keys[i - 1];
        }
        current->numElements++;
        current->children[current->numElements] =
            current->children[current->numElements - 1];
        current->children[current->numElements - 1] = NULL;

        current->keys[0] = leftNode->keys[leftNode->numElements - 1];
        leftNode->numElements--;
        leftNode->children[leftNode->numElements] = current;
        leftNode->children[leftNode->numElements + 1] = NULL;
        parent->keys[left] = current->keys[0];
        return;
      }
    }

    if (right <= parent->numElements) {
      Node *rightNode = parent->children[right];
      if (rightNode->numElements >= (DEGREE + 1) / 2 + 1) {
        current->numElements++;
        current->children[current->numElements] =
            current->children[current->numElements - 1];
        current->children[current->numElements - 1] =
            NULL;  // Maybe unnecessary
        current->keys[current->numElements - 1] = rightNode->keys[0];

        rightNode->numElements--;
        rightNode->children[rightNode->numElements] =
            rightNode->children[rightNode->numElements + 1];
        rightNode->children[rightNode->numElements + 1] = NULL;
        for (int i = 0; i < rightNode->numElements; i++) {
          rightNode->keys[i] = rightNode->keys[i + 1];
        }

        parent->keys[right - 1] = rightNode->keys[0];
        return;
      }
    }

    if (left >= 0) {  // TODO: this needs better structure
                      // We want to merge two nodes
      Node *leftNode = parent->children[left];
      for (int i = leftNode->numElements, j = 0; j < current->numElements;
           i++, j++) {
        leftNode->keys[i] = current->keys[j];
      }
      leftNode->children[leftNode->numElements] = NULL;
      leftNode->numElements += current->numElements;
      leftNode->children[leftNode->numElements] =
          current->children[current->numElements];
      // TODO: remove internal
      delete[] current->keys;
      delete[] current->children;
      delete current;

    } else if (right <= parent->numElements) {
      Node *rightNode = parent->children[right];
      for (int i = current->numElements, j = 0; j < rightNode->numElements;
           i++, j++) {
        current->keys[i] = rightNode->keys[j];
      }
      current->children[current->numElements] = NULL;
      current->numElements += rightNode->numElements;
      current->children[current->numElements] =
          rightNode->children[rightNode->numElements];
      // TODO remove internaln
      delete[] rightNode->keys;
      delete[] rightNode->children;
      delete rightNode;
    }
  }
}

void Btree::removeInternal(int val, Node *current, Node *child) {
  // We need to remove internal nodes seperately since they have different
  // fullness requirements to leaf nodes
  if (current == root) {
    if (current->numElements == 1) {
      if (current->children[1] == child) {
        // change root node to left child
        delete[] child->keys;
        delete[] child->children;
        delete child;

        root = current->children[0];
        delete[] current->keys;
        delete[] current->children;
        delete current;
        return;

      } else if (current->children[0] == child) {
        // change root to right child
        delete[] child->keys;
        delete[] child->children;
        delete child;

        root = current->children[1];
        delete[] current->keys;
        delete[] current->children;
        delete current;
        return;
      }
    }
  }

  int pos;
  for (pos = 0; pos < current->numElements; pos++) {
    if (current->keys[pos] == val) {
      break;
    }
  }

  for (int i = pos; i < current->numElements; i++) {
    current->keys[i] = current->keys[i + 1];
  }

  for (pos = 0; pos < current->numElements; pos++) {
    if (current->children[pos] == child) {
      break;
    }
  }

  for (int i = pos; i < current->numElements + 1; i++) {
    current->children[i] = current->children[i + 1];
  }

  current->numElements--;

  if (current->numElements >= (DEGREE + 1) / 2 - 1) {
    // We meet the fullness criteria
    return;
  }

  if (current == root) {
    return;
  }

  Node *parent = findParent(root, current);
  int left, right;

  for (pos = 0; pos < parent->numElements + 1; pos++) {
    if (parent->children[pos] == current) {
      left = pos - 1;
      right = pos + 1;
      break;
    }
  }

  if (left >= 0) {
    Node *leftNode = parent->children[left];
    if (leftNode->numElements >= DEGREE + 1 / 2) {
      for (int i = current->numElements; i > 0; i--) {
        current->keys[i] = current->keys[i - 1];
      }

      current->keys[0] = parent->keys[left];
      parent->keys[left] = leftNode->keys[leftNode->numElements - 1];
      for (int i = current->numElements + 1; i > 0; i--) {
        current->children[i] = current->children[i - 1];
      }

      current->children[0] = leftNode->children[leftNode->numElements];
      current->numElements++;
      leftNode->numElements--;
      return;
    }
  }

  if (right <= parent->numElements) {
    Node *rightNode = parent->children[right];
    if (rightNode->numElements >= DEGREE + 1 / 2) {
      current->keys[current->numElements] = parent->keys[pos];
      parent->keys[pos] = rightNode->keys[0];

      for (int i = 0; i < rightNode->numElements; i++) {
        rightNode->keys[i] = rightNode->keys[i + 1];
      }

      current->children[current->numElements + 1] = rightNode->children[0];
      for (int i = 0; i < rightNode->numElements; ++i) {
        rightNode->children[i] = rightNode->children[i + 1];
      }

      current->numElements++;
      rightNode->numElements--;
      return;
    }
  }

  if (left >= 0) {
    Node *leftNode = parent->children[left];
    leftNode->keys[leftNode->numElements] = parent->keys[left];

    for (int i = leftNode->numElements + 1, j = 0; j < current->numElements; j++) {
      leftNode->keys[i] = current->keys[j];
    }
    for (int i = leftNode->numElements + 1, j = 0; j < current->numElements; j++) {
      leftNode->children[i] = current->children[j];
      current->children[j] = NULL;
    }

    leftNode->numElements = current->numElements + 1;
    current->numElements = 0;
    removeInternal(parent->keys[left], parent, current);
  } else if (right <= parent->numElements) {
    Node *rightNode = parent->children[right];
    current->keys[current->numElements] = parent->keys[right - 1];

    for (int i = current->numElements + 1, j = 0; j < rightNode->numElements; j++) {
      current->keys[i] = rightNode->keys[j];
    }
    for (int i = current->numElements + 1, j = 0; j < rightNode->numElements + 1; j++) {
      current->children[i] = rightNode->children[j];
      rightNode->children[j] = NULL;
    }

    current->numElements = rightNode->numElements + 1;
    rightNode->numElements = 0;
    removeInternal(parent->keys[right - 1], parent, rightNode);
  }
}

int Btree::getSize(Node *node) {
  int size = 0;
  if (node == NULL || node->numElements == 0) {
    return size;
  } else if (node->IS_LEAF) {
    return size + node->numElements;
  } else {
    for (int i = 0; i < node->numElements; i++) {
      size += getSize(node->children[i]);
    }
    return size;
  }
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
