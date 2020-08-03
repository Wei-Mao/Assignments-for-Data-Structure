#include <iostream>
#include <string>
#include <vector>

using namespace std;
using std::string;
typedef unsigned long long ull;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

bool are_equal(string T, string P)
{
  if(T.size() != P.size())
  {
    return false;
  }
  for(long long  i(0); i < T.size(); i++)
  {
    if(T[i] != P[i])
    {
      return false;
    }
  }
  return true;
}

ull poly_hash(const string &s, int p, int x)
{
  unsigned long long hash = 0;
  for(long long i(s.size() - 1); i >= 0; i--)
  {
    hash = (hash * x + s[i]) % p;
    if(hash < 0)
    {
      hash += p;
    }
  }
  return hash;
}

vector<ull> precompute_hashes(string T, int len_pattern, int p, int x)
{
  vector<ull> H(T.size() - len_pattern + 1);
  string S = T.substr(T.size() - len_pattern, len_pattern); 
  // compute the initial value outside of the loop
  H[T.size() - len_pattern] = poly_hash(S, p, x);
  ull y(1);
  for(long long i(1); i <= len_pattern; i ++)
  {
    y = (y * x) % p;
  }

  for(long long i(T.size() - len_pattern - 1); i >=0; i--)
  {
    H[i] = (x * H[i+1] + T[i] - y * T[i + len_pattern]) % p;
    if(H[i] < 0)
    {
      H[i] += p;
    }
  }

  return H;
}

void print_occurrences(const std::vector<int>& output) {
    for (long long i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

std::vector<int> get_occurrences(const Data& input) {
    const string& s = input.pattern, t = input.text;
    std::vector<int> ans;
    // Need to compare pattern with all substrings of T with the same length as pattern
    // Improvement: use hashing to quickly compare P with substrings of T
    // Instead of O(|T||P|), it will reduce to O(|T|)  on the grounds that we compute hash values outside of the loop.
    // Moving expensive operations out of the loop is a common practice to optimize the algorithm. 
    for (long long i = 0; i + s.size() <= t.size(); ++i)
        if (t.substr(i, s.size()) == s) // O(length of s)
            ans.push_back(i);
    return ans;
}

std::vector<int> get_occurrences_hash(const Data& input)
{
  std::vector<int> ans;
  const string & s = input.pattern, t = input.text;
  int p = 1000000007;
  int x = rand() % (p-1) + 1;
  ull pattern_hash = poly_hash(s, p, x);
  vector<ull> H = precompute_hashes(t, s.size(), p, x);
  for(long long i(0); i <= t.size() - s.size(); i++)
  {
    if(H[i] != pattern_hash)
    {
      continue;  // break out of the current iteration
    }

    // hash values match
    if(are_equal(t.substr(i, s.size()), s))
    {
      ans.push_back(i);
    }
  }
  return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences_hash(read_input()));
    return 0;
}
