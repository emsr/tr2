// $HOME/bin/bin/g++ -std=c++14 -o test_make_array test_make_array.cpp

#include "make_array"
#include <iostream>

int
main()
{
  double arr[]{0.0, 1.0, 2.0};

  auto a = std::experimental::make_array(0.0, 1.0, 2.0);
  std::cout << "a.size() = " << a.size() << '\n';
  std::cout << "a = {" << a[0] << ", " << a[1] << ", " << a[2] << "}\n";
  auto b = std::experimental::to_array(arr);
  std::cout << "b.size() = " << b.size() << '\n';
  std::cout << "b = {" << b[0] << ", " << b[1] << ", " << b[2] << "}\n";

  double x = 0.0, y = 1.1, z = 2.2;
  
  auto c = std::experimental::make_array(x, y, z);
  std::cout << "c.size() = " << c.size() << '\n';
  std::cout << "c = {" << c[0] << ", " << c[1] << ", " << c[2] << "}\n";

  double & rx = x;
  double & ry = y;
  double & rz = z;
  auto d = std::experimental::make_array(rx, ry, rz);
  std::cout << "d.size() = " << d.size() << '\n';
  std::cout << "d = {" << d[0] << ", " << d[1] << ", " << d[2] << "}\n";

  //auto e = std::experimental::make_array(std::ref(x), std::ref(y), std::ref(z));
  //std::cout << "e.size() = " << e.size() << '\n';
  //std::cout << "e = {" << e[0] << ", " << e[1] << ", " << e[2] << "}\n";

  auto f = std::experimental::make_array(&x, &y, &z);
  std::cout << "f.size() = " << f.size() << '\n';
  std::cout << "f = {" << *f[0] << ", " << *f[1] << ", " << *f[2] << "}\n";
}
