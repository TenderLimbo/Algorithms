#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;


// Условие

// Задан ориентированный граф G, содержащий n вершин и m дуг,
// в котором каждой дуге (u, v) приписана пропускная способность С(u,v).

// Требуется найти в этой сети поток максимальной величины из источника (вершины 1)
// в сток (вершину n).


struct Rib {
  int to;
  int flow;
  int capacity;
  int back;
};

long long dfs(vector<vector<Rib>> &g, vector<int> &distance, vector<int> &ptr, int u, int f, int b) {
  if (u == b || f == 0)
    return f;
  for (int i = ptr[u]; i < g[u].size(); i++) {
    if (g[u][i].capacity > g[u][i].flow) {
      int to = g[u][i].to;
      if (distance[to] == distance[u] + 1) {
        long long push = dfs(g, distance, ptr, to, min(f, g[u][i].capacity - g[u][i].flow), b);
        if (push > 0) {
          g[u][i].flow += push;
          g[to][g[u][i].back].flow -= push;
          return push;
        }
      }
    }
    ptr[u]++;
  }
  return 0;
}

int bfs(vector<vector<Rib>> &g, vector<int> &distance, vector<int> &q, int a, int b) {
  distance.assign(distance.size(), INT_MAX);
  distance[a] = 0;
  int it_end = 0;
  q [it_end] = a;
  it_end++;
  for (int it = 0; it < it_end; it++) {
    int u = q[it];
    for (int v = 0; v < g[u].size(); v++) {
      int to = g[u][v].to;
      int delta = g[u][v].capacity - g[u][v].flow;
      if (distance[to] == INT_MAX && delta > 0) {
        distance[to] = distance[u] + 1;
        q[it_end] = to;
        it_end++;
      }
    }
  }
  return distance[b] != INT_MAX;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n, m;
  vector<int> distance;
  vector<int> ptr;
  vector<vector<Rib>> g;
  cin >> n >> m;
  distance.assign(n, -1);
  g.assign(n, vector<Rib>());
  int x, y, c;
  for (int i = 0; i < m; i++) {
    cin >> x >> y >> c;
    g[x - 1].push_back({y - 1, 0, c, int(g[y - 1].size())});
    g[y - 1].push_back({x - 1, 0, 0, int(g[x - 1].size() - 1)});
  }

  long long maxflow = 0;
  int a = 0;
  int b = n - 1;

  vector <int> q(n,0);
  while (bfs(g, distance,q, a, b)) {
    ptr.assign(n, 0);
    while (long long push = dfs(g, distance, ptr, a, INT_MAX, b))
      maxflow += push;
  }
  cout << maxflow << endl;
  return 0;
}
