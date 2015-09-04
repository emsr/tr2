//  http://stackoverflow.com/questions/32404711/how-to-save-all-vertices-in-shortest-path-using-dijkstra-algorithm

//  g++ -std=c++14 -o dijkstra dijkstra.cpp

#include <bits/stdc++.h>
#include <iostream>
#include <vector>

#define Num 4
#define Num 4

int
minDistance(const std::vector<int> & dist,
	    const std::vector<bool> & sptSet)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < Num; ++v)
    {
        if (sptSet[v] == false
	 && dist[v] <= min)
	{
	    min = dist[v];
	    min_index = v;
	}
    }

    return min_index;
}

void
printPath(const std::vector<int> & prev)
{
    std::vector<int> S;
    int u = Num;
    for (int i = 0; i < Num; ++i)
    {
        S.push_back(u);
        u = prev[u];
    }

    std::vector<int> v2(S.rbegin(), S.rend());
    std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, ", "));
}

void
dijkstra(int graph[Num][Num], int src)
{
    std::vector<int> prev(Num);
    std::vector<int> dist(Num, INT_MAX);

    std::vector<bool> sptSet(Num, false);

    dist[src] = 0;
    prev[src] = -1;

    for (int count = 0; count < Num - 1; ++count)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < Num; ++v)
	{
            if (!sptSet[v]
	     && graph[u][v]
	     && dist[u] != INT_MAX
             && dist[u] + graph[u][v] < dist[v])
	    {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    printPath(prev);
}

int
main()
{
    int
    graph[Num][Num]
    {
        {0, 1, 4, 5},
        {1, 0, 2, 2},
        {4, 2, 0, 3},
        {4, 2, 2, 0}
    };

    dijkstra(graph, 1);

    return 0;
}
