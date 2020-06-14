#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>   
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif
using namespace std;

class Node;

/*
   Node in graph consist of key, parent and children 
   One key, one parent but  the number of children ranges from 0 to an arbitrary finite integer.
*/
class Node 
{
public:
  int key;
  Node *parent;                   // pointer to the parent
  std::vector<Node *> children;   // using dynamical array to store the pointers to the children

  Node()
  {
    this->parent = NULL;          // initialize the parent of all nodes to NULL
  }

  void set_parent(Node * the_parent)
  {
    parent = the_parent;
    parent->children.push_back(this);
  }
};

int tree_height_breadth_first_search(std::vector<Node> nodes, int root_index)
{
  // The algorithm is adapted from Level Traversal of tree.
  queue<int> q;
  q.push(root_index);

  int height = 0;
  while(true)
  {
    int node_count = q.size();  // number of nodes for one level.
    if(node_count == 0)
    {
      return height;
    }
    
    while(node_count >0)
    {
      // pop out one level of nodes
      int node = q.front();
      q.pop();
      node_count--;

      // add nodes of next level. If no children, do not perform the loop
      for(int i(0); i < nodes[node].children.size(); i++)
      {
	q.push(nodes[node].children[i]->key);
      }

      // node_count of current level makes sure only add next level of noes
    }
    // one level of nodes has been popped out
    height ++;
  }

  return height;
  // O(N)
}

int tree_height_recursive(std::vector<Node> nodes, int root_index)
{
  if(nodes[root_index].children.size() == 0)
  {
    return 1;
  }

  int max_height = 0;
  for(int i(0); i < nodes[root_index].children.size(); i++)
  {
    int height = 1 + tree_height_recursive(nodes, nodes[root_index].children[i]->key);
    if(height > max_height)
    {
      max_height = height;
    }
  }
  // there is a lot of redundancy in the recursion. 
  // Memoization can transform recursion algorithm to dynamic programming.
  // As a result, the time complexity is considerably reduced.
  return max_height;
}

int main_with_large_stack_space() {
  std::ios_base::sync_with_stdio(0);
  int n;
  std::cin >> n;

  std::vector<Node> nodes;      // The vector of nodes represents the corresponding tree.
  nodes.resize(n);
  // The labels for each node are integers in the range of 0 to n-1.
  // In this example, labels for each node amounts to keys for each node.
  // node_index = label = key
  int root_index = 0;
  for (int child_index = 0; child_index < n; child_index++) {
    int parent_index;
    std::cin >> parent_index;
    if(parent_index == -1) { root_index = child_index;}

    if (parent_index >= 0)    // if it is not root
      nodes[child_index].set_parent(&nodes[parent_index]);
    nodes[child_index].key = child_index;
  }

  // Replace this code with a faster implementation
  // Naive algorithm to search starting from every node except root to the root 
  // There is redundancy along the same path.
  /* int maxHeight = 0; */
  /* for (int leaf_index = 0; leaf_index < n; leaf_index++) { */
  /*   int height = 0; */
  /*   for (Node *v = &nodes[leaf_index]; v != NULL; v = v->parent) */
  /*     height++; */
  /*   maxHeight = std::max(maxHeight, height); */
  /* } */
    
  /* return tree_height_breadth_first_search(nodes, root_index); */
  return tree_height_breadth_first_search(nodes, root_index);

}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
      if (rl.rlim_cur < kStackSize)
      {
          rl.rlim_cur = kStackSize;
          result = setrlimit(RLIMIT_STACK, &rl);
          if (result != 0)
          {
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }

#endif
  std::cout << main_with_large_stack_space() << std::endl;
  return 0;
}
