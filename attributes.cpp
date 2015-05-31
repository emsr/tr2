
// /home/ed/bin/bin/g++ -std=c++1z -pedantic -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++1z -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++14 -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++14 -pedantic -o attributes attributes.cpp

// /home/ed/bin_biniold/bin/g++ -std=c++14 -o attributes attributes.cpp

// /home/ed/bin_biniold/bin/g++ -std=c++14 -pedantic -o attributes attributes.cpp

enum class Planets
{
  Mercury,
  Venus,
  Earth,
  Mars,
  Jupiter,
  Saturn,
  Uranus [[deprecated("We'll stop those jokes once and for all!")]],
  Urectum = Uranus,
  Neptune,
  Pluto [[removed("Pluto - the first of the minor planets!")]]
};

enum Colors
{
  Red,
  Green,
  Mauve [[deprecated("Mauve is not a real color.")]],
  Blue
};


class C
{
public:
  enum Foo
  {
    T,
    U [[deprecated("unused")]],
    V
  };
};

template<typename Tp>
  class D
  {
  public:
    enum Bar
    {
      X,
      Y [[deprecated("unused")]],
      Z
    };
  };


namespace poo [[gnu::visibility("default")]]
{
  int num_iters;

  Planets hot = Planets::Mercury;
  Planets favorite = Planets::Uranus;
  Planets poor_pluto = Planets::Pluto;

  int i = Mauve;

  auto j = C::U; // { dg-warning ".C::U. is deprecated" }

  auto k = D<int>::Y; // { dg-warning ".D<int>::Y. is deprecated" }
}

int
main()
{
}
