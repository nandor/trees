#include <cassert>
#include <cstring>
#include <chrono>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include "Tree.h"
#include "Treap.h"
#include "BTree.h"
#include "RBTree.h"
#include "AVLTree.h"
using namespace std;

template <class T>
class TreeTest
{
public:
  TreeTest()
    : tree(new T())
  {
  }

  void Run()
  {
    TestInsert();
    TestInsertDuplicate();
    TestInsertDelete();
  }

private:

  void TestInsert()
  {
    for (int i = 0; i < 1000; ++i)
    {
      tree->Insert(i, i);
    }

    assert(tree->GetSize() == 1000);
    for  (int i = 0; i < 1000; ++i)
    {
      assert(tree->Find(i) == i);
    }
  }

  void TestInsertDuplicate()
  {
    for (int i = 0; i < 1000; ++i)
    {
      tree->Insert(i, i);
    }

    assert(tree->GetSize() == 1000);
    for (int i = 0; i < 500; ++i)
    {
      tree->Insert(i, 500 - i);
    }

    assert(tree->GetSize() == 1000);
    for  (int i = 0; i < 1000; ++i)
    {
      assert(tree->Find(i) == (i >= 500 ? i : (500 - i)));
    }
  }

  void TestInsertDelete()
  {
    for (int i = 0; i < 1000; ++i)
    {
      tree->Insert(i, i);
    }

    for (int i = 0; i < 1000; ++i)
    {
      tree->Delete(i);
    }

    assert(tree->GetSize() == 0);
  }

  std::auto_ptr<Tree<int, int>> tree;
};

int main()
{
  (TreeTest<Treap<int, int>>()).Run();
  (TreeTest<AVLTree<int, int>>()).Run();
  (TreeTest<RBTree<int, int>>()).Run();
  (TreeTest<BTree<int, int, 10>>()).Run();
  return 0;
}
