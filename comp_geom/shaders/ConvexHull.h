/* === ConvexHull === */

/**
 * Return square of distance between p1 and p2.
 */
float
dist2(vec2 p1, vec2 p2)
{
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

/**
 * Return the 2D cross product of p0->p1 and p0->p2 vectors,
 * i.e. the z-component of their 3D cross product.
 * Returns a positive value, if p0->p1->p2 makes a counter-clockwise turn,
 * negative for clockwise turn, and zero if the points are collinear.
 */
float
cross(vec2 p0, vec2 p1, vec2 p2)
{
    return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

/**
 * An enumeration of the relative orientation of a sequence of three points.
 */
enum Orientation
{
  HULL_ORIENT_CLOCKWISE,
  HULL_ORIENT_COLLINEAR,
  HULL_ORIENT_COUNTER_CLOCKWISE,
};

/**
 * Return the orientation of an ordered triplet (p, q, r) or points.
 */
template<typename Tp>
  inline Orientation
  orientation(vec2 p, vec2 q, vec2 r)
  {
    auto val = cross(p, q, r);

    if (val == 0.0)
      return HULL_ORIENT_COLLINEAR;
    else
      return (val > 0) ? HULL_ORIENT_COUNTER_CLOCKWISE : HULL_ORIENT_CLOCKWISE;
  }

/**
 * A comparison functor for Points.
 */
struct Compare
{
    vec2 p0;
};

bool
compare(Compare comp, vec2 p1, vec2 p2)
{
  auto orient = orientation(p0, p1, p2);
  if (orient == HULL_ORIENT_COLLINEAR)
    return dist2(p0, p1) < dist2(p0, p2);
  else
    return orient == HULL_ORIENT_COUNTER_CLOCKWISE;
}

/* === ConvexHull === */
