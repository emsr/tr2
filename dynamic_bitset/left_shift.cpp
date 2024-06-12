/*
g++ -g -DLOCAL_TR2 -o left_shift left_shift.cpp
*/

#include <iostream>
#include <bitset>
#ifdef LOCAL_TR2
#  include "dynamic_bitset"
#else
#  include <tr2/dynamic_bitset>
#endif
#include <boost/dynamic_bitset.hpp>

int
main()
{
    std::tr2::dynamic_bitset<> test(65);
    for (int i = 0; i < 64; i++) test[i] = 1;
    std::cout << test << "\n";
    std::cout << (test << 64) << "\n\n";

    boost::dynamic_bitset<> test2(65);
    for (int i = 0; i < 64; i++) test2[i] = 1;
    std::cout << test2 << "\n";
    std::cout << (test2 << 64) << "\n\n";
    
    std::bitset<65> test3;
    for (int i = 0; i < 64; i++) test3[i] = 1;
    std::cout << test3 << "\n";
    std::cout << (test3 << 64) << "\n\n";
}
