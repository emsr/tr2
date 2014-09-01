/* 
 * File:   main.cpp
 * Author: kyle bentley
 *         kwbent@uab.edu
 *
 * Testing the bitset properties and so on...
 * 
 * Created on October 11, 2013, 2:32 PM
 */

#include <iostream>
#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <tr2/dynamic_bitset>

int main(int argc, char** argv)
{ 
    //Check the boost capabilities
    boost::dynamic_bitset<>* pdb = new boost::dynamic_bitset<>;
    pdb->resize(10, 1);
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
        std::cout << pdb->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    pdb->resize(15);    
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
        std::cout << pdb->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    pdb->flip();    
    for (boost::dynamic_bitset<>::size_type i{0}; i != pdb->size(); ++i) {
        std::cout << pdb->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    std::cout<< pdb->count() << '\n';    
    
    delete pdb;
    std::cout<<'\n';
    
    //Check the TR2 dynamic_bitset capabilities
    std::tr2::dynamic_bitset<>* pdb2 = new std::tr2::dynamic_bitset<>;    
    pdb2->resize(10, 1);
   
    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2->size(); ++i) {
        std::cout << pdb2->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    pdb2->resize(15);    
    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2->size(); ++i) {
        std::cout << pdb2->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    pdb2->flip();    
    for (std::tr2::dynamic_bitset<>::size_type i{0}; i != pdb2->size(); ++i) {
        std::cout << pdb2->operator [](i) << ' ';
    }
    std::cout<<'\n';
    
    std::cout<< pdb2->count() << '\n';    
    
    delete pdb2;   
    
    return (EXIT_SUCCESS);
}

