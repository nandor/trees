#ifndef __BTREE_H__
#define __BTREE_H__

/**
 * BTree implementation. Each node contains between T - 1 and 2 * T - 1 keys
 * and all internal nodes contain n + 1 children, where n is the number of
 * keys stored in a node
 *
 * @tparam Key   Key types, must support total ordering
 * @tparam Value Value types
 * @tparam T     Minimal degree of the tree
 */
template <typename Key, typename Value, int T>
class BTree : public Tree<Key, Value>
{
public:
  /**
   * Creates a new BTree
   */
  BTree()
    : root(NULL)
    , size(0)
  {
    root = new Node();
    root->n = 0;
    root->leaf = true;
  }

  /**
   * Destroys the tree
   */
  ~BTree()
  {
    if (root)
    {
      delete root;
    }
  }

  /**
   * Inserts a value into the tree
   */
  void Insert(const Key& key, const Value &value)
  {
    if (root->n < 2 * T - 1)
    {
      InsertNonFull(root, Item(key, value));
    }
    else
    {
      Node *node = new Node();
      node->n = 0;
      node->leaf = false;
      node->child[0] = root;
      root = node;
      Split(node, 0);
      InsertNonFull(node, Item(key, value));
    }
  }

  /**
   * Deletes an entry from the tree
   */
  void Delete(const Key& key)
  {
    Delete(root, key);
  }

  /**
   * Finds a value in the tree
   */
  Value& Find(const Key& key)
  {
    Node *node = root;
    while (node)
    {
      int i = 0;
      while (i < node->n && node->key[i].key < key)
      {
        ++i;
      }

      if (i < node->n && node->key[i].key == key)
      {
        return node->key[i].value;
      }

      if (node->leaf)
      {
        break;
      }

      node = node->child[i];
    }

    throw std::runtime_error("Key not found");
  }

  /**
   * Returns the number of items in the tree
   */
  size_t GetSize()
  {
    return size;
  }

  /**
   * Returns the height of the tree
   */
  size_t GetHeight()
  {
    return root ? root->GetHeight() : 0;
  }

private:
  /**
   * Key-Value pair
   */
  struct Item
  {
    Item()
    {
    }

    Item(const Key& key, const Value& value)
      : key(key)
      , value(value)
    {
    }

    Key    key;
    Value  value;
  };

  /**
   * Internal BTree node
   */
  struct Node
  {
    Node()
    {
      memset(child, 0, sizeof(child));
    }

    ~Node()
    {
      if (n > 0)
      {
        for (int i = 0; i <= n; ++i)
        {
          delete child[i];
        }
      }
    }

    int GetHeight()
    {
      int height = 0;
      for (int i = 0; i <= n; ++i)
      {
        height = std::max(height, child[i]->GetHeight());
      }

      return height + 1;
    }

    int  n;
    bool    leaf;
    Item    key[T * 2];
    Node   *child[T * 2 + 1];
  };

  /**
   * Takes as input a node containing 2 * t - 1 keys and its parent,
   * moves the median key from the node up to the parent and creates a new
   * node out of the keys that are above the median
   * @param x  Parent node
   * @param c  Index of median key in parent
   */
  void Split(Node *x, int c)
  {
    Node *z, *y;

    z = new Node();
    y = x->child[c];
    z->leaf = y->leaf;

    z->n = T - 1;
    y->n = T - 1;

    for (int i = 0; i < T - 1; ++i)
    {
      z->key[i] = y->key[i + T];
    }

    if (!z->leaf)
    {
      for (int i = 0; i < T; ++i)
      {
        z->child[i] = y->child[i + T];
      }
    }

    for (int i = x->n; i > c; --i)
    {
      x->child[i] = x->child[i - 1];
    }

    x->child[c + 1] = z;
    for (int i = x->n - 1; i > c; --i)
    {
      x->key[i + 1] = x->key[i];
    }

    x->key[c] = y->key[T - 1];
    ++x->n;
  }

  /**
   * Joins two nodes with a median key
   * @return Left node
   */
  Node* Join(Node *node, int j)
  {
    Node *left = node->child[j];
    Node *right = node->child[j + 1];

    // Add key to left child
    left->n = 2 * T - 1;
    left->key[T - 1] = node->key[j];

    // Add keys from right
    for (int i = 0; i < T; ++i)
    {
      left->key[T + i] = right->key[i];
    }

    // Add children
    if (!left->leaf)
    {
      for (int i = 0; i <= T; ++i)
      {
        left->child[T + i] = right->child[i];
      }
    }

    // Remove key
    for (int i = j; i < node->n; ++i)
    {
      node->key[i] = node->key[i + 1];
      node->child[i + 1] = node->child[i + 2];
    }
    node->n--;

    // If root became empty, reduce the height of the tree
    if (node->n == 0)
    {
      root->n = 0;
      delete root;
      root = left;
    }

    right->n = 0;
    delete right;

    return left;
  }

  /**
   * Inserts a key into a node that's node full
   * If a duplicate key is found, its value is overwritten
   */
  void InsertNonFull(Node *node, const Item& item)
  {
    if (node->leaf)
    {
      int i, j = node->n - 1;
      for (i = 0; item.key >= node->key[i].key && i < node->n; ++i)
      {
        if (item.key == node->key[i].key)
        {
          node->key[i].value = item.value;
          return;
        }
      }

      while (j >= i)
      {
        node->key[j + 1] = node->key[j];
        --j;
      }

      node->key[j + 1] = item;
      ++node->n;
      ++size;
    }
    else
    {
      int i = node->n - 1;
      while (i >= 0 && item.key < node->key[i].key)
      {
        --i;
      }

      if (i >= 0 && node->key[i].key == item.key)
      {
        node->key[i].value = item.value;
        return;
      }

      ++i;

      if (node->child[i]->n == 2 * T - 1)
      {
        Split(node, i);
        if (item.key > node->key[i].key)
        {
          ++i;
        }
      }

      InsertNonFull(node->child[i], item);
    }
  }

