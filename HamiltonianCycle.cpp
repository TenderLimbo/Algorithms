#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

// Условие

// Имеется полный граф на n вершинах.
// Каждая вершина задаётся своими целочисленными координатами на плоскости.
// Вес ребра { u, v } задаётся по следующей формуле:
//   d(u, v) = |xu − xv| + |yu − yv|.
// Вам нужно найти гамильтонов цикл минимального веса в этом графе.

struct node {
  int x;
  int y;
};

int main() {
  cin.tie(NULL);
  ios_base::sync_with_stdio(false);
  ifstream input("input.txt");
  ofstream output("output.txt");
  int n;
  vector<node> g;

  if (input) {
    input >> n;
    int x, y;
    for (int i = 0; i < n; i++) {
      input >> x >> y;
      g.push_back({x, y});
    }
  }

  vector<stack<int>> ost_list(n);
  vector<bool> used(n);
  vector<int> min_dist(n, INT_MAX);
  vector<int> sec_vert(n, -1);
  vector<pair<pair<int, int>, bool>> edges;

  min_dist[0] = 0;
  for (int i = 0; i < n; i++) {
    int v = -1;
    for (int j = 0; j < n; j++)
      if (!used[j] && (v == -1 || min_dist[j] < min_dist[v]))
        v = j;
    used[v] = true;
    if (sec_vert[v] != -1) {
      edges.push_back({{v, sec_vert[v]}, false});
      edges.push_back({{v, sec_vert[v]}, false});
      ost_list[v].push(edges.size() - 2);
      ost_list[v].push(edges.size() - 1);
      ost_list[sec_vert[v]].push(edges.size() - 2);
      ost_list[sec_vert[v]].push(edges.size() - 1);
    }
    int dist;
    for (int to = 0; to < n; to++) {
      dist = abs(g[v].x - g[to].x) + abs(g[v].y - g[to].y);
      if (dist < min_dist[to]) {
        min_dist[to] = dist;
        sec_vert[to] = v;
      }
    }
  }

  vector<int> eiler;
  stack<int> st;
  st.push(0);

  while (!st.empty()) {
    int v = st.top();
    if (ost_list[v].empty()) {
      eiler.push_back(v);
      st.pop();
    } else {
      int to = ost_list[v].top();
      while (edges[to].second && ost_list[v].size() >= 2) {
        ost_list[v].pop();
        to = ost_list[v].top();
      }
      ost_list[v].pop();
      if (v == edges[to].first.second)
        st.push(edges[to].first.first);
      else
        st.push(edges[to].first.second);
      edges[to].second = true;
    }

  }

  vector<int> ans;
  set<int> s;

  int dist = 0;
  for (const int &el : eiler) {
    int temp_size = s.size();
    s.insert(el);
    if (s.size() > temp_size) {
      ans.push_back(el);
      if (ans.size() > 1) {
        int temp = abs(g[ans[ans.size() - 1]].x - g[ans[ans.size() - 2]].x)
            + abs(g[ans[ans.size() - 1]].y - g[ans[ans.size() - 2]].y);
        dist += temp;
      }
    }
  }
  ans.push_back(eiler[0]);
  dist +=abs(g[ans[ans.size() - 1]].x - g[ans[ans.size() - 2]].x)
      + abs(g[ans[ans.size() - 1]].y - g[ans[ans.size() - 2]].y);
  if (output) {
    output << dist << '\n';
    for (const int &el : ans) {
      output << el + 1 << ' ';
    }
  }
  return 0;
}
