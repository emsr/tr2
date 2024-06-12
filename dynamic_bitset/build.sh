#! /bin/bash

g++ -std=c++17 -DLOCAL_TR2 -g -o test_dynamic_bitset test_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib64
./test_dynamic_bitset < test.in > test.out

g++ -std=c++17 -DLOCAL_TR2 -g -m32 -o test_dynamic_bitset32 test_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib32
./test_dynamic_bitset32 < test.in > test32.out

# Test an installed gcc...
g++ -std=c++17 -g -o test_dynamic_bitset_std test_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib64
./test_dynamic_bitset_std < test.in > test_std.out

g++ -std=c++11 -o resize_dynamic_bitset resize_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib64

g++ -std=c++11 -m32 -o resize_dynamic_bitset32 resize_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib32

g++ -g -DLOCAL_TR2 -o main2 main2.cpp
./main2 > main2.out

g++ -g -DLOCAL_TR2 -o main1 main.cpp
./main1 > main1.out

g++ -g -DLOCAL_TR2 -o right_shift right_shift.cpp
./right_shift > right_shift.out

g++ -g -o right_shift_gcc right_shift.cpp
./right_shift_gcc > right_shift_gcc.out

g++ -g -DLOCAL_TR2 -o left_shift left_shift.cpp
./left_shift > left_shift.out

g++ -g -o left_shift_gcc left_shift.cpp
./left_shift_gcc > left_shift_gcc.out

gcc_dev=/home/ed/gcc
test_inc=$gcc_dev/libstdc++-v3/testsuite/util

if [ -d $gcc_dev ]; then
  g++ -g -DLOCAL_TR2 -I$test_inc -o cmp cmp.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o cons cons.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o copy copy.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o move move.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o pr58729 pr58729.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o pr87784 pr87784.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o pr92059 pr92059.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o pr115399 pr115399.cc
  g++ -g -DLOCAL_TR2 -I$test_inc -o string string.cc
else
  echo "No gcc development environmnt; libstdc++-v3 tests not built."
fi
