/*
   Disjoint Sets support the following operations:
   make_set(x): creates a singleton set {x}
   find(x): returns ID of the set containing x
   union(x, y): merges two sets containing x and y,respectively.
*/
#include <iostream>
#include <vector>
using std::vector;

class Disjoint_Sets_Element 
{
  public:
    // Element represents an original single element.
    int parent, rank;

    // constuctor
    Disjoint_Sets_Element(int parent = -1, int rank = 0)
      :parent(parent), rank(rank)
    {}
};

class Disjoint_Sets 
{
  public:
    vector<Disjoint_Sets_Element> sets;  // singleton set, one for one element
    int size;
    int num_of_sets;

    // constuctor. Implement the maks_set to create singleton sets
    // Make_Set(i)
    Disjoint_Sets(int _size)
      :size(_size), num_of_sets(_size)
    {
      sets.resize(_size);
      for(int i(0); i < _size; i++)
      {
	sets[i].parent = i;  // root of singleton set is itself
	sets[i].rank = 0;    // one node tree is of zero height.
      }
    }
    
    int find(int i)
    {
      /* Return the ID of the set i belongs to.
	 Input:
	      i(int):  the index of the element of interest.
	 Return: the ID of the set containing element indexed by i
      */
      while(i != sets[i].parent)
      {
	i = sets[i].parent;
      }

      return i; // root index as the set ID
    }

    void merge(int i, int j)
    {
      /* Merging sets containing elements indexed by i and j.
	 Input:
	      i and j: the index of the two elements.
      */

      int i_id = find(i);
      int j_id = find(j);

      if (i_id == j_id)
      {
	// the given two elements already lie in the same set
	return;
      } else if (sets[i_id].rank < sets[j_id].rank)
      {
	num_of_sets --;
	// set containing i is shorter than set containing j
	// Hang a shorter tree under the root of the taller one, aka union by rank heuristic
	// update the parent of the root of the shorter to be the root of the taller.
	// rank of nodes in the shorter one is preserved.
	sets[i_id].parent = j_id;
      } else
      {
	num_of_sets--;
	// set containing i is taller than or as high as the one containing j.
	sets[j_id].parent = i_id;
	if (sets[i_id].rank == sets[j_id].rank)
	{
	  // increased the height of the tree to which a tree of the same height is attached.
	  sets[i_id].rank ++;
	}
      }
    }

    void print_state()
    {
      std::cout << "parent" << std::endl;
      for(int i(0); i < size; i++)
      {
	std::cout << sets[i].parent << " ";
      }
      std::cout << std::endl;

      std::cout << "rank" << std::endl;
      for(int i(0); i < size; i++)
      {
	std::cout << sets[i].rank << " ";
      }
      std::cout << std::endl;
    }
};

int main ()
{
  int size = 6;
  Disjoint_Sets dis_sets(size);
  dis_sets.merge(1, 3);
  dis_sets.print_state();
  dis_sets.merge(4, 1);
  dis_sets.print_state();
  dis_sets.merge(2, 0);
  dis_sets.print_state();
  dis_sets.merge(1, 2);
  dis_sets.print_state();
  dis_sets.merge(1, 5);
  dis_sets.print_state();
  std::cout << "Total number of disjoint sets: " << dis_sets.num_of_sets << std::endl;
}
