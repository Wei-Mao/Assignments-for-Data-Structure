#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // std::find

using std::string;
using std::vector;
using std::cin;

struct Query {
    string type, s;
    size_t ind;
};

class QueryProcessor {
    int bucket_count;
    // store all strings in one vector
    vector<string> elems;
    int m=1;
    vector<vector<string>> hash_table;  // An array of arrays of strings.
    // Here we use dynamic array in place of the list.
    size_t hash_func(const string& s) const {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;  
	/*
	    When the characters are used with the arithmetic operations, it converts them into integer value automatically i.e. ASCII value of characters.
	*/
        return hash % bucket_count;
    }

public:
    // Default constructor
    explicit QueryProcessor(int bucket_count): 
      bucket_count(bucket_count) 
    {
      hash_table.resize(bucket_count);   
    }

    Query readQuery() const {
      // only read one query
        Query query;
        cin >> query.type;
        if (query.type != "check")  // del or add or find followed by string
            cin >> query.s;
        else  // check out the indth list in 
            cin >> query.ind;
        return query;
    }

    void writeSearchResult(bool was_found) const {
        std::cout << (was_found ? "yes\n" : "no\n");
    }

    // process one single query
    void processQuery_naive(const Query& query) {
      // Strings(dynamic unordered set of strings) are stored in a dynamic array instead of hash table.
        if (query.type == "check") {
            // use reverse order, because we append strings to the end
            for (int i = static_cast<int>(elems.size()) - 1; i >= 0; --i)
                if (hash_func(elems[i]) == query.ind)
                    std::cout << elems[i] << " ";
            std::cout << "\n";
        } else {
	  // vector.find():
	  // Reference at https://www.geeksforgeeks.org/std-find-in-cpp/
	  // An iterator to the first element in the range that compares equal to val.
          //If no elements match, the function returns last.
	   // use find to check presence of the given elem for find, del or add
            vector<string>::iterator it = std::find(elems.begin(), elems.end(), query.s);
            if (query.type == "find")
                writeSearchResult(it != elems.end());   // found true, not found false
            else if (query.type == "add") {
                if (it == elems.end())   // not found
                    elems.push_back(query.s);
            } else if (query.type == "del") {
                if (it != elems.end())  // found
                    elems.erase(it);
            }
        }
    }

    void process_query_hash(const Query & query)
    { 
      if(query.type == "check")
      {
	if(hash_table[query.ind].empty())  // not found
	{
	  std::cout << "\n";
	}
	else  // found
	{
	  for(int i(hash_table[query.ind].size() - 1); i >= 0; i--)
	  {
	    std::cout << hash_table[query.ind][i] << " ";
	  }
	  std::cout << "\n";
	}
      }
      else
      {
	// check whether the string is already present in the hash_table
	int hash_value = hash_func(query.s);
	vector<string>::iterator it = std::find(hash_table[hash_value].begin(), hash_table[hash_value].end(), query.s);
	if(query.type == "find")
	{
	  writeSearchResult(it != hash_table[hash_value].end()); // 
	} else if(query.type == "add")
	{
	  if(it == hash_table[hash_value].end())  // not found
	  {
	    hash_table[hash_value].push_back(query.s);
	  }
	} else if(query.type == "del")
	{
	  if(it != hash_table[hash_value].end())  // found
	  {
	    hash_table[hash_value].erase(it);
	  }
	}

      }
    }

    void processQueries() {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            process_query_hash(readQuery());
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    int bucket_count;
    cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.processQueries();
    return 0;
}
