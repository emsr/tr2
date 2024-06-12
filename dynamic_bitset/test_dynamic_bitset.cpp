
/*
$HOME/bin/bin/g++ -std=c++17 -DLOCAL_TR2 -g -o test_dynamic_bitset test_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib64
./test_dynamic_bitset < test.in > test.out

$HOME/bin/bin/g++ -std=c++17 -DLOCAL_TR2 -g -m32 -o test_dynamic_bitset32 test_dynamic_bitset.cpp -Wl,-rpath=$HOME/bin/lib32
./test_dynamic_bitset32 < test.in > test32.out

// Test an installed gcc...
$HOME/bin/bin/g++ -std=c++17 -g -o test_dynamic_bitset_std test_dynamic_bitset_std.cpp -Wl,-rpath=$HOME/bin/lib64
./test_dynamic_bitset_std < test.in > test_std.out
*/

#ifdef LOCAL_TR2
#  include "dynamic_bitset"
#else
#  include <tr2/dynamic_bitset>
#endif
#include <iostream>
#include <stdexcept>
#include <bitset>

//  The output is:
//
//  11001
//  10011
void
example1()
{
  std::tr2::dynamic_bitset<> x(5); // all 0's by default
  x[0] = 1;
  x[1] = 1;
  x[4] = 1;
  for (std::tr2::dynamic_bitset<>::size_type i = 0; i < x.size(); ++i)
    std::cout << x[i];
  std::cout << '\n';
  std::cout << x << '\n';

  return;
}

//  The output is:
//
//  bits(0) = 00
//  bits(1) = 01
//  bits(2) = 10
//  bits(3) = 11
void
example2()
{
  const std::tr2::dynamic_bitset<> b0(2, 0ul);
  std::cout << "bits(0) = " << b0 << '\n';

  const std::tr2::dynamic_bitset<> b1(2, 1ul);
  std::cout << "bits(1) = " << b1 << '\n';

  const std::tr2::dynamic_bitset<> b2(2, 2ul);
  std::cout << "bits(2) = " << b2 << '\n';

  const std::tr2::dynamic_bitset<> b3(2, 3ul);
  std::cout << "bits(3) = " << b3 << '\n';

  return;
}

void
example3()
{
  std::tr2::dynamic_bitset<> mask(12, 2730ul);
  std::cout << "mask     = " << mask << '\n';

  std::tr2::dynamic_bitset<> x;
  std::cout << "x.size() = " << x.size() << '\n';

  std::cout << "Enter a bitset in binary: x = " << std::flush;
  if (std::cin >> x)
  {
    const std::size_t sz = x.size();
    std::cout << '\n';
    std::cout << "Input number:             " << x << '\n';
    std::cout << "x.size() is now:          " << sz << '\n';

    bool fits_in_ulong = true;
    unsigned long ul = 0;
    try
    {
      ul = x.to_ulong();
    }
    catch (std::overflow_error &)
    {
      fits_in_ulong = false;
    }

    std::cout << '\n';
    std::cout << "As unsigned long:         ";
    if (fits_in_ulong)
    {
      std::cout << ul;
    }
    else
    {
      std::cout << "(unsigned long overflow exception)";
    }

    bool fits_in_ullong = true;
    unsigned long long ull = 0;
    try
    {
      ull = x.to_ullong();
    }
    catch (std::overflow_error &)
    {
      fits_in_ullong = false;
    }

    std::cout << '\n';
    std::cout << "As unsigned long long:    ";
    if (fits_in_ullong)
    {
      std::cout << ull;
    }
    else
    {
      std::cout << "(unsigned long long overflow exception)";
    }

    std::cout << '\n';

    mask.resize(sz);

    std::cout << "Mask (possibly resized):  " << mask << '\n';

    std::cout << "And with mask:            " << (x & mask) << '\n';
    std::cout << "Or with mask:             " << (x | mask) << '\n';
    std::cout << "Shifted left by 5:        " << (x << 5) << '\n';
    std::cout << "Shifted right by 5:       " << (x >> 5) << '\n';
  }

  return;
}

