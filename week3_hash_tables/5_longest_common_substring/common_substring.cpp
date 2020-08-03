#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
using namespace std;
using std::string;
typedef unsigned long long ull;
int uniform_int_generator(int from, int to)
{
  /* Generate random numbers uniformly over an range from (from) to (to)
  */
  const int range_from  = from;
  const int range_to    = to;
  std::random_device                  rand_dev;
  std::mt19937                        generator(rand_dev());
  std::uniform_int_distribution<int>  distr(range_from, range_to);

  return distr(generator);
}

struct Data {
    string pattern, text;
};

vector<long long> x_power_l_mod_m(long long x, long long l, long long m)
{
  x = x % m;
  vector<long long> result(l);
  result[0] = x;
  for(long long i(1); i < l; i++)
  {
    result[i] = (result[i-1] * x) % m;
  }
  return result;
}

ull poly_hash(const string &s, long long p, long long x)
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

vector<ull> precompute_hashes(string T, long long len_pattern, long long p, long long x, vector<long long> & x_l)
{
  vector<ull> H(T.size() - len_pattern + 1);
  string S = T.substr(T.size() - len_pattern, len_pattern);
  // compute the initial value outside of the loop
  H[T.size() - len_pattern] = poly_hash(S, p, x);


  for(long long i(T.size() - len_pattern - 1); i >=0; i--)
  {
    H[i] = (x * H[i+1] + T[i] - x_l[len_pattern-1] * T[i + len_pattern]) % p;
    if(H[i] < 0)
    {
      H[i] += p;
    }
  }

  return H;
}


std::vector<long long> get_occurrences_hash(const Data& input, const vector<ull> & H1, const vector<ull> &H2, long long m1, long long m2, long long x)
{
  std::vector<long long> ans;  // empty
  const string & s = input.pattern, t = input.text;
  /* long long m1 = 1e9 +7;  //p1 */
  /* long long m2 = 1e9 +9; */
  /* long long p = 1e9; */
  /* long long x = rand() % (p) + 1; */
  ull pattern_hash_1 = poly_hash(s, m1, x);
  ull pattern_hash_2 = poly_hash(s, m2, x);
  /* vector<ull> H1 = precompute_hashes(t, s.size(), m1, x); */
  /* vector<ull> H2 = precompute_hashes(t, s.size(), m2, x); */
  for(long long i(0); i <= t.size() - s.size(); i++)
  {
    if((H1[i] == pattern_hash_1) && (H2[i] == pattern_hash_2))
    {
      ans.push_back(i);  // starting position  in text
      break;
    }
    else
    {
      continue;  // break out of the current iteration
    }
  }
  return ans;   // staring position of the matching substring. No match if ans is empty.
}

struct Answer {
	size_t i, j, len;
};

Answer solve_naive(const string &s, const string &t) {
      // O(|S||T||max_len|^2) quite slow
	Answer ans = {0, 0, 0};
	for (size_t i = 0; i < s.size(); i++)
		for (size_t j = 0; j < t.size(); j++)
			for (size_t len = 0; i + len <= s.size() && j + len <= t.size(); len++)
				if (len > ans.len && s.substr(i, len) == t.substr(j, len))
					ans = {i, j, len};
	return ans;
}

unordered_map<long long, long long> hashvalue2hashtable(vector<ull> hash_value, long long m)
{
  unordered_map<long long, long long> result;
  for(long long i(0); i < hash_value.size(); i++)
  {
    result.insert(make_pair(hash_value[i], i));
  }
  return result;
}
Answer solve_fast(string &s, string &t)
{
  Answer ans = {0, 0, 0};
  long long m1 = 1e9 + 4897;  //p1
  long long m2 = 1e9 + 1;
  /* long long x1 = rand() % (m1 - 1) + 1; */  // cause error in that rand() does not generate strictly uniformly distributed random integers.
  long long x1 = uniform_int_generator(1, m1 - 1);
  long long x2 = uniform_int_generator(1, m2 - 1);
  long long L = s.size() < t.size() ? s.size() : t.size();  // min length of the two.
  long long l = 0;
  long long r = L;
  vector<long long> x_l_1 = x_power_l_mod_m(x1, L, m1);
  vector<long long> x_l_2 = x_power_l_mod_m(x2, L, m2); 
  Data input;
  input.text = s;
  while(l <= r)
  {
    bool is_success = false;
    vector<long long> result;
    long long mid = l + (r - l) / 2;
    vector<ull> H1_s = precompute_hashes(s, mid, m1, x1, x_l_1); 
    vector<ull> H2_s = precompute_hashes(s, mid, m2, x2, x_l_2);
    vector<ull> H1_t = precompute_hashes(t, mid, m1, x1, x_l_1); 
    vector<ull> H2_t = precompute_hashes(t, mid, m2, x2, x_l_2);

    unordered_map<long long, long long> H1_s_table = hashvalue2hashtable(H1_s, m1);
    unordered_map<long long, long long> H2_s_table = hashvalue2hashtable(H2_s, m2);

    for(long long j(0); j <= t.size() - mid; j++)
    {
      if((H1_s_table.find(H1_t[j]) != H1_s_table.end()) && (H2_s_table.find(H2_t[j]) != H2_s_table.end()))
      {
	is_success = true;
	ans.j = j;
	ans.i = H1_s_table[H1_t[j]];
	ans.len = mid;
	break;
      }
    }

    if(is_success)      // check for the common substring of length mid.
    {
      l = mid + 1;      // check for larger length.
    }
    else
    {
      r = mid - 1;
    }
  }

  return ans;
}

int main() {
	/* ios_base::sync_with_stdio(false), cin.tie(0); */
	string s, t;
	while (cin >> s >> t) {
		Answer ans = solve_fast(s, t);
		cout << ans.i << " " << ans.j << " " << ans.len << "\n";
	}
}
