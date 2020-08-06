#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
 private:
  vector<int> data_;
  vector< pair<int, int> > swaps_;

  // When a function is declared as const, it can be called on any type of object, const object as well as non-const objects.
  /* A function becomes const when the const keyword is used in the function’s declaration. The idea of const functions is not to allow them to modify the object on which they are called */
  void WriteResponse() const {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < swaps_.size(); ++i) {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
  }

  void ReadData() {
    int n;
    cin >> n;
    data_.resize(n);
    for(int i = 0; i < n; ++i)
      cin >> data_[i];
  }

  // Note here the array is zero-based indexed.
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

   // min-heap property: A[i] <= children of A[i]
  void sift_down(int i)
  {
    int size = data_.size();
    while(true)
    { 
      // Find the min of the 3 node system
      int min_index(i);
      int left = left_child(i);
      int right = right_child(i);
      if(left < size)
      {
	if(data_[left] < data_[min_index])
	{
	  min_index = left;
	}
      }

      if(right < size)
      {
	if(data_[right] < data_[min_index])
	{
	  min_index = right;
	}
      }

      // termination condition: reach the leaf or does not violate the min-heap property
      if((i >= (size / 2)) || (min_index == i)) { break; }

      // replace the node i with the min 
      // Up to this point,  min_index != i
      swap(data_[i], data_[min_index]);
      swaps_.push_back(make_pair(i, min_index));
      i = min_index;
    }
  }

  void GenerateSwaps() {
    swaps_.clear();
    // The following naive implementation just sorts 
    // the given sequence using selection sort algorithm
    // and saves the resulting sequence of swaps.
    // This turns the given array into a heap, 
    // but in the worst case gives a quadratic number of swaps.
    //
    // TODO: replace by a more efficient implementation
    /* for (int i = 0; i < data_.size(); ++i) */
    /*   for (int j = i + 1; j < data_.size(); ++j) { */
    /*     if (data_[i] > data_[j]) { */
    /*       swap(data_[i], data_[j]); */
    /*       swaps_.push_back(make_pair(i, j)); */
    /*     } */
    /*   } */
    // Time Complexity: O(n) with n as the total number of nodes.
    int n = data_.size();
    for(int i(n/2 - 1); i >= 0; i--)
    {
      sift_down(i);
    }
  }

 public:
  void Solve() {
    ReadData();
    GenerateSwaps();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
