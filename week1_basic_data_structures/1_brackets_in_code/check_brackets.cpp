#include <iostream>
#include <stack>
#include <string>

struct Bracket {
  // In C++ the only difference between a class and a struct is that members and base classes are private by default in classes, 
  // whereas they are public by default in structs.
    Bracket(char type, int position):
        type(type),
        position(position)            // initializer list
    {}

    // Member Function of struct!
    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    int position;
};

int main() {
    std::string text;
    getline(std::cin, text);

    std::stack <Bracket> opening_brackets_stack;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];

        if (next == '(' || next == '[' || next == '{') {
            // Process opening bracket, write your code here
	  Bracket next_bracket(next, position);   // position is zero-based.
	  opening_brackets_stack.push(next_bracket);
        }

        if (next == ')' || next == ']' || next == '}') {
          // Process closing bracket, write your code here

	  // Fisrt priority is to find the unmatched closing bracket
	  // case 1: no matching opening bracket
	  if (opening_brackets_stack.empty())
	  {
	    std::cout <<  position + 1 << std::endl;
	    return 0;
	  }

	  // case 2: closing the wrong opening bracket
	  Bracket top = opening_brackets_stack.top();
	  opening_brackets_stack.pop();
	  if(!top.Matchc(next))
	  {
	    std::cout << position + 1 << std::endl;
	    return 0;
	  }

        }
    }

    if(opening_brackets_stack.empty())
    {
      std::cout << "Success" << std::endl;
      return 0;
    }
    else
    {
      Bracket current_bracket = opening_brackets_stack.top();
      while(!opening_brackets_stack.empty())
      {
        current_bracket = opening_brackets_stack.top();
	opening_brackets_stack.pop();
      }

      std::cout <<  current_bracket.position + 1 << std::endl;
      return 0;
    }

}
