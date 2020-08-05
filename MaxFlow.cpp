#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>

using namespace std;

//Условие

// Лабиринт задаётся матрицей смежности A размера n × n, где aij = 1,
// если комната i соединена с комнатой j посредством дороги
// (i и j — целые числа от 1 до n). Часть комнат является входами, часть — выходами.
// Входы и выходы задаются наборами комнат: { x1, …, xp } и { y1, …, yk } соответственно.

// Необходимо найти максимальное число людей, которых можно провести от входов до выходов
// таким образом, чтобы их пути не пересекались по комнатам (в том числе и по входам-выходам).


int dfs(vector<vector<int>> &cap, vector<bool> &used, int u, int t, int f) {
  if (u == t)
    return f;
  used[u] = true;
  for (int v = 0; v < used.size(); v++)
    if (!used[v] && cap[u][v] > 0) {
      int push = dfs(cap, used, v, t, min(f, cap[u][v]));
      if (push > 0) {
        cap[u][v] -= push;
        cap[v][u] += push;
        return push;
      }
    }
  return 0;
}

int main() {
  ifstream input("input.in");
  int n, s, t;
  vector<vector<int>> capacity;
  if (input) {
    input >> n >> s >> t;
    n = n * 2 + 2;
    capacity.assign(n, vector<int>(n));;
    int k;
    for (int i = 0; i < n / 2 - 1; i++) {
      capacity[i][i + n / 2 - 1] = 1;
      capacity[i + n / 2 - 1][i] = 1;
      for (int j = 0; j < n / 2 - 1; j++) {
        input >> k;
        capacity[i + n / 2 - 1][j] = k;
      }
    }
    int r;
    for (int i = 0; i < s; i++) {
      input >> r;
      capacity[n - 2][r - 1] = 1;
      capacity[r - 1][n - 2] = 1;
    }
    for (int i = 0; i < t; i++) {
      input >> r;
      capacity[n - 1][r - 1] = 1;
      capacity[r - 1][n - 1] = 1;
    }
  }

  int maxflow = 0;

  while (true) {
    vector<bool> used(capacity.size(), false);
    int push = dfs(capacity, used, n - 2, n - 1, INT_MAX);
    if (push == 0)
      break;
    maxflow += push;
  }

  ofstream output("output.out");
  output << maxflow;

  return 0;
}