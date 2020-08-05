#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>

using namespace std;

// Условие

// Имеется клеточное поле размера n × m,
// в некоторых позициях которого расставлены чёрные фигуры.
// Необходимо расставить максимальное число белых ладей, чтобы они не били друг друга.

vector<vector<pair<int, int>>> temp_ans;
vector<vector<pair<int, int>>> ans;
int max_rooks = 0;
vector<vector<pair<int, int>>> full_ans;

void PrintMatrix(const vector<vector<int>> &m) {
  for (const auto &v : m) {
    for (const auto &el : v)
      cout << el << ' ';
    cout << endl;
  }
}

bool CheckMas(vector<int> &mas, vector<pair<int, int>> &temp, int &free_cells, int &depth, int x, int n, int m) {
  mas[x] = 2;
  int size = mas.size() - 1;
  free_cells -= 1;
  depth++;
  int i = x;
  bool first = false;
  if (x % m == 1)
    first = true;

  while (i % m != 1 || first) {
    first = false;
    if (mas[i] == 2 || mas[i] == -1) {
      i++;
      continue;
    }
    if (mas[i] == 0) {
      mas[i] = -1;
      free_cells -= 1;
    } else break;
    i++;
  }

  i = x;
  if (x % m == 0)
    first = true;

  while (i % m != 0 || first) {
    first = false;
    if (mas[i] == 2 || mas[i] == -1) {
      i--;
      continue;
    }
    if (mas[i] == 0) {
      mas[i] = -1;
      free_cells -= 1;
    } else break;
    i--;
  }

  i = x;
  while (i <= size) {
    if (mas[i] == 2 || mas[i] == -1) {
      i += m;
      continue;
    }
    if (mas[i] == 0) {
      mas[i] = -1;
      free_cells -= 1;
    } else break;
    i += m;
  }

  i = x;
  while (i >= 1) {
    if (mas[i] == 2 || mas[i] == -1) {
      i -= m;
      continue;
    }
    if (mas[i] == 0) {
      mas[i] = -1;
      free_cells -= 1;
    } else break;
    i -= m;
  }
  int col = x % m;
  if (!col)
    col = m;
  int row = (x - col) / m + 1;
  temp.emplace_back(col, row);
  return free_cells != 0 && free_cells >= max_rooks - depth;
}

void TryRookMas(vector<int> mas, int x, int n, int m, int free_cells, vector<pair<int, int>> &temp, int depth) {
  if (CheckMas(mas, temp, free_cells, depth, x, n, m)) {
    for (int i = x + 1; i < mas.size(); i++) {
      if (mas[i] == 0) {
        TryRookMas(mas, i, n, m, free_cells, temp, depth);
        temp.pop_back();
      }
    }
  } else {
    if (temp.size() >= max_rooks) {
      max_rooks = temp.size();
      if (!ans.empty()) {
        if (ans[ans.size() - 1].size() < max_rooks) {
          ans.clear();
        }
      }
      ans.push_back(temp);
    }
    return;
  }
}

bool can(int x, int y, vector<vector<int>> &field, int n, int m) {
  return x >= 0 && x < n && y >= 0 && y < m && !field[x][y];
}

void bfs(vector<pair<int, int>> &steps, vector<vector<int>> &field, int x, int y, int count) {
  queue<pair<int, int>> q;
  q.push({x, y});
  field[x][y] = count;
  while (!q.empty()) {
    pair<int, int> cur = q.front();
    q.pop();
    int cx = cur.first;
    int cy = cur.second;
    for (int i = 0; i < steps.size(); i++) {
      pair<int, int> step = steps[i];
      int dx = step.first, dy = step.second;
      if (can(cx + dx, cy + dy, field, field.size(), field[0].size())) {
        q.push({cx + dx, cy + dy});
        field[cx + dx][cy + dy] = count;
      }
    }
  }
}

void foo(vector<int> &mas, int n, int m, int k) {
  int free_cells;
  int depth;
  vector<pair<int, int>> temp;
  vector<int> v;
  for (int i = 1; i < mas.size(); i++) {
    if (!mas[i]) {
      v = mas;
      free_cells = mas.size() - k - 1;
      depth = 0;
      TryRookMas(v, i, n, m, free_cells, temp, depth);
      temp.clear();
    }
  }
}

vector<int> edit(vector<vector<int>> &field, int &k, int count) {
  vector<int> mas;
  mas.push_back(0);
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field[i].size(); j++) {
      if (field[i][j] == count)
        mas.push_back(0);
      else {
        mas.push_back(1);
        k++;
      }
    }
  }
  return mas;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  fstream input("input.txt");
  FILE *out;
  out = fopen("output.txt", "w");
  int n, m, k;
  vector<vector<int>> field;
  vector<pair<int, int>> steps = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  if (input) {
    input >> n >> m >> k;
    if (k == n * m) {
      fprintf(out, "%s", "<--->");
      fprintf(out, "%c", '\n');
      fprintf(out, "%d", 1);
      return 0;
    }
    field.assign(n, vector<int>(m));
    int a, b;
    for (int i = 0; i < k; i++) {
      input >> a >> b;
      field[a - 1][b - 1] = 1;

    }
  }

  int count = 2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (field[i][j] == 0) {
        bfs(steps, field, i, j, count);
        int black = 0;
        vector<int> v = edit(field, black, count);
        foo(v, n, m, black);
        if (!temp_ans.empty()) {
          if (!full_ans.empty())
            temp_ans = full_ans;
          full_ans.clear();
          for (int i = 0; i < temp_ans.size(); i++) {
            for (int j = 0; j < ans.size(); j++) {
              vector<pair<int, int>> t = temp_ans[i];
              t.insert(t.end(), ans[j].begin(), ans[j].end());
              full_ans.push_back(t);
            }
          }
        } else
          temp_ans = ans;
        ans.clear();
        max_rooks = 0;
        count++;
      }
    }
  }


  if (full_ans.empty())
    full_ans = temp_ans;
  for (const vector<pair<int, int>> &p : full_ans) {
    for (const pair<int, int> &el : p) {
      fprintf(out, "%d ", el.first);
      fprintf(out, "%d", el.second);
      fprintf(out, "%c", '\n');
    }
    fprintf(out, "%s", "<--->");
    fprintf(out, "%c", '\n');
  }
  fprintf(out, "%d", full_ans.size());

}