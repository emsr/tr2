// Implementation of Andrew's monotone chain 2D convex hull algorithm.
#include <algorithm>
#include <vector>
using namespace std;
 
typedef int coord_t;         // coordinate type
typedef long long coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct Point
{
  coord_t x, y;
 
  bool
  operator<(const Point &p) const
  { return x < p.x || (x == p.x && y < p.y); }
};

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t
cross(const Point &O, const Point &A, const Point &B)
{
  return (A.x - O.x) * (coord2_t)(B.y - O.y) - (A.y - O.y) * (coord2_t)(B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<Point>
convex_hull(vector<Point> P)
{
  int n = P.size(), k = 0;
  vector<Point> H(2*n);

  // Sort points lexicographically
  sort(P.begin(), P.end());

  // Build lower hull.
  for (int i = 0; i < n; i++)
  {
    while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0)
      k--;
    H[k++] = P[i];
  }

  // Build upper hull.
  for (int i = n-2, t = k+1; i >= 0; i--)
  {
    while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0)
      k--;
    H[k++] = P[i];
  }

  H.resize(k);
  return H;
}
