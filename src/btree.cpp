#include "../include/btree.h"

void Btree::insert(int n) {
  if (this->root == NULL) {
    this->root = new Node;
    this->root->keys[0] = n;
    this->root->IS_LEAF = true;
    this->root->size = 1;
    // write to disk
    
  } else {
    Node *current = root;
    Node *parent;
    while (!current->IS_LEAF) {
      parent = current;
      for (int i = 0; i < current->size; i++) {
        if (n < current->keys[i]) {
          current = current->children[i];
          break;
        }
        if (i == current->size - 1) {
          current = current->children[i + 1];
          break;
        }
      }
    }

    if (current->size < DEGREE) {
      int i = 0;
      while (n > current->keys[i] && i < current->size) {
        i++;
      }

      for (int j = current->size; j > i; j--) {
        current->keys[j] = current->keys[j - 1];
      }

      current->keys[i] = n;
      current->size++;
      current->children[current->size] =
          current->children[current->size - 1];
      current->children[current->size - 1] = NULL;
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

      current->size = (DEGREE + 1) / 2;
      newLeaf->size = DEGREE + 1 - (DEGREE + 1) / 2;
      current->children[current->size] = newLeaf;
      newLeaf->children[newLeaf->size] = current->children[DEGREE];
      current->children[DEGREE] = NULL;

      for (i = 0; i < current->size; i++) {
        current->keys[i] = tempNode[i];
      }

      int j;
      for (i = 0, j = current->size; i < newLeaf->size;
           i++, j++) {
        newLeaf->keys[i] = tempNode[j];
      }

      if (current == root) {
        Node *newRoot = new Node;
        newRoot->keys[0] = newLeaf->keys[0];
        newRoot->children[0] = current;
        newRoot->children[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->keys[0], parent, newLeaf);
      }
    }
  }
}

