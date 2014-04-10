#ifndef __TREE_H__
#define __TREE_H__

template <typename Key, typename Value>
class Tree
{
public:
  /**
   * Inserts a value into the tree
   */
  virtual void Insert(const Key& key, const Value &value) = 0;

  /**
   * Deletes an entry from the tree
   */
  virtual void Delete(const Key& key) = 0;

  /**
   * Finds a value in the tree
   */
  virtual Value& Find(const Key& key) = 0;

  /**
   * Returns the number of items in the tree
   */
  virtual size_t GetSize() = 0;

  /**
   * Returns the height of the tree
   */
  virtual size_t GetHeight() = 0;
};

#endif /*__TREE_H__*/
