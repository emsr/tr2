// /home/ed/bin_literal/bin/g++ -std=c++1y -fdiagnostics-color=auto -o quotedstd quotedstd.cpp

#include <string>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>

int
main()
{
  std::stringstream ss;
  std::string original = "foolish me";
  std::string round_trip;

  ss << std::quoted(original);
  std::cout << "original: " << original << '\n';
  std::cout << "std::quoted(original): " << std::quoted(original) << '\n';
  ss >> std::quoted(round_trip);
  std::cout << "round_trip: " << round_trip << '\n';

  std::cout << "original: " << original << '\n';     // outputs: foolish me
  std::cout << "round_trip: " << round_trip << '\n';   // outputs: foolish me

  assert(original == round_trip); // assert will not fire

  std::cout << std::quoted("She said \"Hi!\"") << '\n';  // outputs: "She said \"Hi!\""
  std::cout << std::quoted("She said \"Hi!\"", '\'', '&') << '\n';  // outputs: 'She said "Hi!"'
  std::cout << std::quoted("Harry & Sally", '\'', '&') << '\n';  // outputs: 'Harry && Sally'

  std::cout << "Enter quoted string: ";
  std::string quote;
  //std::cin >> std::quoted(quote);
  std::cout << "Quoted string: " << quote << '\n';

  std::cout << "Quoted string: " << std::quoted("\"There's a dead bishop on the landing!\"") << '\n';

  //  Test skipws correctness.
  ss.seekg(0);
  ss.seekp(0);
  ss.clear();
  ss << std::quoted("Hello Goodbye") << ' ' << 1 << ' ' << 2;
  std::cout << "ss.str(): " << ss.str() << '\n';
  std::string song;
  int thing1, thing2;
  ss >> std::quoted(song) >> thing1 >> thing2;
  std::cout << "song: " << song << '\n';
  std::cout << "thing1: " << thing1 << '\n';
  std::cout << "thing2: " << thing2 << '\n';
  assert(song == "Hello Goodbye");
  assert(thing1 == 1);
  assert(thing2 == 2);

  //  Test read of unquoted string.
  ss.seekg(0);
  ss.seekp(0);
  ss.clear();
  ss << "Alpha Omega";
  std::string test;
  ss >> std::quoted(test);
  assert(test == "Alpha");

  //  Test delimited string extension.
}
