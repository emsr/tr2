// g++ -std=c++11 -g -o test_dijkstra test_dijkstra.cpp

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

int
test_dijkstra()
{
  int num_vertices, num_edges;
  std::cin >> num_vertices >> num_edges;
  std::vector<std::unordered_map<int,int>> vertex(num_vertices + 1);
  for (int e = 0; e < num_edges; ++e)
  {
    int a, b, s;
    std::cin >> a >> b >> s;
    auto ret = vertex[e].insert(std::pair<int,int>(b,s));
    if ((*ret.first).first > s) // exists
      vertex[a][b] = s;
  }

  int x, y;
  std::cin >> x >> y;
  std::map<int, int> q;
  for (auto it = vertex[x].begin(), end = vertex[x].end(); it != end; ++it)
    q.insert(std::pair<int,int>(it->second, it->first));

  std::vector<int> dist(num_vertices + 1);
  const auto end = vertex[x].end();
  for (int i = 1; i <= num_vertices; ++i)
  {
    if (i == x)
      continue;
    const auto tmp = vertex[x].find(i);
    dist[i] = (tmp != end) ? tmp->second : -1;
  }

  for (int i = 1; i <= num_vertices; ++i)
  {
    std::cout << ' ' << dist[i];
  }
  std::cout << '\n';

  return 0;
}

int
main()
{
  test_dijkstra();
}

/*
#include <climits>
#include <cstdio>
#include <map>
#include <unordered_map>

const int NODES_MAX = 10000;

using namespace std;
using namespace std::tr1;

int test_dijkstra() {
  int v, e, a, b, s, *dist;
  map<int, int> q;

  scanf( "%d %d\n", &v, &e );
  unordered_map<int,int> * nodes = new unordered_map<int,int>[v];
  unordered_map<int,int> * tmp2;
  pair<unordered_map<int,int>::iterator,bool> ret;
  dist = new int[v];
printf("!");
  for( ; e > 0; e-- ) {
    scanf( "%d %d %d\n", &a, &b, &s );
tmp2 = &nodes[a];
    printf("%d %d %d=\n",a,b,s);
tmp2[0].insert(pair<int,int>( 0, 0 ) );
    printf("/");

    ret = nodes[a].insert( pair<int,int>( b, s ) ); // code below is nec. since int() == 0
        printf(",");
    if ( ret.first->first > s ) // exists
      nodes[a][b] = s;
  }
printf("!");
  scanf( "%d %d\n", &a, &b ); // test case
  for( unordered_map<int,int>::iterator it = nodes[a].begin(), end = nodes[a].end(); it != end; it++ )
    q.insert( pair<int,int>(it->second, it->first) );

  //int cur_node = a;
  unordered_map<int,int>::iterator end = nodes[a].end(), tmp;
  for( int i = 0; i < v; i++ ) {
    if ( i == a )
      continue;
    tmp = nodes[a].find(i);
    dist[i] = (tmp != end) ? tmp->second : INT_MAX;
  }
  for( int i = 0; i < v; i++ ) {
  //for( map<int,int>::iterator it = q.begin(), end = q.end(); it != end; it++ )
//    if
  printf("%d ",dist[i]);}
  printf("\n");
  delete [] nodes;
  delete [] dist;
  return 0;
}
*/
