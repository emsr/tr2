struct __attribute__((deprecated)) Old1 {};

Old1 old1 __attribute__((deprecated));

struct [[deprecated("deprecated type Old2")]] Old2 {};

Old2 old2 [[deprecated("deprecated global old2")]]; // ##

int
main()
{
 //old1;
 //Old1 newold1 [[gnu::unused]];
 //old2;
 //Old2 newold2 [[gnu::unused]];
}

// /home/ed/bin/bin/g++ -std=c++14 -Wall -Wextra -pedantic -o test_pr61754 test_pr61754.cpp

// /home/ed/bin/bin/g++ -std=c++14 -o test_pr61754 test_pr61754.cpp

// /home/ed/bin/bin/g++ -std=c++14 -Wdeprecated-declarations -o test_pr61754 test_pr61754.cpp

