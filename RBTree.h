#ifndef __RBTREE_H__
#define __RBTREE_H__

template<typename Key, typename Value>
class RBTree : public Tree<Key, Value>
{
public:
  /**
   * Creates an empty Red-Black tree
   */
  RBTree()
    : root(NULL)
    , size(0)
  {
  }

  /**
   * Destroys the Red-Black tree
   */
  ~RBTree()
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
    Node *node = root, *parent = NULL;

    if (!root)
    {
      root = new Node();
      root->red = false;
      root->key = key;
      root->value = value;
      ++size;
    }
    else
    {
      while (node)
      {
        parent = node;
        if (key < node->key)
        {
          node = node->left;
        }
        else if (key == node->key)
        {
          node->value = value;
          return;
        }
        else
        {
          node = node->right;
        }
      }

      node = new Node();
      node->red = true;
      node->key = key;
      node->value = value;
      node->parent = parent;
      ++size;

      if (node->key < parent->key)
      {
        parent->left = node;
      }
      else
      {
        parent->right = node;
      }

      InsertFixup(node);
    }
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
        break;
      }
    }

    if (!node || node->key != key)
    {
      throw std::runtime_error("Key not found");
    }

    bool red = true;
    Node *sub, *succ;
    if (!node->left)
    {
      Transplant(node, sub = node->right);
    }
    else if (!node->right)
    {
      Transplant(node, sub = node->left);
    }
    else
    {
      succ = Successor(node);
      red = succ->red;
      sub = succ->right;

      if (succ->parent == node)
      {
        sub->parent = node;
      }
      else
      {
        Transplant(succ, succ->right);
        succ->right = node->right;
        succ->right->parent = succ;
      }

      Transplant(node, succ);
      succ->left = node->left;
      succ->left->parent = succ;
      succ->red = node->red;
    }

    node->left = node->right = NULL;
    delete node;
    --size;

    if (!red)
    {
      DeleteFixup(sub);
    }
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
      : red(false)
      , parent(NULL)
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
     * Returns the height of a node
     */
    size_t GetHeight()
    {
      size_t h = 0;

      if (left)
      {
        h = std::max(h, left->GetHeight());
      }

      if (right)
      {
        h = std::max(h, right->GetHeight());
      }

      return h + 1;
    }

  public:
    /**
     * True if the node is red
     */
    bool red;

    /**
     * Key of the node
     */
    Key key;

    /**
     * Value stored in the node
     */
    Value value;

    /**
     * Parent link
     */
    Node *parent;

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
  void RotateLeft(Node *x)
  {
    Node *y;

    y = x->right;
    x->right = y->left;
    if (y->left)
    {
      y->left->parent = x;
    }

    y->parent = x->parent;
    y->left = x;

    if (!x->parent)
    {
      root = y;
    }
    else if (x == x->parent->left)
    {
      x->parent->left = y;
    }
    else
    {
      x->parent->right = y;
    }
    x->parent = y;
  }

  /**
   * Rotates a node right
   */
  void RotateRight(Node *y)
  {
    Node *x;

    x = y->left;
    y->left = x->right;
    if (x->right)
    {
      x->right->parent = y;
    }

    x->parent = y->parent;
    if (!y->parent)
    {
      root = x;
    }
    else if (y == y->parent->left)
    {
      y->parent->left = x;
    }
    else
    {
      y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
  }

  /**
   * Replaces a node with another one
   */
  void Transplant(Node *dest, Node *src)
  {
    if (dest->parent == NULL)
    {
      root = src;
    }
    else if (dest == dest->parent->left)
    {
      dest->parent->left = src;
    }
    else
    {
      dest->parent->right = src;
    }

    if (src)
    {
      src->parent = dest->parent;
    }
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
   * Restores invariant after inserting a node
   */
  void InsertFixup(Node *z)
  {
    Node *uncle;
    while (z->parent && z->parent->red)
    {
      if (z->parent == z->parent->parent->left)
      {
        uncle = z->parent->parent->right;
        if (uncle && uncle->red)
        {
          z->parent->red = false;
          uncle->red = false;
          z->parent->parent->red = true;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->right)
          {
            z = z->parent;
            RotateLeft(z);
          }

          z->parent->red = false;
          z->parent->parent->red = true;
          RotateRight(z->parent->parent);
        }
      }
      else
      {
        uncle = z->parent->parent->left;
        if (uncle && uncle->red)
        {
          z->parent->red = false;
          uncle->red = false;
          z->parent->parent->red = true;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->left)
          {
            z = z->parent;
            RotateRight(z);
          }

          z->parent->red = false;
          z->parent->parent->red = true;
          RotateLeft(z->parent->parent);
        }
      }
    }

    root->red = false;
  }

  /**
   * Restores the invariant after removing a node
   */
  void DeleteFixup(Node *node)
  {
    Node *sibling;
    while (node != root && !node->red)
    {
      if (node == node->parent->left)
      {
        sibling = node->parent->right;
        if (sibling->red)
        {
          sibling->red = false;
          node->parent->red = true;
          RotateLeft(node->parent);
          sibling = node->parent->right;
        }

        if ((!sibling->left || !sibling->left->red) &&
            (!sibling->right || !sibling->right->red))
        {
          sibling->red = true;
          node = node->parent;
        }
        else
        {
          if (!sibling->right->red)
          {
            sibling->red = true;
            sibling->left->red = false;
            RotateRight(sibling);
            sibling = node->parent->right;
          }

          sibling->red = node->parent->red;
          node->parent->red = false;
          sibling->left->red = false;
          RotateLeft(node->parent);
          node = root;
        }
      }
      else
      {
        sibling = node->parent->left;
        if (sibling->red)
        {
          sibling->red = false;
          node->parent->red = true;
          RotateRight(node->parent);
          sibling = node->parent->left;
        }

        if ((!sibling->left || !sibling->left->red) &&
            (!sibling->right || !sibling->right->red))
        {
          sibling->red = true;
          node = node->parent;
        }
        else
        {
          if (!sibling->left->red)
          {
            sibling->red = true;
            sibling->right->red = false;
            RotateLeft(sibling);
            sibling = node->parent->left;
          }

          sibling->red = node->parent->red;
          node->parent->red = false;
          sibling->right->red = false;
          RotateRight(node->parent);
          node = root;
        }
      }
    }

    node->red = false;
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

#endif /*__RBTREE_H__*/
