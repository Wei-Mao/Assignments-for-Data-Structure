#include <iostream>
#include <queue>  // std::priority_queue
using namespace std;

class Compare 
{
  public:
     /* The overloaded () acts as < for the purpose of determining priority */
    // i.e. determine the object of lower priority.
    bool operator () (int a, int b)
    {
      return a > b;
    }

};

int main()
{
  priority_queue<int, vector<int>, Compare> mypq;

  mypq.push(10);
  mypq.push(20);
  mypq.push(30);

  int top = mypq.top();  // only return value
  top = 100;
  cout << "mypq.top() is now: " << mypq.top() << endl;

  mypq.pop();
  mypq.push(100);
  cout << "mypq.top() is now: " << mypq.top() << endl;

  return 0;
}
