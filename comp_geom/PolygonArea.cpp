/*
g++ -Wall -Wextra -o PolygonArea PolygonArea.cpp
./PolygonArea < polygon_area.in > polygon_area.out

g++ -std=c++14 -Wall -Wextra -o PolygonArea PolygonArea.cpp
./PolygonArea < polygon_area.in > polygon_area.out
*/

#include <iostream>

using namespace std;

const int MAXPOLY = 200;
double EPSILON = 0.000001;

struct Point
{
  double x;
  double y;
};

struct Polygon
{
  Point p[MAXPOLY];
  int n;
};

double
area(Polygon p)
{
  double total = 0;
  for (int i = 0; i < p.n; i++)
    {
      auto j = (i + 1) % p.n;
      total += (p.p[i].x * p.p[j].y) - (p.p[j].x * p.p[i].y);
    }
  return total / 2;
}

int
main()
{
  Polygon p;

  cout << "Enter the number of points in Polygon: ";
  cin >> p.n;
  cout << "Enter the coordinates of each point: <x> <y>";
  for (int i = 0; i < p.n; i++)
  {
    cin >> p.p[i].x;
    cin >> p.p[i].y;
  }

  auto a = area(p);
  if (a > 0)
    cout << "\nThe Area of Polygon with " << (p.n)
         << " points using Slicker Algorithm is : " << a << '\n';
  else
    cout << "\nThe Area of Polygon with " << p.n
         << " points using Slicker Algorithm is : " << -a << '\n';
}
