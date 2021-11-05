/* === ConvexHullJarvisMarch === */

// A C++ program to find convex hull of a set of points using the Jarvis march algorithm.

#version 460

#define MAX_NUM_POINTS 100

/**
 * Return the convex hull of a set of points.
 */
void
convex_hull_jarvis_march(vec2 point[MAX_NUM_POINTS], int n,
                         out vec2 hull[MAX_NUM_POINTS], out int h)
{
    const int n = point.size();

    // There must be at least 3 points.
    if (n < 3)
    {
        h = 0;
        return;
    }

    // Find the lower left-most point.
    int l = 0;
    for (int i = 1; i < n; ++i)
    {
	if (point[i].x < point[l].x)
	    l = i;
	else if (point[i].x == point[l].x && point[i].y < point[l].y)
	    l = i;
    }

    // Start from left-most point, keep moving counterclockwise
    // until reach the start point again.
    int p = l, q;
    h = 0;
    do
    {
	hull.push_back(point[p]);

	// Search for a point 'q' such that orientation(p, i, q) is
	// counterclockwise for all points 'i'.
	q = (p + 1) % n;
	for (int i = 0; i < n; ++i)
        {
	    if (orientation(point[p], point[i], point[q]) == HULL_ORIENT_COUNTER_CLOCKWISE)
                q = i;
        }

	p = q; // Set p as q for next iteration.
    }
    while (p != l);

    return hull;
  }

// Driver program to test above functions.
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
    convex_hull_jarvis_march(point, 7, hull, h);

    return 0;
}

/* === ConvexHullJarvisMarch === */
