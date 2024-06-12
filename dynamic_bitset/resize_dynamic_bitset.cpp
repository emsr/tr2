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
$HOME/bin/bin/g++ -std=c++11 -o resize_dynamic_bitset resize_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib64

$HOME/bin/bin/g++ -std=c++11 -m32 -o resize_dynamic_bitset32 resize_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib32
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
main(int argc, char** argv)
{
  //Check the boost capabilities
  boost::dynamic_bitset<>* pdb = new boost::dynamic_bitset<>;
  std::cout << *pdb << '\n';
  pdb->resize(10, 1);
  std::cout << *pdb << '\n';

  for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
      std::cout << pdb->operator [](i) << ' ';
  }
  std::cout << '\n';
  std::cout << *pdb << '\n';

  pdb->resize(15);
  for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
      std::cout << pdb->operator [](i) << ' ';
  }
  std::cout << '\n';
  std::cout << *pdb << '\n';

  pdb->flip();
  for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
      std::cout << pdb->operator [](i) << ' ';
  }
  std::cout << '\n';
  std::cout << *pdb << '\n';

  std::cout << pdb->count() << '\n';

  pdb->resize(20, 0);
  std::cout << *pdb << '\n';

  pdb->resize(25, 1);
  std::cout << *pdb << '\n';

  for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
      std::cout << pdb->operator [](i) << ' ';
  }
  std::cout << '\n';
  std::cout << *pdb << '\n';

  std::cout << pdb->count() << '\n';

  pdb->resize(75, true);

  for (auto i = 0; i != pdb->size(); ++i) {
      std::cout << pdb->operator [](i) << ' ';
  }
  std::cout << '\n';
  std::cout << *pdb << '\n';

  std::cout << pdb->count() << '\n';

  delete pdb;
  std::cout << '\n';


  //Check the TR2 dynamic_bitset capabilities
  std::tr2::dynamic_bitset<> pdb2{};
  std::cout << pdb2 << '\n';
  pdb2.resize(10, 1);
  std::cout << pdb2 << '\n';

  for (auto i = 0; i != pdb2.size(); ++i)
    std::cout << pdb2.operator [](i) << ' ';
  std::cout << '\n';
  std::cout << pdb2 << '\n';

  pdb2.resize(15);
  for (auto i = 0; i != pdb2.size(); ++i)
    std::cout << pdb2.operator [](i) << ' ';
  std::cout << '\n';
  std::cout << pdb2 << '\n';

  pdb2.flip();
  for (auto i = 0; i != pdb2.size(); ++i)
    std::cout << pdb2.operator [](i) << ' ';
  std::cout << '\n';
  std::cout << pdb2 << '\n';

  std::cout << pdb2.count() << '\n';

  pdb2.resize(20, false);
  std::cout << pdb2 << '\n';

  pdb2.resize(25, true);
  std::cout << pdb2 << '\n';

  for (auto i = 0; i != pdb2.size(); ++i)
    std::cout << pdb2.operator [](i) << ' ';
  std::cout << '\n';
  std::cout << pdb2 << '\n';

  std::cout << pdb2.count() << '\n';

  pdb2.resize(75, true);

  for (auto i = 0; i != pdb2.size(); ++i)
    std::cout << pdb2.operator [](i) << ' ';
  std::cout << '\n';
  std::cout << pdb2 << '\n';

  std::cout << pdb2.count() << '\n';

  return (EXIT_SUCCESS);
}
