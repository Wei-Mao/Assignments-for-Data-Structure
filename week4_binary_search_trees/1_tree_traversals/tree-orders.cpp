#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::vector;
using std::ios_base;
using std::cin;
using std::cout;
using std::queue;
using std::stack;

// you can build a class composed of three attributes: key, left, and right
// and then use vector of nodes to represent the tree.

/*
   class data member: nodes of binary tree
   class function member: read subroutine, various traversal depth first traversal methods and print methods
*/
class TreeOrders {
  int n;
  vector <int> key;
  vector <int> left;
  vector <int> right;
  // vertex zero is the root

public:
  void read() {
    cin >> n;
    key.resize(n);
    left.resize(n);
    right.resize(n);
    for (int i = 0; i < n; i++) {
      cin >> key[i] >> left[i] >> right[i]; // continuously to read in the input information about vertices.
    }
  }


  void in_order_DF(int node_id, vector<int> &result)
  {
    /* print out the key in between traverse left and right sub-trees.
       Input:
	  node_id(int): ID of the root node
	  result(&vector<int>): Store the traversed keys.
    */

    if(node_id == -1)
    {
      return;
    }

    in_order_DF(left[node_id], result);
    result.push_back(key[node_id]);
    in_order_DF(right[node_id], result);
  }

  vector <int> in_order() {
    /* print out the key in between traverse left and right sub-trees.
       Output(vector): result of keys printed by in-order depth-first method.
    */
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that
    in_order_DF(0, result);

    return result;
  }

  void pre_order_DF(int node_id, vector<int> &result)
  {
    /* print out the key in before traversing left and right sub-trees.
       Input:
	  node_id(int): ID of the root node
	  result(&vector<int>): Store the traversed keys.
    */

    if(node_id == -1) //  reach the leaf
    {
      return;
    }

    result.push_back(key[node_id]);
    pre_order_DF(left[node_id], result);
    pre_order_DF(right[node_id], result);
  }
  vector <int> pre_order() {
    vector<int> result;    
    // Finish the implementation
    // You may need to add a new recursive method to do tha
    pre_order_DF(0, result) ;
    return result;
  } void post_order_DF(int node_id, vector<int> &result) { /* print out the key in after traversing left and right sub-trees.  Input:
	  node_id(int): ID of the root node
	  result(&vector<int>): Store the traversed keys.
    */

    if(node_id == -1) //  reach the leaf
    {
      return;
    }

    post_order_DF(left[node_id], result);
    post_order_DF(right[node_id], result);
    result.push_back(key[node_id]);
  }

  vector <int> post_order() {
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that
    post_order_DF(0, result); 
    return result;
  }

  vector<int> level_traversal()
  {
    /* Breadth-first traversal.
       Output(vector<int>): traversed keys.
       Remarks: Starting at root. Output least-recently keys. Hence, we need queue as the dynamic data structure to store the nodes while traversing.
    */
    vector<int> result;
    
    // If tree is empty, return an empty result.  
    if(key.empty()) { return result;}
    queue<int>  queue_idx;
    queue_idx.push(0);
    // continuously output.
    // output one node, but may add 2 nodes. Hence, dynamic data structure is better.
    while(!queue_idx.empty())
    {
      // Output the least-recently accessed node, and add their children. and then remove it from the queue.
      int node_idx = queue_idx.front();
      result.push_back(key[node_idx]);
      queue_idx.pop();
      if(left[node_idx] != -1)
      {
	queue_idx.push(left[node_idx]);
      }
      if(right[node_idx] != -1)
      {
	queue_idx.push(right[node_idx]);
      }
    }
  }

  vector<int> in_order_traverse() {
    vector<int> printed;
    stack<int> stack_visited;
    int current_node = 0;
    while((current_node != -1) || (!stack_visited.empty()))
    {
      if(current_node != -1)
      {
	stack_visited.push(current_node);
	current_node = left[current_node];
      }
      else
      {
	// current_node ==  NULL
	int pushed = stack_visited.top(); 
	stack_visited.pop();
	printed.push_back(key[pushed]);
	current_node = right[pushed];
      } 
    }
    return printed;
  }
};

void print(vector <int> a) {
  for (size_t i = 0; i < a.size(); i++) {
    if (i > 0) {
      cout << ' ';  // No space in front of the first output.
    }
    cout << a[i];
  }
  cout << '\n';
}

int main_with_large_stack_space() {
  ios_base::sync_with_stdio(0);
  TreeOrders t;
  t.read();
  print(t.in_order());
  print(t.pre_order());
  print(t.post_order());
  /* print(t.level_traversal()); */
  print(t.in_order_traverse());
  return 0;
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

  return main_with_large_stack_space();
}

