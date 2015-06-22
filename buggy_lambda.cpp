// $HOME/bin/bin/g++ -std=c++14 -o buggy_lambda buggy_lambda.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./buggy_lambda

#include <iostream>
#include <set>
#include <stdexcept>
#include <initializer_list>


int main(int argc, char ** argv)
{
    enum Switch {
        Switch_1,
        Switch_2,
        Switch_3,
        Switch_XXXX,
    };

    int foo_1 = 1;
    int foo_2 = 2;
    int foo_3 = 3;
    int foo_4 = 4;
    int foo_5 = 5;
    int foo_6 = 6;
    int foo_7 = 7;

    auto get_foos = [=] (Switch ss) -> std::initializer_list<int> {
        switch (ss) {
            case Switch_1:
                return {foo_1, foo_2, foo_3};
            case Switch_2:
                return {foo_4, foo_5};
            case Switch_3:
                return {foo_6, foo_7};
            default:
                throw std::logic_error("invalid switch");
        }
    };

    std::set<int> foos = get_foos(Switch_1);
    for (auto && foo : foos) {
        std::cout << foo << " ";
    }
    std::cout << std::endl;
    return 0;
}
