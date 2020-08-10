#include <iostream>
#include <cstdlib> // std::rand()
#include <vector>
#include <algorithm>  //std::swap
#include <limits>

/*
   Using array to represent the binary complete max-heap, which in turn implement the priority queue.

*/



class pq_node
{
  public:
    int key;

    pq_node(int _key)
      :key(_key)
    {
    }
};

class Priority_Queue
{
  private:
    std::vector<pq_node> H;  // Remains to be converted to max-heap.
    int int_min = std::numeric_limits<int>::min();
    int int_max = std::numeric_limits<int>::max();
    // We store the binary man-heap as an array of parents. We store the nodes from top to bottom and from left to right.
    /*
		    0
		   / \
		  1   2
		 / \ / \
		3  4 5  6
	Array element at index 0 is the root of the binary max-heap. Leaves are the nodes indexed by floor(H.size() / 2), floor(H.size() /2) + 1, .....
    */

  public:


    Priority_Queue(std::vector<pq_node> data)
      :H(data)
    {
      build_heap();
    }

    bool is_higer_priority(pq_node &left_node, pq_node &right_node)
    {
      /*
	 Return true if left_node takes priority over the right_child.
	 You can the modify the return statement to define what the higher priority means.
      */

      // Here, we define that smaller key means higher priority.
      return left_node.key < right_node.key;
    }

    void assgin_highest_priority(int i)
    {
      H[i].key = int_min; // smaller key means higher priority.
      // H[i].key = int_max; // larger key means higher priority.
    }

    // Note here the array is indexed in zero-based mode.
    int parent(int i)
    {
      return (i - 1) / 2;
    }

    int left_child(int i)
    {
      return 2 * i + 1;
    }

    int right_child(int i)
    {
      return 2 * i + 2;
    }

    void sift_up(int i)
    {
      /*
	 Time Complexity: O(log n) with n as the total number of the nodes in the heap.
      */
      while(i >0 && !is_higer_priority(H[parent(i)], H[i]))
      {
	// continue to shift the node upwards closer to the root
	// if current node is not root and violate the max-heap property
	std::swap(H[parent(i)], H[i]);
	i = parent(i);  // i decreases. 
      }
    }

    void sift_down(int i)
    {
      /*
	 Time Complexity: O(log n) with n as the total number of nodes in the heap.
      */
      // Since it cumbersome to check for the leaf node, we adopt the recursive sift_down scheme.

      int size = H.size();

      while(true)
      {
	// find the node with the largest priority among i and its two children.
	int max_idx = i;
	int l = left_child(i);  // No left child if l >= size. zero-based indexing
	int r = right_child(i); // Possibly, no right child.

	if(l < size && is_higer_priority(H[l], H[max_idx]))
	{
	  max_idx = l;
	}

	if(r < size && is_higer_priority(H[r], H[max_idx]))
	{
	  max_idx = r;
	}

	if((i >= (size / 2)) || (max_idx == i))
	{
	  // Note that: With our representation, the leaves are the nodes indexed by floor(H.size()) / 2, floor(H.size()) / 2 + 1,.......(zero-based indexing)
	  // If reaching the leaf or finishing fix
	  // stop sift_down

	  break;
	}
	else
	{
	  // max-heap priority is violated. We need to swap i and its child with higher priority.
	  std::swap(H[i], H[max_idx]);
	}
      }
    }

    void insert(pq_node p)
    {
      H.push_back(p);  // add p as the leaf in the left-most vacant position
      sift_up(H.size() - 1); // zero-based index
    }

    pq_node extract_max()
    {
      /* Return the pq_node with the highest priority.
	 Time Complexity: O(log H.size())
      */
      pq_node res =  H[0];  // root
      H[0] = H[H.size() - 1]; // swap root and last leaf.
      // Node of highest priority is removed from the heap
      sift_down(0);
      return res;
    }

    // Bug: How do you know the i for the element of  interest?
    void remove(int i)
    {
      /*
	 Time Complexity: O(log H.size())
      */
      assgin_highest_priority(i); // Modify key such that H[i] becomes the node of highest priority.
      sift_up(i);  // move it to the root position.
      extract_max();  //  remove the new root.
    }

    // Bug: How do you know the i for the element of interest?
    void change_priority(int i, pq_node p)
    {
      /*
	 Time Complexity: O(log H.size())
      */
      pq_node old_node = H[i];
      H[i] = p;
      if(is_higer_priority(p, old_node))
      {
	//p takes precedence over old_node;
	sift_up(i);
      }
      else
      {
	sift_down(i);
      }
    }

    void build_heap()
    {
      /*
	 Basic Idea: sift_down all non-leaf nodes to the get the right max-heap. 
	 Time Complexity: O(H.size())
      */
      int n = H.size();
      for(int i(n/2 - 1); i >= 0; i--)
      {
	sift_down(i);
      }
    }
};

int main()
{
  bool sucess = true;
  while(sucess)
  {
    std::vector<pq_node> data;
    int length = 100;
    int upper_bound = 1000;
    int min_key = std::numeric_limits<int>::max();
    for(int i(0); i < length; i++)
    {
      int random_int = std::rand() % upper_bound;
      min_key = random_int <  min_key ? random_int : min_key;
      data.push_back(pq_node(random_int));
    }

    // smaller key means highest priority.
    Priority_Queue pq(data);

    /*
       Note that extract_max() means return and remove the node with maximum priority!
    */
    /* // test extract_max() and build_heap() */
    /* if(pq.extract_max().key != min_key) */
    /* { */
    /*   std::cout << "Fail!" << std::endl; */
    /*   std::cout << pq.extract_max().key <<  " and " << min_key; */
    /*   break; */
    /* } */
    /* else */
    /* { */
    /*   std::cout << "Success!" << std::endl; */
    /* } */

    // you have to comment out the above code with pq.extract_max being executed.
    // test insert()
    int inserted = std::rand() % upper_bound;
    if(inserted < min_key) {min_key = inserted;}
    data.push_back(inserted);
    pq.insert(pq_node(inserted));

    if(pq.extract_max().key != min_key)
    {
      std::cout << "Fail on insertion" << std::endl;
      std::cout << pq.extract_max().key <<  " and " << min_key;
      break;
    }
    else
    {
      std::cout << "Success on insertion!" << std::endl;
    }

  }
}
