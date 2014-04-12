#ifndef __TREAP_H__
#define __TREAP_H__

template<typename Key, typename Value>
class Treap : public Tree<Key, Value>
{
public:
  /**
   * Creates an empty Red-Black tree
   */
  Treap()
    : root(NULL)
    , size(0)
  {
  }

  /**
   * Destroys the Red-Black tree
   */
  ~Treap()
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
    Node *node = root, *parent = NULL;
    while (node)
    {
      if (key < node->key)
      {
        parent = node;
        node = node->left;
      }
      else if (key > node->key)
      {
        parent = node;
        node = node->right;
      }
      else
      {
        if (parent == NULL)
        {
          root = Delete(node);
        }
        else if (parent->left == node)
        {
          parent->left = Delete(node);
        }
        else
        {
          parent->right = Delete(node);
        }
        return;
      }
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
   * Internal node in the tree
   */
  class Node
  {
  public:
    /**
     * Allocates an empty node
     */
    Node()
      : weight(rand())
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
    y->left = x;
    return y;
  }

  /**
   * Rotates a node right
   */
  Node *RotateRight(Node *y)
  {
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
  }

  /**
   * Balances a node
   */
  Node *Balance(Node *node)
  {
    if (node->left && node->left->weight < node->weight)
    {
      return RotateRight(node);
    }

    if (node->right && node->right->weight < node->weight)
    {
      return RotateLeft(node);
    }

    return node;
  }

  /**
   * Inserts a new node into the tree, preserving balance
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
   * Removes a node from the treap, preserving balance
   */
  Node *Delete(Node *node)
  {
    if (!node->left && !node->right)
    {
      delete node;
      --size;
      return NULL;
    }

    if (!node->right || (node->left && node->left->weight < node->right->weight))
    {
      node = RotateRight(node);
      node->right = Delete(node->right);
      return node;
    }

    if (!node->left || (node->right && node->right->weight <= node->left->weight))
    {
      node = RotateLeft(node);
      node->left = Delete(node->left);
      return node;
    }

    return node;
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

#endif /*__TREAP_H__*/
