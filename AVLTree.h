#ifndef __AVLTREE_H__
#define __AVLTREE_H__

template<typename Key, typename Value>
class AVLTree : public Tree<Key, Value>
{
public:
  /**
   * Creates an empty Red-Black tree
   */
  AVLTree()
    : root(NULL)
    , size(0)
  {
  }

  /**
   * Destroys the Red-Black tree
   */
  ~AVLTree()
  {
    if (root)
    {
      delete root;
    }
  }

  /**
   * Inserts a new item into the tree
   */
  void Insert(const Key& key, const Value& value)
  {
    Node *node = new Node();
    node->key = key;
    node->value = value;
    root = Insert(root, node);
  }

  /**
   * Retrieves an item from the tree
   */
  Value& Find(const Key& key)
  {
    Node *node = root;
    while (node)
    {
      if (node->key > key)
      {
        node = node->left;
      }
      else if (node->key < key)
      {
        node = node->right;
      }
      else
      {
        return node->value;
      }
    }

    throw std::runtime_error("Key not found");
  }

  /**
   * Deletes an item from the tree
   */
  void Delete(const Key& key)
  {
    root = Delete(root, key);
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
   * Internal node in the tree
   */
  class Node
  {
  public:
    /**
     * Allocates an empty node
     */
    Node()
      : weight(1)
      , left(NULL)
      , right(NULL)
    {
    }

    /**
     * Frees the node & its children
     */
    ~Node()
    {
      if (left)
      {
        delete left;
      }

      if (right)
      {
        delete right;
      }
    }

    /**
     * Computes the height of a node
     */
    void ComputeWeight()
    {
      weight = 1;

      if (left)
      {
        weight += left->weight;
      }

      if (right)
      {
        weight += right->weight;
      }
    }

    /**
     * Retrieves the balance factor
     */
    int GetBalance()
    {
      int diff = 0;

      if (left)
      {
        diff += left->weight;
      }

      if (right)
      {
        diff -= right->weight;
      }

      return diff;
    }

    /**
     * Retrieves the height of the tree
     */
    int GetHeight()
    {
      int height = 0;

      if (left)
      {
        height = std::max(height, left->GetHeight());
      }

      if (right)
      {
        height = std::max(height, right->GetHeight());
      }

      return height + 1;
    }

  public:
    /**
     * Size of the tree
     */
    size_t weight;

    /**
     * Key of the node
     */
    Key key;

    /**
     * Value stored in the node
     */
    Value value;

    /**
     * Left child
     */
    Node *left;

    /**
     * Right child
     */
    Node *right;
  };

  /**
   * Rotates a node left
   */
  Node *RotateLeft(Node *x)
  {
    Node *y = x->right;

    x->right = y->left;
    x->ComputeWeight();

    y->left = x;
    y->ComputeWeight();

    return y;
  }

  /**
   * Rotates a node right
   */
  Node *RotateRight(Node *y)
  {
    Node *x = y->left;

    y->left = x->right;
    y->ComputeWeight();

    x->right = y;
    x->ComputeWeight();

    return x;
  }

  /**
   * Returns the successor of a node (the smallest node larger
   * than this one)
   */
  Node *Successor(Node *node)
  {
    node = node->right;
    if (node)
    {
      while (node->left)
      {
        node = node->left;
      }
    }

    return node;
  }

  /**
   * Balances a node
   */
  Node *Balance(Node *node)
  {
    node->ComputeWeight();
    int balance = node->GetBalance();

    if (balance > 1)
    {
      if (node->left->GetBalance() < 0)
      {
        node->left = RotateLeft(node->left);
      }
      return RotateRight(node);
    }

    if (balance < -1)
    {
      if (node->right->GetBalance() > 0)
      {
        node->right = RotateRight(node->right);
      }
      return RotateLeft(node);
    }

    return node;
  }

  /**
   * Inserts a node & restores balance
   */
  Node *Insert(Node *node, Node *what)
  {
    if (node == NULL)
    {
      ++size;
      return what;
    }

    if (what->key < node->key)
    {
      node->left = Insert(node->left, what);
      return Balance(node);
    }

    if (what->key > node->key)
    {
      node->right = Insert(node->right, what);
      return Balance(node);
    }

    node->value = what->value;
    return node;
  }

  /**
   * Removes a node from the tree & restores balance
   */
  Node *Delete(Node *node, const Key& key)
  {
    if (!node)
    {
      throw std::runtime_error("Key not found");
    }

    if (key < node->key)
    {
      node->left = Delete(node->left, key);
      return Balance(node);
    }

    if (key > node->key)
    {
      node->right = Delete(node->right, key);
      return Balance(node);
    }

    Node *tmp;

    --size;
    if (!node->left)
    {
      tmp = node->right;
      node->right = NULL;
      delete node;
      return tmp;
    }

    if (!node->right)
    {
      tmp = node->left;
      node->left = NULL;
      delete node;
      return tmp;
    }

    tmp = Successor(node);
    tmp->right = DeleteMin(node->right);
    tmp->left = node->left;

    return Balance(tmp);
  }

  /**
   * Removes the successor of a node
   */
  Node *DeleteMin(Node *node)
  {
    if (!node->left)
    {
      return node->right;
    }

    return Balance(DeleteMin(node->left));
  }

  /**
   * Root node of the tree
   */
  Node *root;

  /**
   * Number of items stored in the tree
   */
  size_t size;
};

#endif /*__AVLTREE_H__*/
