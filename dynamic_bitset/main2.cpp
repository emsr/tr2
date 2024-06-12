/*
 * File:   main.cpp
 * Author: kyle bentley
 *         kwbent@uab.edu
 *
 * Testing the bitset properties and so on...
 *
 * Created on October 11, 2013, 2:32 PM
 */

/*
g++ -g -DLOCAL_TR2 -o main2 main2.cpp
*/

#include <iostream>
#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#ifdef LOCAL_TR2
#  include "dynamic_bitset"
#else
#  include <tr2/dynamic_bitset>
#endif

int
main()
{
    using std::cout;

    //Check the boost capabilities
    boost::dynamic_bitset<> pdb;
    pdb.resize(10, 1);
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb.size(); ++i) {
        cout << pdb.operator [](i) << ' ';
    }
    cout<<'\n';

    pdb.resize(15);
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb.size(); ++i) {
        cout << pdb.operator [](i) << ' ';
    }
    cout<<'\n';

    pdb.flip();
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb.size(); ++i) {
        cout << pdb.operator [](i) << ' ';
    }
    cout<<'\n';

    cout<< pdb.count() << '\n';

    cout<<'\n';

    //Check the TR2 dynamic_bitset capabilities
    std::tr2::dynamic_bitset<> pdb2;
    pdb2.resize(10, 1);

    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2.size(); ++i) {
        cout << pdb2.operator [](i) << ' ';
    }
    cout<<'\n';

    pdb2.resize(15);
    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2.size(); ++i) {
        cout << pdb2.operator [](i) << ' ';
    }
    cout<<'\n';

    pdb2.flip();
    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2.size(); ++i) {
        cout << pdb2.operator [](i) << ' ';
    }
    cout<<'\n';

    cout<< pdb2.count() << '\n';

    return 0;
}

