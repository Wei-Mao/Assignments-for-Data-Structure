#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Node {
  public:
  long long key;
  int left;
  int right;

  // overload constructor
  Node() : key(0), left(-1), right(-1) {}
  Node(int key_=0, int left_=-1, int right_=-1) : key(key_), left(left_), right(right_) {}
};

// If you don't create an tree class, you are supposed to use new and delete to allocate the memory dynamically in order to access the memory globally.
class Binary_Tree
{
  vector<Node> tree;

  public:
  void read()
  {
    int nodes; // 
    cin >> nodes;
    for (int i = 0; i < nodes; ++i) 
    {
      int key, left, right;
      cin >> key >> left >> right;
      tree.push_back(Node(key, left, right));
    }
  }

  bool is_binary_search_tree_recursive(int node_idx, long min, long max)
  {
    //  Base case. An empty tree is a bst.
    if (node_idx == -1) {return true;}
    
    if (tree[node_idx].key < min) {return false;}

    // For any node in the left sub-tree, its key must be strictly less than node.key. 
    if (tree[node_idx].key >= max) {return false;} 

    // for each subtree, update  one limit, either min or max.
    return is_binary_search_tree_recursive(tree[node_idx].left, min, tree[node_idx].key) && \
           is_binary_search_tree_recursive(tree[node_idx].right, tree[node_idx].key, max);
  }

  bool IsBinarySearchTree() {
    // The min and max that can be represented inside an integer.
    if(tree.empty()) {return true;}
    long long MIN = std::numeric_limits<long long>::min();
    long long MAX = std::numeric_limits<long long>::max(); 
    // Vertex 0 is the root.
    return is_binary_search_tree_recursive(0, MIN, MAX);
  }
};
int main() {
  Binary_Tree tree;
  tree.read(); 
  if (tree.IsBinarySearchTree()) {
    cout << "CORRECT" << endl;
  } else {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
