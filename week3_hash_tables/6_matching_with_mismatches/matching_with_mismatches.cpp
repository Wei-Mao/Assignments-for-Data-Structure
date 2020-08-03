#include <iostream>
#include <vector>
#include <random>

using namespace std;



class String_Equality {
	string s;
	long long m1, m2;
	long long  x;
	vector<long long> h1;
	vector<long long> h2;
	vector<long long> x_l_1;
	vector<long long> x_l_2;
public:
	String_Equality(string s) : s(s) {
		// initialization, precalculation
	  m1 = 1e9 + 7;
	  m2 = 1e9 + 9;
	  int p = 1e9;
	  /* x = rand() % p + 1; */
	  x = uniform_int_generator(1, p);
	  h1.resize(s.size() + 1);
	  h2.resize(s.size() + 1);
	  x_l_1.resize(s.size());  // 0-based x_l_1[i] = x^(i+1) % m1
	  x_l_2.resize(s.size());
	  x_l_1 = x_power_l_mod_m(x, s.size(), m1);
	  x_l_2 = x_power_l_mod_m(x, s.size(), m2);
	  h1[0] = 0;
	  h2[0] = 0;
	  for(long long i(1); i <=s.size(); i++)
	  {
	    h1[i] = (x * h1[i-1] + s[i-1]) % m1;
	    h2[i] = (x * h2[i-1] + s[i-1]) % m2;
	    if(h1[i] < 0)
	    {
	      h1[i] += m1;
	    }
	    if(h2[i] < 0)
	    {
	      h2[i] += m2;
	    }
	  }
	}

	bool ask(int a, int b, int l) {
	  long long Ha1 = h1[a+l] - (x_l_1[l-1] * h1[a]) % m1;
	  long long Ha2 = h2[a+l] - (x_l_2[l-1] * h2[a]) % m2;
	  long long Hb1 = h1[b+l] - (x_l_1[l-1] * h1[b]) % m1;
	  long long Hb2 = h2[b+l] - (x_l_2[l-1] * h2[b]) % m2;
	  if(Ha1 < 0) { Ha1 += m1;}
	  if(Ha2 < 0) { Ha2 += m2;}
	  if(Hb1 < 0) { Hb1 += m1;}
	  if(Hb2 < 0) { Hb2 += m2;}
	  if((Ha1 == Hb1) && (Ha2 == Hb2))
	  {
	    return true;
	  }
	  else
	  {
	    return false;
	  }
	}

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
};

vector<int> solve(int k, const string &text, const string &pattern) 
{
	vector<int> pos;
	long long text_size = text.size();
	long long pattern_size = pattern.size();
	String_Equality str_equ(text + pattern);
	for(int i(0); i < (text_size - pattern_size); i++)
	{
	  int count_mis(0);

	  int l(i);
	  int r(i+pattern_size-1);

	  // find k mismatches
	  for(int cnt(0); cnt <=k; cnt++)
	  {
	    if(!str_equ.ask(l, text_size + (l - i), r - l + 1))
	    {
	      count_mis ++;
	      while(l<r) 
	      {
		int mid = l + (r - l) / 2;
		int sub_win_size = mid - l +1;
		if(!str_equ.ask(l, text_size + (l - i), sub_win_size))
		{
		  r = mid;  // to right half.
		}
		else
		{
		  l = mid + 1;
		}
	      }
	      l = l + 1;
	      cout << "l" << l << endl;
	      r = i + pattern_size -1;
	    }
	  }

	  if(count_mis <= k)
	  {
	    pos.push_back(i);
	  }
	}

	return pos;
}

int main() {
	/* ios_base::sync_with_stdio(false), cin.tie(0); */
	int k;
	string t, p;
	while (cin >> k >> t >> p) {
		auto a = solve(k, t, p);
		cout << a.size();
		for (int x : a)
			cout << " " << x;
		cout << "\n";
	}
}
