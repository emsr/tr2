/*
g++ -g -DLOCAL_TR2 -o right_shift right_shift.cpp
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
    std::tr2::dynamic_bitset<> test(128);
    for (int i = 64; i < 128; i++) test[i] = 1;
    std::cout << test << "\n";
    std::cout << (test >> 65) << "\n\n";

    boost::dynamic_bitset<> test2(128);
    for (int i = 64; i < 128; i++) test2[i] = 1;
    std::cout << test2 << "\n";
    std::cout << (test2 >> 65) << "\n\n";
    
    std::bitset<128> test3;
    for (int i = 64; i < 128; i++) test3[i] = 1;
    std::cout << test3 << "\n";
    std::cout << (test3 >> 65) << "\n\n";
}
