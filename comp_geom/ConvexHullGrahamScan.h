
// A C++ program to find convex hull of a set of points
// Refer http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of orientation()

#include <vector>
//#include <stack>
#include <algorithm>

/**
 * A utility function push a point onto a vector.
 */
template<typename Tp>
  void
  push(std::vector<Tp>& S, Tp p)
  { S.push_back(p); }

/**
 * A utility function pop a point off a vector.
 */
template<typename Tp>
  void
  pop(std::vector<Tp>& S)
  { S.erase(S.end() - 1); }

/**
 * A utility to return the top of a vector.
 */
template<typename Tp>
  Tp
  top(std::vector<Tp>& S)
  { return S.back(); }

/**
 * A utility function to find next to top in a vector.
 */
template<typename Tp>
  Tp
  next(std::vector<Tp>& S)
  { return S[S.size() - 2]; }

/**
 * Return the convex hull of a set of points.
 */
template<typename Tp>
  std::vector<Point<Tp>>
  convex_hull_graham_scan(std::vector<Point<Tp>> point)
  {
    const int n = point.size();

    // There must be at least 3 points.
    if (n < 3)
      return std::vector<Point<Tp>>();

    // Find the bottom-most point.
    auto y_min = point[0].y;
    int i_min = 0;
    for (int i = 1; i < n; ++i)
      {
	const auto y = point[i].y;

	// Pick the bottom-most or chose the left-most point in case of tie
	if ((y < y_min) || (y == y_min && point[i].x < point[i_min].x))
	  {
	    i_min = i;
	    y_min = y;
	  }
      }

    // Place the bottom-most point at first position.
    std::swap(point[0], point[i_min]);
    i_min = 0;

    // Sort remaining points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2 has larger polar angle.
    // (in counterclockwise direction) than p1.
    std::sort(point.begin() + 1, point.end(), Compare<Tp>(point[0]));

    // Create an empty stack and push first three points to it.
    std::vector<Point<Tp>> hull;
    push(hull, point[0]);
    push(hull, point[1]);
    push(hull, point[2]);

    // Process remaining points.
    for (int i = 3; i < n; ++i)
      {
	// Keep removing top while the angle formed by points next-to-top,
	// top, and point[i] makes a non-left turn
	while (orientation(next(hull), top(hull), point[i]) != counter_clockwise)
	  pop(hull);
	push(hull, point[i]);
      }

    return hull;
  }
