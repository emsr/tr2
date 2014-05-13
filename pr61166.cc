#include <chrono>

using namespace std;
using namespace std::chrono;

int
main()
{
  // std::numeric_limits<unsigned>::max() == 4294967295
  cout << 429496729510 << endl;
  cout << (429496729510h).count() << endl;
  return 0;
}

//And the result is:
//429496729510 
//926648584