void Btree::insertInternal(int n, Node *current, Node *child) {
  if (current->size < DEGREE) {
    int i = 0;
    while (n > current->keys[i] && i < current->size) {
      i++;
    }

    for (int j = current->size; j > i; j--) {
      current->keys[j] = current->keys[j - 1];
    }

    for (int j = current->size + 1; j > i; j--) {
      current->children[j] = current->children[j - 1];
    }

    current->keys[i] = n;
    current->size++;
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
    current->size = (DEGREE + 1) / 2;
    newInternal->size = DEGREE - (DEGREE + 1) / 2;

    int j;
    for (i = 0, j = current->size + 1; i < newInternal->size;
         i++, j++) {
      newInternal->keys[i] = tempKeys[j];
    }
    for (i = 0, j = current->size + 1; i < newInternal->size + 1;
         i++, j++) {
      newInternal->children[i] = tempChildren[j];
    }
    if (current == root) {
      Node *newRoot = new Node;
      newRoot->keys[0] = current->keys[current->size];
      newRoot->children[0] = current;
      newRoot->children[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(current->keys[current->size],
                     findParent(root, current), newInternal);
    }
  }
}

Node *Btree::findParent(Node *current, Node *child) {
  Node *parent;
  if (current->IS_LEAF || (current->children[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < current->size + 1; i++) {
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


Node *leafSearch(int val, Node *node) {
  if (node->IS_LEAF) {
    return node;
  }
  // TODO: Make binary search
  for (int i = 0; i <= node->size - 1; i++) {
    if (val < node->keys[i]) {
      return leafSearch(val, node->children[i]);
    }
  }
  return leafSearch(val, node->children[node->size]);
}

int Btree::search(int val) {
  Node *leaf = leafSearch(val, root);
  // More efficient search here?
  for (int i = 0; i <= leaf->size - 1; i++) {
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
      for (int i = 0; i < current->size; i++) {
        parent = current;
        left = i - 1;
        right = i + 1;
        if (val < current->keys[i]) {
          current = current->children[i];
          break;
        }
        if (i == current->size - 1) {
          left = i;
          right = i + 2;
          current = current->children[i + 1];
          break;
        }
      }
    }
    bool found = false;
    int pos;
    for (pos = 0; pos < current->size; pos++) {
      if (current->keys[pos] == val) {
        found = true;
        break;
      }
    }

    if (!found) {
      // TODO: sensible error or something
      return;
    }

    for (int i = pos; i < current->size; i++) {
      current->keys[i] = current->keys[i + 1];
    }
    current->size--;

    if (current == root) {
      for (int i = 0; i < DEGREE - 1; i++) {
        current->children[i] = NULL;
      }

      if (current->size == 0) {
        // TODO: Cleanup. Should the tree be deleted?
      }
      return;
    }

    current->children[current->size] =
        current->children[current->size + 1];
    current->children[current->size + 1] = NULL;

    if (current->size >= (DEGREE + 1) / 2) {
      // We still satisfy the fullness requirement
      return;
    }
    // The fullness requirement is not satisfied, we have to shuffle nodes
    if (left >= 0) {
      Node *leftNode = parent->children[left];
      if (leftNode->size >= (DEGREE + 1) / 2 + 1) {
        // Make space for new key
        for (int i = current->size; i > 0; i--) {
          current->keys[i] = current->keys[i - 1];
        }
        current->size++;
        current->children[current->size] =
            current->children[current->size - 1];
        current->children[current->size - 1] = NULL;

        current->keys[0] = leftNode->keys[leftNode->size - 1];
        leftNode->size--;
        leftNode->children[leftNode->size] = current;
        leftNode->children[leftNode->size + 1] = NULL;
        parent->keys[left] = current->keys[0];
        return;
      }
    }

    if (right <= parent->size) {
      Node *rightNode = parent->children[right];
      if (rightNode->size >= (DEGREE + 1) / 2 + 1) {
        current->size++;
        current->children[current->size] =
            current->children[current->size - 1];
        current->children[current->size - 1] =
            NULL;  // Maybe unnecessary
        current->keys[current->size - 1] = rightNode->keys[0];

        rightNode->size--;
        rightNode->children[rightNode->size] =
            rightNode->children[rightNode->size + 1];
        rightNode->children[rightNode->size + 1] = NULL;
        for (int i = 0; i < rightNode->size; i++) {
          rightNode->keys[i] = rightNode->keys[i + 1];
        }

        parent->keys[right - 1] = rightNode->keys[0];
        return;
      }
    }

    if (left >= 0) {  // TODO: this needs better structure
                      // We want to merge two nodes
      Node *leftNode = parent->children[left];
      for (int i = leftNode->size, j = 0; j < current->size;
           i++, j++) {
        leftNode->keys[i] = current->keys[j];
      }
      leftNode->children[leftNode->size] = NULL;
      leftNode->size += current->size;
      leftNode->children[leftNode->size] =
          current->children[current->size];
      // TODO: remove internal
      delete[] current->keys;
      delete[] current->children;
      delete current;

    } else if (right <= parent->size) {
      Node *rightNode = parent->children[right];
      for (int i = current->size, j = 0; j < rightNode->size;
           i++, j++) {
        current->keys[i] = rightNode->keys[j];
      }
      current->children[current->size] = NULL;
      current->size += rightNode->size;
      current->children[current->size] =
          rightNode->children[rightNode->size];
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
    if (current->size == 1) {
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
  for (pos = 0; pos < current->size; pos++) {
    if (current->keys[pos] == val) {
      break;
    }
  }

  for (int i = pos; i < current->size; i++) {
    current->keys[i] = current->keys[i + 1];
  }

  for (pos = 0; pos < current->size; pos++) {
    if (current->children[pos] == child) {
      break;
    }
  }

  for (int i = pos; i < current->size + 1; i++) {
    current->children[i] = current->children[i + 1];
  }

  current->size--;

  if (current->size >= (DEGREE + 1) / 2 - 1) {
    // We meet the fullness criteria
    return;
  }

  if (current == root) {
    return;
  }

  Node *parent = findParent(root, current);
  int left, right;

  for (pos = 0; pos < parent->size + 1; pos++) {
    if (parent->children[pos] == current) {
      left = pos - 1;
      right = pos + 1;
      break;
    }
  }

  if (left >= 0) {
    Node *leftNode = parent->children[left];
    if (leftNode->size >= DEGREE + 1 / 2) {
      for (int i = current->size; i > 0; i--) {
        current->keys[i] = current->keys[i - 1];
      }

      current->keys[0] = parent->keys[left];
      parent->keys[left] = leftNode->keys[leftNode->size - 1];
      for (int i = current->size + 1; i > 0; i--) {
        current->children[i] = current->children[i - 1];
      }

      current->children[0] = leftNode->children[leftNode->size];
      current->size++;
      leftNode->size--;
      return;
    }
  }

  if (right <= parent->size) {
    Node *rightNode = parent->children[right];
    if (rightNode->size >= DEGREE + 1 / 2) {
      current->keys[current->size] = parent->keys[pos];
      parent->keys[pos] = rightNode->keys[0];

      for (int i = 0; i < rightNode->size; i++) {
        rightNode->keys[i] = rightNode->keys[i + 1];
      }

      current->children[current->size + 1] = rightNode->children[0];
      for (int i = 0; i < rightNode->size; ++i) {
        rightNode->children[i] = rightNode->children[i + 1];
      }

      current->size++;
      rightNode->size--;
      return;
    }
  }

  if (left >= 0) {
    Node *leftNode = parent->children[left];
    leftNode->keys[leftNode->size] = parent->keys[left];

    for (int i = leftNode->size + 1, j = 0; j < current->size; j++) {
      leftNode->keys[i] = current->keys[j];
    }
    for (int i = leftNode->size + 1, j = 0; j < current->size; j++) {
      leftNode->children[i] = current->children[j];
      current->children[j] = NULL;
    }

    leftNode->size = current->size + 1;
    current->size = 0;
    removeInternal(parent->keys[left], parent, current);
  } else if (right <= parent->size) {
    Node *rightNode = parent->children[right];
    current->keys[current->size] = parent->keys[right - 1];

    for (int i = current->size + 1, j = 0; j < rightNode->size; j++) {
      current->keys[i] = rightNode->keys[j];
    }
    for (int i = current->size + 1, j = 0; j < rightNode->size + 1; j++) {
      current->children[i] = rightNode->children[j];
      rightNode->children[j] = NULL;
    }

    current->size = rightNode->size + 1;
    rightNode->size = 0;
    removeInternal(parent->keys[right - 1], parent, rightNode);
  }
}

int Btree::getSize(Node *node) {
  int size = 0;
  if (node == NULL || node->size == 0) {
    return size;
  } else if (node->IS_LEAF) {
    return size + node->size;
  } else {
    for (int i = 0; i < node->size; i++) {
      size += getSize(node->children[i]);
    }
    return size;
  }
}


void Btree::printTree(Node *node) {
  if (node != NULL) {
    for (int i = 0; i < node->size; i++) {
      cout << node->keys[i] << " ";
    }
    cout << "\n";
    if (node->IS_LEAF != true) {
      for (int i = 0; i < node->size + 1; i++) {
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