int
main()
{
  using std::cout;

  cout << '\n';
  unsigned long i = 0;
  cout << "i  = " << i << '\n';
  cout << "~i = " << ~i << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> db(64, 0xffffffffUL);
  cout << "db.num_blocks() = " << db.num_blocks() << '\n';
  cout << "db.size()       = " << db.size() << '\n';
  cout << "db              = " << db << '\n';
  db.set();
  cout << "db.set()        = " << db << '\n';
  db.reset();
  cout << "db.reset()      = " << db << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> dbs(32, 0xffffffffUL);
  cout << "dbs.num_blocks() = " << dbs.num_blocks() << '\n';
  cout << "dbs.size()       = " << dbs.size() << '\n';
  cout << "dbs              = " << dbs << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<unsigned short> dbshort(16, 0xffff);
  cout << "dbshort.num_blocks() = " << dbshort.num_blocks() << '\n';
  cout << "dbshort.size()       = " << dbshort.size() << '\n';
  cout << "dbshort              = " << dbshort << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<unsigned char> dbs2(8, 0x0a);
  cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << '\n';
  cout << "dbs2.size()        = " << dbs2.size() << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "~dbs2              = " << ~dbs2 << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "(dbs2 << 1)        = " << (dbs2 << 1) << '\n';
  cout << "(dbs2 << 2)        = " << (dbs2 << 2) << '\n';
  cout << "(dbs2 << 3)        = " << (dbs2 << 3) << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "(dbs2 >> 1)        = " << (dbs2 >> 1) << '\n';
  cout << "(dbs2 >> 2)        = " << (dbs2 >> 2) << '\n';
  cout << "(dbs2 >> 3)        = " << (dbs2 >> 3) << '\n';

  cout << '\n';
  cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << '\n';
  cout << "dbs2.size()        = " << dbs2.size() << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "(dbs2 >>= 1)       = " << (dbs2 >>= 1) << '\n';
  cout << "(dbs2 >>= 2)       = " << (dbs2 >>= 2) << '\n';
  cout << "(dbs2 <<= 2)       = " << (dbs2 <<= 2) << '\n';
  cout << "(dbs2 <<= 1)       = " << (dbs2 <<= 1) << '\n';
  cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << '\n';
  cout << "dbs2.size()        = " << dbs2.size() << '\n';
  cout << "dbs2               = " << dbs2 << '\n';

  cout << '\n';
  unsigned char b2 = 0x0a;
  dbs2.append(b2);
  cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << '\n';
  cout << "dbs2.size()        = " << dbs2.size() << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "dbs2.to_ulong()    = " << dbs2.to_ulong() << '\n';
  cout << "dbs2.to_ullong()   = " << dbs2.to_ullong() << '\n';

  unsigned char vb2[2] = {0xff, 0xff};
  dbs2.append(vb2, vb2 + 2);
  cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << '\n';
  cout << "dbs2.size()        = " << dbs2.size() << '\n';
  cout << "dbs2               = " << dbs2 << '\n';
  cout << "dbs2.to_ulong()    = " << dbs2.to_ulong() << '\n';
  cout << "dbs2.to_ullong()   = " << dbs2.to_ullong() << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> set("01010101010101010101010101010101");
  cout << "set = " << set << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> sets(std::string("01010101010101010101010101010101"));
  cout << "sets = " << sets << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> pb;
  for(size_t b = 0; b < 2048; ++b)
    pb.push_back(bool(b%2));
  cout << "pb = " << pb << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> ilb({0xfacebeefUL, 0xdeadbabeUL});
  cout << "ilb = " << ilb << '\n';
  ilb.append({0xfacebeefUL, 0xdeadbabeUL});
  cout << "ilb = " << ilb << '\n';

  cout << '\n';
  cout << "example1" << '\n';
  example1();

  cout << '\n';
  cout << "example2" << '\n';
  example2();

  cout << '\n';
  cout << "example3" << '\n';
  example3();

  cout << '\n';
  std::tr2::dynamic_bitset<> a = ilb;
  cout << "a = " << a << '\n';

  cout << '\n';
  std::tr2::dynamic_bitset<> b(64, 0xccccccccUL);
  cout << "b = " << b << '\n';
  cout << "b & ilb = " << (b & ilb) << '\n';

  std::tr2::dynamic_bitset<> testc({0xccccccccUL, 0xccccccccUL, 0xccccccccUL, 0xccccccccUL});
  testc &= ilb;
  cout << "testc = " << testc << '\n';

  std::tr2::dynamic_bitset<> testm({0xccccccccUL, 0xccccccccUL, 0xccccccccUL, 0xccccccccUL});
  testm &= std::tr2::dynamic_bitset<>({0xfacebeefUL, 0xdeadbabeUL,0xfacebeefUL, 0xdeadbabeUL});
  cout << "testm = " << testm << '\n';
}
