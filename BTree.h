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
template <typename Key, typename Value, size_t T>
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

  }

  /**
   * Deletes an entry from the tree
   */
  void Delete(const Key& key)
  {
  }

  /**
   * Finds a value in the tree
   */
  Value& Find(const Key& key)
  {
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
      memset(key, 0, sizeof(key));
      memset(child, 0, sizeof(child));
    }

    ~Node()
    {

    }

    size_t GetHeight()
    {
      size_t height = 0;
      for (size_t i = 0; i <= n; ++i)
      {
        height = std::max(height, child[i]->GetHeight());
      }

      return height + 1;
    }

    size_t  n;
    bool    leaf;
    Item   *key[T * 2];
    Node   *child[T * 2];
  };

  /**
   * Takes as input a node containing 2 * t - 1 keys and its parent,
   * moves the median key from the node up to the parent and creates a new
   * node out of the keys that are above the median
   * @param x  Parent node
   * @param c  Index of median key in parent
   * @param y  Child with 2 * t - 1 keys
   * @return   New node
   */
  Node *Divide(Node *x, size_t c, Node *y)
  {
    Node *z;

    z = new Node();
    z->leaf = y->leaf;

    z->n = T - 1;
    y->n = T - 1;

    for (size_t i = 0; i < T - 1; ++i) {
      z->key[i] = y->key[i + T];
    }

    if (!z->leaf) {
      for (size_t i = 0; i < T; ++i) {
        z->child[i] = y->child[i + T];
      }
    }

    for (size_t i = x->n; i > c; --i) {
      x->child[i] = x->child[i - 1];
    }

    for (size_t i = x->n - 1; i >= c; --i) {
      x->key[i] = x->key[i - 1];
    }

    x->key[c] = y->key[T];
    ++x->n;

    x->child[c] = z;
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
