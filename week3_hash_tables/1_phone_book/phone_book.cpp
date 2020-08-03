#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cin;

// class for both query and contact(name + number)
struct Query {
    string type, name;
    int number;
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else // find by number or delete by number
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string>& result) {
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";  // output of the find query!
}

vector<string> process_queries(const vector<Query>& queries) {
    vector<string> result;
    // Keep list of all existing (i.e. not deleted yet) contacts.
    vector<Query> contacts;  // empty vector
    for (size_t i = 0; i < queries.size(); ++i)
        if (queries[i].type == "add") {
            bool was_founded = false;
            // if we already have contact with such number,
            // we should rewrite contact's name
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts[j].name = queries[i].name;
                    was_founded = true;
                    break;  // beak out of the loop
                }
            // otherwise, just add it
            if (!was_founded)   // not exist
                contacts.push_back(queries[i]);
        } else if (queries[i].type == "del") {
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts.erase(contacts.begin() + j);   // do not forget to add iterator .begin()
                    break;
                }
        } else {   // "find by number"
            string response = "not found";
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    response = contacts[j].name;
                    break;
                }
            result.push_back(response);
        }
    // for each query, you have to scan the vector of contacts, hence O(n) with n is maximum of the numbers of contacts occurring in the book.
    return result;
}

vector<string> process_queries_fast(const vector<Query> &queries)
{
  /*
     All queries work in constant time but space-inefficient.
  */
  // adopt the direct addressing scheme. That is, contacts[phone_number] = name.
  vector<string> result;
  vector<string> contacts(10000000, "nil");
  for(int i(0); i < queries.size(); i++)
  {
    if(queries[i].type == "add")
    { 
      // If we already have contact with such number, just rewrite the contact's name
      // no need to check.
	contacts[queries[i].number] = queries[i].name;
    }
    else if(queries[i].type == "del")
    {
      contacts[queries[i].number] = "nil";
    }
    else // find by number
    {
      string response = "not found";
      if(contacts[queries[i].number] != "nil")
      {
	response = contacts[queries[i].number];
      }
      result.push_back(response);
    } 
  }

  return result;
}

int main() {
    write_responses(process_queries_fast(read_queries()));
    return 0;
}
