
// /home/ed/bin/bin/g++ -std=c++0x -g -o test_dynamic_bitset test_dynamic_bitset.cpp

// /home/ed/bin/bin/g++ -std=c++0x -g -m32 -o test_dynamic_bitset test_dynamic_bitset.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_dynamic_bitset

#include "dynamic_bitset"
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
  std::cout << std::endl;
  std::cout << x << std::endl;

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
  std::cout << "bits(0) = " << b0 << std::endl;

  const std::tr2::dynamic_bitset<> b1(2, 1ul);
  std::cout << "bits(1) = " << b1 << std::endl;

  const std::tr2::dynamic_bitset<> b2(2, 2ul);
  std::cout << "bits(2) = " << b2 << std::endl;

  const std::tr2::dynamic_bitset<> b3(2, 3ul);
  std::cout << "bits(3) = " << b3 << std::endl;

  return;
}

void
example3()
{
  std::tr2::dynamic_bitset<> mask(12, 2730ul);
  std::cout << "mask     = " << mask << std::endl;

  std::tr2::dynamic_bitset<> x;
  std::cout << "x.size() = " << x.size() << std::endl;

  std::cout << "Enter a bitset in binary: x = " << std::flush;
  if (std::cin >> x)
  {
    const std::size_t sz = x.size();
    std::cout << std::endl;
    std::cout << "Input number:             " << x << std::endl;
    std::cout << "x.size() is now:          " << sz << std::endl;

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

    std::cout << std::endl;
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

    std::cout << std::endl;
    std::cout << "As unsigned long long:    ";
    if (fits_in_ullong)
    {
      std::cout << ull;
    }
    else
    {
      std::cout << "(unsigned long long overflow exception)";
    }

    std::cout << std::endl;

    mask.resize(sz);

    std::cout << "Mask (possibly resized):  " << mask << std::endl;

    std::cout << "And with mask:            " << (x & mask) << std::endl;
    std::cout << "Or with mask:             " << (x | mask) << std::endl;
    std::cout << "Shifted left by 1:        " << (x << 1) << std::endl;
    std::cout << "Shifted right by 1:       " << (x >> 1) << std::endl;
  }

  return;
}

int
main()
{
  std::cout << std::endl;
  unsigned long i = 0;
  std::cout << "i  = " << i << std::endl;
  std::cout << "~i = " << ~i << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> db(64, 0xffffffffUL);
  std::cout << "db.num_blocks() = " << db.num_blocks() << std::endl;
  std::cout << "db.size()       = " << db.size() << std::endl;
  std::cout << "db              = " << db << std::endl;
  db.set();
  std::cout << "db.set()        = " << db << std::endl;
  db.reset();
  std::cout << "db.reset()      = " << db << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> dbs(32, 0xffffffffUL);
  std::cout << "dbs.num_blocks() = " << dbs.num_blocks() << std::endl;
  std::cout << "dbs.size()       = " << dbs.size() << std::endl;
  std::cout << "dbs              = " << dbs << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<unsigned short> dbshort(16, 0xffff);
  std::cout << "dbshort.num_blocks() = " << dbshort.num_blocks() << std::endl;
  std::cout << "dbshort.size()       = " << dbshort.size() << std::endl;
  std::cout << "dbshort              = " << dbshort << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<unsigned char> dbs2(8, 0x0a);
  std::cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << std::endl;
  std::cout << "dbs2.size()        = " << dbs2.size() << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "~dbs2              = " << ~dbs2 << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "(dbs2 << 1)        = " << (dbs2 << 1) << std::endl;
  std::cout << "(dbs2 << 2)        = " << (dbs2 << 2) << std::endl;
  std::cout << "(dbs2 << 3)        = " << (dbs2 << 3) << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "(dbs2 >> 1)        = " << (dbs2 >> 1) << std::endl;
  std::cout << "(dbs2 >> 2)        = " << (dbs2 >> 2) << std::endl;
  std::cout << "(dbs2 >> 3)        = " << (dbs2 >> 3) << std::endl;

  std::cout << std::endl;
  std::cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << std::endl;
  std::cout << "dbs2.size()        = " << dbs2.size() << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "(dbs2 >>= 1)       = " << (dbs2 >>= 1) << std::endl;
  std::cout << "(dbs2 >>= 2)       = " << (dbs2 >>= 2) << std::endl;
  std::cout << "(dbs2 <<= 2)       = " << (dbs2 <<= 2) << std::endl;
  std::cout << "(dbs2 <<= 1)       = " << (dbs2 <<= 1) << std::endl;
  std::cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << std::endl;
  std::cout << "dbs2.size()        = " << dbs2.size() << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;

  std::cout << std::endl;
  unsigned char b2 = 0x0a;
  dbs2.append(b2);
  std::cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << std::endl;
  std::cout << "dbs2.size()        = " << dbs2.size() << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "dbs2.to_ulong()    = " << dbs2.to_ulong() << std::endl;
  std::cout << "dbs2.to_ullong()   = " << dbs2.to_ullong() << std::endl;

  unsigned char vb2[2] = {0xff, 0xff};
  dbs2.append(vb2, vb2 + 2);
  std::cout << "dbs2.num_blocks()  = " << dbs2.num_blocks() << std::endl;
  std::cout << "dbs2.size()        = " << dbs2.size() << std::endl;
  std::cout << "dbs2               = " << dbs2 << std::endl;
  std::cout << "dbs2.to_ulong()    = " << dbs2.to_ulong() << std::endl;
  std::cout << "dbs2.to_ullong()   = " << dbs2.to_ullong() << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> set("01010101010101010101010101010101");
  std::cout << "set = " << set << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> sets(std::string("01010101010101010101010101010101"));
  std::cout << "sets = " << sets << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> pb;
  for(size_t b = 0; b < 2048; ++b)
    pb.push_back(bool(b%2));
  std::cout << "pb = " << pb << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> ilb({0xfacebeefUL, 0xdeadbabeUL});
  std::cout << "ilb = " << ilb << std::endl;
  ilb.append({0xfacebeefUL, 0xdeadbabeUL});
  std::cout << "ilb = " << ilb << std::endl;

  std::cout << std::endl;
  std::cout << "example1" << std::endl;
  example1();

  std::cout << std::endl;
  std::cout << "example2" << std::endl;
  example2();

  std::cout << std::endl;
  std::cout << "example3" << std::endl;
  example3();

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> a = ilb;
  std::cout << "a = " << a << std::endl;

  std::cout << std::endl;
  std::tr2::dynamic_bitset<> b(64, 0xccccccccUL);
  std::cout << "b = " << b << std::endl;
  std::cout << "b & ilb = " << (b & ilb) << std::endl;

  std::tr2::dynamic_bitset<> testc({0xccccccccUL, 0xccccccccUL, 0xccccccccUL, 0xccccccccUL});
  testc &= ilb;
  std::cout << "testc = " << testc << std::endl;

  std::tr2::dynamic_bitset<> testm({0xccccccccUL, 0xccccccccUL, 0xccccccccUL, 0xccccccccUL});
  testm &= std::tr2::dynamic_bitset<>({0xfacebeefUL, 0xdeadbabeUL,0xfacebeefUL, 0xdeadbabeUL});
  std::cout << "testm = " << testm << std::endl;
}
