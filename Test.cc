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

template <class T, int N = 20>
class TreeTest
{
public:
  TreeTest()
    : tree(new T())
  {
  }

  void Run()
  {
    TestInsertDelete();
    TestInsertDuplicate();
  }

private:

  void TestInsertDelete()
  {
    for (int i = 0; i < N; ++i)
    {
      tree->Insert(i, i);
    }

    assert(tree->GetSize() == N);
    for  (int i = 0; i < N; ++i)
    {
      assert(tree->Find(i) == i);
    }

    for (int i = 0; i < N; ++i)
    {
      tree->Delete(i);
    }

    assert(tree->GetSize() == 0);
  }

  void TestInsertDuplicate()
  {
    for (int i = 0; i < N; ++i)
    {
      tree->Insert(i, i);
    }

    assert(tree->GetSize() == N);
    for (int i = 0; i < N / 2; ++i)
    {
      tree->Insert(i, N / 2 - i);
    }

    assert(tree->GetSize() == N);
    for  (int i = 0; i < N; ++i)
    {
      assert(tree->Find(i) == ((i >= (N / 2)) ? i : (N / 2 - i)));
    }
  }

  std::auto_ptr<Tree<int, int>> tree;
};

int main()
{
  (TreeTest<Treap<int, int>>()).Run();
  (TreeTest<AVLTree<int, int>>()).Run();
  (TreeTest<RBTree<int, int>>()).Run();
  (TreeTest<BTree<int, int, 2>>()).Run();
  return 0;
}
