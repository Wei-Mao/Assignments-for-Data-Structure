#include <iostream>
#include <random>
using namespace std;
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

int main()
{
  cout << uniform_int_generator(0, 10) << endl;
}
