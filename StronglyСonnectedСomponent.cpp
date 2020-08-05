#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include<algorithm>

using namespace std;

// Условие

// Винни-Пух и Пятачок нанялись защищать компьютерную сеть от хакеров,
// которые выкачивали из компьютеров секретную информацию.
// Компьютерная сеть Винни-Пуха и Пятачка состояла из соединённых между собой больших ЭВМ,
// к каждой из которых подключалось несколько терминалов. Подключение к одной из больших ЭВМ,
// позволяет получить информацию, содержащуюся в памяти этой ЭВМ,
// а также всю информацию доступную для ЭВМ, к которым данная ЭВМ могла направлять запросы.
// Хакеры и раньше нападали на подобные компьютерные сети, и их тактика была известна.
// Поэтому Винни-Пух и Пятачок разработали специальную программу,
// которая помогла принять меры против готовившегося нападения. Тактика хакеров:
// при нападении хакеры всегда получают доступ к информации всех ЭВМ сети.
// Добиваются они этого, захватывая некоторые ЭВМ сети, чтобы от них можно было запросить
// информацию, которая имеется у оставшихся ЭВМ. Вариантов захвата существует множество,
// например, захватить все ЭВМ. Однако хакеры всегда выбирают такой вариант,
// при котором суммарное число терминалов у захваченных ЭВМ минимально.
// Необходимо определить список номеров ЭВМ, которые могут быть выбраны хакерами
// для захвата сети согласно их тактике.

void dfs(int v, const vector<vector<int>> &g, vector<bool> &used, vector<int> &order) {
  used[v] = true;
  for (int i = 0; i < g[v].size(); i++) {
    int to = g[v][i];
    if (!used[to])
      dfs(to, g, used, order);
  }
  order.push_back(v);
}

void dfs_trans(int v, const vector<vector<int>> &trans_g, vector<bool> &used, vector<int> &comp) {
  used[v] = true;
  comp.push_back(v);
  for (int i = 0; i < trans_g[v].size(); i++) {
    int to = trans_g[v][i];
    if (!used[to])
      dfs_trans(to, trans_g, used, comp);
  }
}

void PrintMatrix(const vector<vector<int>> &m) {
  cout << endl << endl;
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m.size(); j++)
      cout << m[i][j] << ' ';
    cout << endl;
  }
}

void Floyd(vector<vector<int>> &m) {
  for (int k = 0; k < m.size(); k++) {
    for (int i = 0; i < m.size(); i++) {
      for (int j = 0; j < m.size(); j++) {
        m[i][j] = (m[i][j] || (m[i][k] && m[k][j]));
      }
    }
  }
}

int main() {
  int n, t, v;
  vector<vector<int>> g;
  vector<vector<int>> trans_g;
  vector<bool> used;
  vector<vector<int>> matrix;
  vector<int> cost;
  ifstream input("input.txt");
  if (input) {
    input >> n;
    used.assign(n, false);
    matrix.assign(n, vector<int>(n));
    g.assign(n, vector<int>());
    trans_g.assign(n, vector<int>());
    for (int i = 0; i < n; i++) {
      input >> t;
      cost.push_back(t);
    }
    input >> t >> v;
    while (t != 0 && v != 0) {
      matrix[t - 1][v - 1] = 1;
      g[t - 1].push_back(v - 1);
      trans_g[v - 1].push_back(t - 1);
      input >> t >> v;
    }
  }
  Floyd(matrix);
  PrintMatrix(matrix);

  ofstream output("output.txt");

  int min = INT_MAX;
  int min_node = -1;
  for (int i = 0; i < n; i++) {
    int p = count(matrix[i].begin(), matrix[i].end(), 1);
    if ((p == n || (p == n - 1 && matrix[i][i] == 0)) && cost[i] < min) {
      min = cost[i];
      min_node = i;
    }
  }
  if (min != INT_MAX) {
    output << 1 << endl << min_node + 1;
    return 0;
  }

  vector<int> ist;
  vector<int> ans;
  int c = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrix[j][i] == 0)
        c++;
    }
    if (c == n) {
      ans.push_back(i + 1);
      ist.push_back(i);
    }
    c = 0;
  }

  vector<int> comp;
  vector<int> order;

  for (int i = 0; i < ist.size(); i++) {
    if (!used[ist[i]]) {
      dfs(ist[i], g, used, order);
    }
  }

  vector<bool> mark(n, false);
  mark = used;

  bool check = false;
  for (int i = 0; i < n; i++) {
    if (!used[i]) {
      check = true;
      break;
    }
  }

  if (check) {
    order.clear();
    used.assign(n, false);
    for (int i = 0; i < n; i++)
      if (!used[i])
        dfs(i, g, used, order);
    used.assign(n, false);
    vector<bool> metka(n,false);
    for (int i = 0; i < n; i++) {
      int u = order[n - 1 - i];
      if (!used[u]) {
        dfs_trans(u, trans_g, used, comp);
        if (comp.size() > 1) {
          if (mark[comp[0]] || metka[comp[0]])
            break;
          dfs(comp[0],g,metka,order);
          pair<int, int> min = {INT_MAX, -1};
          for (int j = 0; j < comp.size(); j++) {
            cout << comp[j] + 1 << ' ';
            if (cost[comp[j]] < min.first)
              min = {cost[comp[j]], comp[j]};
          }
          cout << endl;
          ans.push_back(min.second + 1);
        }
        comp.clear();
      }
    }
  }

  if (output) {
    output << ans.size() << endl;
    for (int i = 0; i < ans.size(); i++) {
      output << ans[i];
      if (i != ans.size() - 1)
        output << ' ';
    }
  }
  return 0;
}