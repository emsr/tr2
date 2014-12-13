
// /home/ed/bin/bin/g++ -std=c++1z -pedantic -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++1z -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++14 -o attributes attributes.cpp

// /home/ed/bin_binio/bin/g++ -std=c++14 -pedantic -o attributes attributes.cpp

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
    XX,
    YY [[deprecated("unused")]],
    ZZ
  };
};

template<typename Tp>
  class D
  {
  public:
    enum Foo
    {
      XX,
      YY [[deprecated("unused")]],
      ZZ
    };
  };


namespace poo [[gnu::visibility("default")]]
{
  int num_iters;

  Planets hot = Planets::Mercury;
  Planets favorite = Planets::Uranus;
  Planets poor_pluto = Planets::Pluto;

  int i = Mauve;

  auto j = C::YY;

  auto k = D<int>::YY;
}

int
main()
{
}
