//  http://stackoverflow.com/questions/32404711/how-to-save-all-vertices-in-shortest-path-using-dijkstra-algorithm

//  g++ -std=c++14 -o dijkstra dijkstra.cpp

#include <iostream>
#include <vector>
#include <climits>
#include <iterator>
#include <limits>

template<typename IdxTp, typename DistTp>
  IdxTp
  minDistanceIdx(const std::vector<DistTp> & dist,
                 const std::vector<bool> & sptSet)
  {
    DistTp min = std::numeric_limits<DistTp>::max();
    int min_index;

    for (int v = 0; v < dist.size(); ++v)
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

template<typename IdxTp>
  void
  printPath(const std::vector<IdxTp> & prev)
  {
    std::vector<IdxTp> S;
    IdxTp u = prev.size() - 1;
    for (IdxTp i = 0; i < prev.size() && u != std::numeric_limits<IdxTp>::max(); ++i)
    {
      S.push_back(u);
      u = prev[u];
    }

    std::vector<IdxTp> v2(S.rbegin(), S.rend());
    std::copy(v2.begin(), v2.end(), std::ostream_iterator<IdxTp>(std::cout, ", "));
    std::cout << '\n';
  }

template<typename IdxTp, typename DistTp, IdxTp Num>
  void
  dijkstra(const DistTp (&graph)[Num][Num], IdxTp src)
  {
    std::vector<IdxTp> prev(Num);
    std::vector<DistTp> dist(Num, std::numeric_limits<DistTp>::max());

    std::vector<bool> sptSet(Num, false);

    dist[src] = 0;
    prev[src] = std::numeric_limits<IdxTp>::max();

    for (int count = 0; count < Num; ++count)
    {
      IdxTp u = minDistanceIdx<IdxTp>(dist, sptSet);
      //if (u == -1)
	//break;

      sptSet[u] = true;

      for (IdxTp v = 0; v < Num; ++v)//Num - 1
      {
	if (!sptSet[v]
	 && graph[u][v]
	 && dist[u] != std::numeric_limits<DistTp>::max()
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
  graph1[4][4]
  {
    {0, 1, 4, 5},
    {1, 0, 2, 2},
    {4, 2, 0, 3},
    {4, 2, 2, 0}
  };

  int
  graph2[8][8]
  {
    {0, 1, 4, 5, 1, 2, 1, 3},
    {1, 0, 2, 2, 6, 2, 3, 1},
    {4, 2, 0, 3, 2, 1, 7, 2},
    {4, 2, 2, 0, 1, 3, 1, 3},
    {1, 6, 2, 1, 0, 1, 4, 5},
    {2, 2, 1, 4, 1, 0, 2, 2},
    {1, 3, 9, 2, 4, 2, 0, 3},
    {3, 1, 2, 3, 4, 2, 2, 0}
  };

  dijkstra(graph1, 1);

  dijkstra(graph2, 4);

  return 0;
}
