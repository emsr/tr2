
//  /home/ed/bin/bin/g++ -std=c++1y -DVECTOR -o time_vector time_cpp.cpp > /dev/null

//  /home/ed/bin/bin/g++ -std=c++1y -UVECTOR -o time_dynarray time_cpp.cpp > /dev/null

//  LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./time_vector

//  LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./time_dynarray

//  http://stackoverflow.com/questions/5142366/how-fast-is-d-compared-to-c

#include <iostream>
#include <random>
#include <chrono>
#ifdef VECTOR
#  include <vector>
#else
#  include "dynarray"
#endif

typedef std::chrono::duration<long, std::ratio<1, 1000>> millisecs;
template<typename _T>
  long
  time_since(std::chrono::time_point<_T>& time)
  {
    long tm = std::chrono::duration_cast<millisecs>(std::chrono::system_clock::now() - time).count();
    time = std::chrono::system_clock::now();
    return tm;
  }

const long N = 20000;
const int size = 10;

typedef int value_type;
typedef long long result_type;
#ifdef VECTOR
typedef std::vector<value_type> vector_t;
#else
typedef std::dynarray<value_type> vector_t;
#endif
typedef typename vector_t::size_type size_type;

inline value_type
scalar_product(const vector_t& x, const vector_t& y)
{
  value_type res = 0;
  size_type size = x.size();
  for (size_type i = 0; i < size; ++i)
    res += x[i] * y[i];
  return res;
}

int
main()
{
  auto tm_before = std::chrono::system_clock::now();

  // 1. allocate and fill randomly many short vectors
  std::vector<vector_t> xs(N, vector_t(size));
  std::cerr << "allocation: " << time_since(tm_before) << " ms" << std::endl;

  std::mt19937 rnd_engine;
  std::uniform_int_distribution<value_type> runif_gen(-1000, 1000);
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < size; ++j)
      xs[i][j] = runif_gen(rnd_engine);
  std::cerr << "random generation: " << time_since(tm_before) << " ms" << std::endl;

  // 2. compute all pairwise scalar products:
  time_since(tm_before);
  result_type avg = 0;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      avg += scalar_product(xs[i], xs[j]);
  avg /= N*N;
  auto time = time_since(tm_before);
  std::cout << "result: " << avg << std::endl;
  std::cout << "time: " << time << " ms" << std::endl;
}
