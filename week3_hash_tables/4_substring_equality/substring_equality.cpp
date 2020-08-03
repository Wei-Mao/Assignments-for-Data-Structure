#include <iostream>
#include <random>
#include <cmath>

using namespace std;


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

class Solver {
	string s;
	long long m1, m2;
	long long  x;
	vector<long long> h1;
	vector<long long> h2;
	vector<long long> x_l_1;
	vector<long long> x_l_2;
public:	
	Solver(string s) : s(s) {	
		// initialization, precalculation
	  m1 = 1e9 + 7;
	  m2 = 1e9 + 9;
	  long long p = 1e9;
	  x = rand() % p + 1;
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
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0);

	string s;
	int q;
	cin >> s >> q;
	Solver solver(s);
	for (int i = 0; i < q; i++) {
		int a, b, l;
		cin >> a >> b >> l;
		cout << (solver.ask(a, b, l) ? "Yes\n" : "No\n");
	}
}