  /**
   * Deletes a node from the tree, maintaining balance
   */
  void Delete(Node *node, const Key& key)
  {
    if (node->leaf)
    {
      // Rule 1: If key is in a leaf, it is removed
      DeleteFromLeaf(node, key);
      return;
    }

    int i = node->n - 1;
    while (i >= 0 && key < node->key[i].key)
    {
      --i;
    }

    if (i >= 0 && node->key[i].key == key)
    {
      // Rule 2: Key is in an internal node
      if (node->child[i]->n >= T)
      {
        // Rule 2a
        node->key[i] = DeleteMax(node->child[i]);
        --size;
      }
      else if (node->child[i + 1]->n >= T)
      {
        // Rule 2b
        node->key[i] = DeleteMin(node->child[i + 1]);
        --size;
      }
      else
      {
        // Rule 2c
        Delete(Join(node, i), key);
      }
    }
    else
    {
      // Key is in one of the children
      ++i;
      if (node->child[i]->n >= T)
      {
        // Rule 3
        Delete(node->child[i], key);
      }
      else
      {
        if (i >= 1 && node->child[i - 1]->n >= T)
        {
          // Rule 3a
          BorrowLeft(node, i);
          Delete(node->child[i], key);
        }
        else if (i < node->n && node->child[i + 1]->n >= T)
        {
          // Rule 3a
          BorrowRight(node, i);
          Delete(node->child[i], key);
        }
        else
        {
          // Rule 3b
          Delete(Join(node, i >= 1 ? i - 1 : i), key);
        }
      }
    }
  }

  /**
   * Finds the largest key in a subtree & deletes it
   * Works in a similar fashion as normal deletion, except
   * it does not have to check for internal nodes
   * as the maximum is always in a leaf
   */
  Item DeleteMax(Node *node)
  {
    if (node->leaf)
    {
      // Rule 2: Max is the last key
      return node->key[--node->n];
    }

    if (node->child[node->n]->n >= T)
    {
      // Rule 3
      return DeleteMax(node->child[node->n]);
    }

    if (node->child[node->n - 1]->n >= T)
    {
      // Rule 3a
      BorrowLeft(node, node->n);
      return DeleteMax(node->child[node->n]);
    }

    return DeleteMax(Join(node, node->n - 1));
  }

  /**
   * Same as before, but removes the minimum
   */
  Item DeleteMin(Node *node)
  {
    if (node->leaf)
    {
      // Rule 2
      Item key = node->key[0];
      for (int i = 0; i < node->n - 1; ++i)
      {
        node->key[i] = node->key[i + 1];
      }

      --node->n;
      return key;
    }

    if (node->child[0]->n >= T)
    {
      // Rule 3
      return DeleteMin(node->child[0]);
    }

    if (node->child[1]->n >= T)
    {
      BorrowRight(node, 0);
      return DeleteMin(node->child[0]);
    }

    return DeleteMin(Join(node, 0));
  }

  /**
   * Deletes a key from a leaf node or throws
   * and exception if the key is not in the leaf
   */
  void DeleteFromLeaf(Node *node, const Key& key)
  {
    for (int i = 0; i < node->n; ++i)
    {
      if (node->key[i].key == key)
      {
        for (int j = i + 1; j < node->n; ++j)
        {
          node->key[j - 1] = node->key[j];
        }

        --size;
        --node->n;
        return;
      }
    }

    throw std::runtime_error("Key not found!");
  }

  /**
   * Borrows a key from the left sibling
   */
  void BorrowLeft(Node *node, int i)
  {
    Node *child = node->child[i];
    Node *sibling = node->child[i - 1];

    // Move keys of the child to the right
    for (int i = T; i >= 1; --i)
    {
      child->key[i] = child->key[i - 1];
    }

    // Add a key from x
    child->key[0] = node->key[i - 1];
    child->n = T;

    // Move key from sibling to node
    node->key[i - 1] = sibling->key[sibling->n - 1];

    // Move & add children
    if (!child->leaf)
    {
      for (int i = T + 1; i >= 1; --i);
      {
        child->child[i] = child->child[i - 1];
      }
      child->child[0] = sibling->child[sibling->n];
    }

    --sibling->n;
  }

  /**
   * Borrows a key from the right sibling
   */
  void BorrowRight(Node *node, int i)
  {
    Node *child = node->child[i];
    Node *sibling = node->child[i + 1];

    // Move a key from node to child
    child->key[T - 1] = node->key[i];
    child->n = T;

    // Move a key from sibling to node
    node->key[i] = sibling->key[0];

    // Remove key from sibling
    for (int i = 1; i < sibling->n; ++i)
    {
      sibling->key[i - 1] = sibling->key[i];
    }

    if (!child->leaf)
    {
      child->child[T] = sibling->child[0];
      for (int i = 1; i <= sibling->n; ++i)
      {
        sibling->child[i - 1] = sibling->child[i];
      }
    }

    --sibling->n;
  }

private:
  /**
   * Root node
   */
  Node  *root;

  /**
   * Number of items
   */
  size_t size;
};

#endif /*__BTREE_H__*/
