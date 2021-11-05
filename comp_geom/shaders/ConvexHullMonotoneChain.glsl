/* === ConvexHullMonotoneChain === */

#version 460

#define MAX_NUM_POINTS 100

/**
 * Return the convex hull of a set of points using the monotone munch algorithm.
 */
void
convex_hull_monotone_chain(vec2 point[MAX_NUM_POINTS], int n,
                           out vec2 hull[MAX_NUM_POINTS], out int h)
{
    // There must be at least 3 points.
    if (n < 3)
    {
        h = 0;
        return;
    }

    std::vector<Point<Tp>> hull(2 * n);

    // Sort points lexicographically.
    std::sort(point.begin(), point.end());

    // Test both branches.
    bool orig_direction = true;
    if (orig_direction)
      {
	int k = 0;

	// Build lower hull.
	for (int i = 0; i < n; ++i)
	  {
	    while (k >= 2 && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	// Build upper hull.
	// Skip both endopints to avoid duplicating hull points.
	for (int i = n - 2, t = k + 1; i > 0; i--)
	  {
	    while (k >= t && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	hull.resize(k);
	return hull;
      }
    else
      {
	int k = 0;

	// Build upper hull.
	for (int i = n - 1; i >= 0; i--)
	  {
	    while (k >= 2 && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	// Build lower hull.
	// Skip both endopints to avoid duplicating hull points.
	for (int i = 1, t = k + 1; i < n - 1; ++i)
	  {
	    while (k >= t && cross(hull[k - 2], hull[k - 1], point[i]) <= 0)
	      --k;
	    hull[k++] = point[i];
	  }

	hull.resize(k);
	return hull;
      }
  }

// Driver program to test above functions
int
main()
{
    vec2[7]
    point
    {
      { 0, 3 },
      { 2, 2 },
      { 1, 1 },
      { 2, 1 },
      { 3, 0 },
      { 0, 0 },
      { 3, 3 }
    };

    vec2 hull[7];
    int h = 0;
    convex_hull_monotone_chain(point, 7, hull, h);

    return 0;
}

/* === ConvexHullMonotoneChain === */
