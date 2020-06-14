#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

using std::cin;
using std::string;
using std::vector;
using std::cout;
using std::max_element;

class StackWithMax {
    vector<int> stack;
    vector<int> stack_auxiliary;

  public:

    void Push(int value) {
        stack.push_back(value);
	if(stack.empty())
	{
	  stack_auxiliary.push_back(value);
	  max_value = value;
	}
	else
	{
	  if(value <= max_value)
	  {
	    stack_auxiliary.push_back(value);
	  }
	  else
	  {
	    stack_auxiliary.push_back(2 * value - max_value);  // 2 *value -max_value > value.
	    max_value = value;
	    // The element greater than the current maximum encodes the immediately previous maximum.
	  }
	}
	
    }

    void Pop() {
        assert(stack.size());   // check empty before and after popping!
	int pop_value = stack_auxiliary.back();
        stack.pop_back();
	stack_auxiliary.pop_back();
	// already check the empty.
	if(pop_value > max_value)
	{
	  max_value = 2 * max_value - pop_value; // previous maximum.
	}

    }

    int Max() const {
        assert(stack.size());
        return max_value;
    }

    int max_value;
};

int main() {
    int num_queries = 0;
    cin >> num_queries;
    std::vector<int> arr_max;

    string query;
    string value;

    StackWithMax stack;

    for (int i = 0; i < num_queries; ++i) {
        cin >> query;
        if (query == "push") {
            cin >> value;
            stack.Push(std::stoi(value));   // convert the string to integer.
        }
        else if (query == "pop") {
            stack.Pop();
        }
        else if (query == "max") {
            /* cout << stack.Max() << "\n"; */
	  arr_max.push_back(stack.Max());
        }
        else {
            assert(0);
        }
    }

    for(int i(0); i < arr_max.size(); i++)
    {
      std::cout << arr_max[i] << "\n";
    }
    return 0;
}
