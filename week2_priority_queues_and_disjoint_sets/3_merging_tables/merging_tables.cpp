#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::vector;

/*
Yes, a struct is identical to a class except for the default access level (member-wise and inheritance-wise). (and the extra meaning class carries when used with a template)
Every functionality supported by a class is consequently supported by a struct. You'd use methods the same as you'd use them for a class.
*/

struct DisjointSetsElement {
	// Element represents a original non-merged table
	int size, parent, rank;
	// Except the root, sizes of the all other tables should be zero.
	
	// constructor
	DisjointSetsElement(int size = 0, int parent = -1, int rank = 0):
	    size(size), parent(parent), rank(rank) {}
};


/*
   We can replace several related arrays sharing the indices by one array of custom class.
   Recall from the job_queue. One thread class has 
*/

struct DisjointSets {
	int size;
	int max_table_size;   // for output
	vector <DisjointSetsElement> sets; 

	// Constructor. Implement the makeset to create singleton sets.
	DisjointSets(int size): size(size), max_table_size(0), sets(size) {
		for (int i = 0; i < size; i++)
			sets[i].parent = i;
	}

	int getParent(int table) {
	 // find parent and compress path
	 // Return root of the tree table belongs to.
	  // the root acts as the ID of the tree(set)
	  vector<int> path;
	  path.push_back(table);

	  // find parent. We assume that parent of the root is itself.
	  while(table != sets[table].parent)   // not reach the root
	  {
	    table = sets[table].parent;
	    path.push_back(table);
	  }

	  // Up to this point, reached the root, i.e. table == sets[table].parent
	  // table is the root
	  // compress path by assign root to all elements in path.
	  for(int i(0); i < path.size(); i++)
	  {
	    path[i] = table;
	  }
	  return table;
	}

	void merge(int destination, int source) {
		int realDestination = getParent(destination);
		int realSource = getParent(source);         
		
		if(realDestination == realSource)
		{
		  return;   // already in the same set.
		}

		// two tables are not merged.
		// use union by rank heuristic
		// hang the shorter one to the taller one by updating parent of the root of the shorter one to the root(ID) of taller
		if(sets[realSource].rank > sets[realDestination].rank)
		{
		  sets[realDestination].parent = realSource;
		  sets[realSource].size += sets[realDestination].size;
		  if(sets[realSource].size > max_table_size)
		  {
		    max_table_size = sets[realSource].size;
		  }
		}
		else
		{
		  sets[realSource].parent = realDestination;  // realDestination is the new root
		  // For two trees of same height, resulting height should be increased by 1
		  if(sets[realSource].rank == sets[realDestination].rank)
		  {
		    // Since we compare the rank of the root, it suffices to update the rank of the root only.
		    sets[realDestination].rank += 1; 
		  }
		  sets[realDestination].size += sets[realSource].size;
		  if(sets[realDestination].size > max_table_size)
		  {
		    max_table_size = sets[realDestination].size;
		  }
		}	
	}
};

int main() {
	int n, m;
	cin >> n >> m;

	DisjointSets tables(n);
	// The auto keyword specifies that the type of the variable that is being declared will be automatically deducted from its initializer. In case of functions, if their return type is auto then that will be evaluated by return type expression at runtime.
	// range-based loop similar to that in python.
	for (auto &table : tables.sets) {
		cin >> table.size;
		tables.max_table_size = max(tables.max_table_size, table.size);
	}

	// read merge order and perform merge
	for (int i = 0; i < m; i++) {
		int destination, source;
		cin >> destination >> source;
		// convert 1-based index to 0-based index
                --destination;
                --source;
		
		tables.merge(destination, source);
	        cout << tables.max_table_size << endl;
	}

	return 0;
}
