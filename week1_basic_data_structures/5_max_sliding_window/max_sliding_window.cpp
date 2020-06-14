#include <iostream>
#include <vector>
#include <deque>
/*
   For more details on deque in STL, refer to the following post.
   https://www.tutorialspoint.com/cplusplus-program-to-implement-deque-in-stl#:~:text=Double%20Ended%20Queue%20is%20a,both%20front%20and%20rear%20positions.
   */
using std::cin;
using std::cout;
using std::vector;
using std::max;

void max_sliding_window_naive(vector<int> const & A, int w) {
    for (size_t i = 0; i < A.size() - w + 1; ++i) {
        int window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = max(window_max, A.at(j));

        cout << window_max << " ";
    }

    return;
}

void max_sliding_window_deque(vector<int> const & A, int w)
{
  // It seems more than O(n) at first look. It can be observed that every element of array is added and removed at most once. So there are total 2n operations.
  // The deque will store indices of relevant elements in every window and  // it will maintain decreasing order of the values from front to the rear in deque, namely that A[deque.font()] to A[deque.back()]

  std::deque<int> deq;
  // Initial window
  for(int i(0); i < w; i++)
  {
    // removing useless elements
    // Note that check whether the deque is empty before removal
    // Make sure elements before i is >= A[i]
    // Finally, descending order comes into being.
    while(!deq.empty() && A[i] >= A[deq.back()])
    {
      deq.pop_back();
    }

    deq.push_back(i);
  }
  cout << A[deq.front()] << " ";

  // scan from A[w] to A[A.size() - 1]
  for(int j(w); j < A.size(); j++)
  {
    // remove all indices out of the current window
    // remove indices <= j-w 
    while(!deq.empty() && (deq.front() <= (j - w)))
    {
      deq.pop_front();
    }

    // removing all elements smaller than A[j]
    while(!deq.empty() && A[j] >= A[deq.back()])
    {
      deq.pop_back();
    }

    // adding the current index to the window.
    // still maintain descending order of the values from front to back
    deq.push_back(j);
    cout << A[deq.front()] << " ";
  }

}



int main() {
    int n = 0;
    cin >> n;

    vector<int> A(n);
    for (size_t i = 0; i < n; ++i)
        cin >> A.at(i);

    int w = 0;
    cin >> w;

    /* max_sliding_window_naive(A, w); */
    max_sliding_window_deque(A, w);

    return 0;
}
