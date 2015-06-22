//  $HOME/bin/bin/g++ -std=c++1y -o dr2344 dr2344.cpp

#include <sstream>
#include <iomanip>
#include <cassert>

void
test01()
{
  std::ostringstream x, y;
  x << "[" << std::left << std::setfill('x') << std::setw(20) << R"("AB \"CD\" EF")" << "]";
  assert(x.str() == "[\"AB \\\"CD\\\" EF\"xxxxxx]");
  y << "[" << std::left << std::setfill('y') << std::setw(20) << std::quoted(R"(GH "IJ" KL)") << "]";
  assert(y.str() == "[\"GH \\\"IJ\\\" KL\"yyyyyy]");
}

#include <iostream>

int
main()
{
  //  Should be: ["AB \"CD\" EF"xxxxxx]
  //  Gives    : ["AB \"CD\" EF"xxxxxx]
  std::cout << "[" << std::left << std::setfill('x') << std::setw(20) << R"("AB \"CD\" EF")" << "]" << std::endl;
  //  Should be: ["GH \"IJ\" KL"yyyyyy]
  //  Gives    : ["yyyyyyyyyyyyyyyyyyyGH \"IJ\" KL"]
  std::cout << "[" << std::left << std::setfill('y') << std::setw(20) << std::quoted(R"(GH "IJ" KL)") << "]" << std::endl;

  test01();
}
