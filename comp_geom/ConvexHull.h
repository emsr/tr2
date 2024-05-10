#ifndef CONVEXHULL_H
#define CONVEXHULL_H 1

/**
 * A two-dimensional point.
 */
template<typename Tp>
  struct Point
  {
    Tp x;
    Tp y;

    bool
    operator<(const Point& p) const
    { return x < p.x || (x == p.x && y < p.y); }
  };

/**
 * Return square of distance between p1 and p2.
 */
template<typename Tp>
  inline Tp
  dist2(const Point<Tp>& p1, const Point<Tp>& p2)
  { return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y); }

/**
 * Return the 2D cross product of p0->p1 and p0->p2 vectors,
 * i.e. the z-component of their 3D cross product.
 * Returns a positive value, if p0->p1->p2 makes a counter-clockwise turn,
 * negative for clockwise turn, and zero if the points are collinear.
 */
template<typename Tp>
  inline Tp
  cross(const Point<Tp>& p0, const Point<Tp>& p1, const Point<Tp>& p2)
  { return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x); }

/**
 * An enumeration of the relative orientation of a sequence of three points.
 */
enum Orientation
{
  clockwise,
  collinear,
  counter_clockwise,
};

/**
 * Return the orientation of an ordered triplet (p, q, r) or points.
 */
template<typename Tp>
  inline Orientation
  orientation(const Point<Tp>& p, const Point<Tp>& q, const Point<Tp>& r)
  {
    auto val = cross(p, q, r);

    if (val == 0)
      return collinear;
    else
      return (val > 0) ? counter_clockwise : clockwise;
  }

/**
 * A comparison functor for Points.
 */
template<typename Tp>
  struct Compare
  {
    Point<Tp> p0;

    Compare(Point<Tp> center)
    : p0(center)
    { }

    bool
    operator()(Point<Tp> p1, Point<Tp> p2)
    {
      auto orient = orientation(p0, p1, p2);
      if (orient == collinear)
	return dist2(p0, p1) < dist2(p0, p2);
      else
	return orient == counter_clockwise;
    }
  };

#include "ConvexHullGiftWrap.h"
#include "ConvexHullGrahamScan.h"
#include "ConvexHullJarvisMarch.h"
#include "ConvexHullMonotoneChain.h"

#endif // CONVEXHULL_H
