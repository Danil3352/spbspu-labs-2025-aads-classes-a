#include <iostream>
#include <stack>
namespace
{
  template< class T, class Cmp >
  struct TriTree {
    std::pair< T, T > data;
    TriTree< T, Cmp > * left, * mid, * right, * parent;
  };

  template< class T, class Cmp >
  struct TriTreeIterator;

  template< class T, class Cmp >
  TriTreeIterator< T, Cmp > begin(TriTree< T, Cmp > * root);

  template< class T, class Cmp >
  struct TriTreeIterator {
    using this_t = TriTreeIterator< T, Cmp >;
    friend TriTreeIterator< T, Cmp > begin<>(TriTree< T, Cmp > * root);
    bool hasNext() const
    {
      if (stack.empty() && check_last == 1)
      {
        return false;
      }
      return true;
    }
    this_t next() const
    {
      TriTreeIterator< T, Cmp > temp = *this;
      if (stack.empty())
      {
        temp.check_last = 1;
        return temp;
      }
      temp.current = temp.stack.top();
      temp.stack.pop();
      return temp;
    }

    std::pair< T, T > & data()
    {
      return current->data;
    }
  private:
    TriTree< T, Cmp > * current;
    int check_last = 0;
    std::stack< TriTree< T, Cmp > * > stack;
    TriTree< T, Cmp > * fillStack(TriTree< T, Cmp > * current);
  };

  template< class T, class Cmp >
  size_t intersects(TriTree< T, Cmp >* root, T& v1, T& v2)
  {
    if (!root)
    {
      return 0;
    }
    size_t count = 0;
    for (auto it = begin(root); it.hasNext(); it = it.next())
    {
      if (v2 >= it.data().first && v1 <= it.data().second)
      {
        count++;
      }
    }
    return count;
  }

  template< class T, class Cmp >
  size_t covers(TriTree< T, Cmp >* root, T& v1, T& v2)
  {
    if (!root)
    {
      return 0;
    }
    size_t count = 0;
    for (auto it = begin(root); it.hasNext(); it = it.next())
    {
      if (v1 <= it.data().first && v2 >= it.data().second)
      {
        count++;
      }
    }
    return count;
  }

  template< class T, class Cmp >
  size_t avoids(TriTree< T, Cmp >* root, T& v1, T& v2)
  {
    if (!root)
    {
      return 0;
    }
    size_t count = 0;
    for (auto it = begin(root); it.hasNext(); it = it.next())
    {
      if (v1 > it.data().second || v2 < it.data().first)
      {
        count++;
      }
    }
    return count;
  }

  template< class T, class Cmp >
  TriTree< T, Cmp > * TriTreeIterator< T, Cmp >::fillStack(TriTree< T, Cmp > * current)
  {
    if (current == nullptr)
    {
      return nullptr;
    }
    stack.push(current);
    fillStack(current->right);
    fillStack(current->mid);
    fillStack(current->left);
    TriTree< T, Cmp > * temp = stack.top();
    return temp;
  }

  template< class T, class Cmp >
  TriTreeIterator< T, Cmp > begin(TriTree< T, Cmp > * root)
  {
    TriTreeIterator< T, Cmp > temp;
    temp.current = temp.fillStack(root);
    temp.stack.pop();
    return temp;
  }


  template< class T, class Cmp >
  TriTree< T, Cmp >* insertValue(TriTree< T, Cmp >* const root, const std::pair< T, T >& value, Cmp cmp)
  {
    if (root == nullptr)
    {
      return new TriTree< T, Cmp >{value, nullptr, nullptr, nullptr, nullptr};
    }
    else if (cmp(value.second, root->data.first) && cmp(value.first, root->data.first))
    {
      root->left = insertValue(root->left, value, cmp);
      if (root->left->parent == nullptr)
      {
        root->left->parent = root;
      }
    }
    else if (cmp(value.second, root->data.second) && cmp(value.first, root->data.second))
    {
      root->mid = insertValue(root->mid, value, cmp);
      if (root->mid->parent == nullptr)
      {
        root->mid->parent = root;
      }
    }
    else if (cmp(root->data.second, value.second) && cmp(root->data.second, value.first))
    {
      root->right = insertValue(root->right, value, cmp);
      if (root->right->parent == nullptr)
      {
        root->right->parent = root;
      }
    }
    else
    {
      return nullptr;
    }
    return root;
  }

  template< class T, class Cmp >
  void clear(TriTree< T, Cmp >* root)
  {
    if (root)
    {
      clear(root->left);
      clear(root->mid);
      clear(root->right);
      delete root;
    }
  }
}

int main()
{
  size_t size = 0;
  std::cin >> size;
  if (!std::cin >> size)
  {
    std::cerr << "Wrong input\n";
    return 1;
  }
  TriTree< int, std::less< int > >* root = nullptr;
  int value1 = 0, value2 = 0;
  for (size_t i = 0; i < size; ++i)
  {
    if (!(std::cin >> value1 >> value2))
    {
      clear(root);
      std::cerr << "Wrong input\n";
      return 1;
    }
    try
    {
      root = insertValue(root, std::make_pair(value1, value2), std::less<int>());
    }
    catch (const std::bad_alloc&)
    {
      clear(root);
      std::cerr << "Memory error\n";
      return 1;
    }
  }
  std::string command = "";
  while (std::cin >> command)
  {
    int val1 = 0, val2 = 0;
    if (!(std::cin >> val1 >> val2) || (val2 < val1))
    {
      std::cout << "INVALID COMMAND\n";
      continue;
    }
    if (command == "covers")
    {
      std::cout << covers(root, val1, val2) << "\n";
    }
    else if (command == "avoids")
    {
      std::cout << avoids(root, val1, val2) << "\n";
    }
    else if (command == "intersects")
    {
      std::cout << intersects(root, val1, val2) << "\n";
    }
    else
    {
      clear(root);
      std::cerr << "Error: not this command";
      return 1;
    }
  }
  clear(root);
  return 0;
}
