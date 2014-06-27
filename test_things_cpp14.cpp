
// /home/ed/bin/bin/g++ -std=c++14 -o test_things_cpp14 test_things_cpp14.cpp > /dev/null

// /home/ed/bin/bin/g++ -std=c++14 -pedantic -o test_things_cpp14 test_things_cpp14.cpp > /dev/null

// /home/ed/bin/bin/g++ -std=c++14 -ansi -o test_things_cpp14 test_things_cpp14.cpp > /dev/null

auto
fun(int i)
-> decltype(i + 5)
{ return i + 5; }

[[deprecated]]
int
bar(int i)
{ return i + 4; }

int
main()
{
  int bin = 0b010101110;
  int n = 5;
  double arr[n];

  int x = 4;
  auto y = [&r = x, x = x + 1]()
           -> int
           {
             r += 2;
             return x + 2;
           }();  // Updates ::x to 6, and initializes y to 7.

  auto glambda = [](auto a, auto&& b) { return  a < b; };
  bool b = glambda(3, 3.14);

  bar(n);

  int q = 123'456'789;
}
