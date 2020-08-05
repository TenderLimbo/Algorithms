#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include<algorithm>

using namespace std;


// Условие

// Некоторые школы связаны компьютерной сетью.
// Между школами заключены соглашения: каждая школа имеет список школ-получателей,
// которым она рассылает программное обеспечение всякий раз,
// получив новое бесплатное программное обеспечение (извне сети или из другой школы).
// При этом, если школа b есть в списке получателей школы a,
// то школа a может и не быть в списке получателей школы b.

// Необходимо решить следующие две подзадачи:

// Определить минимальное число школ,
// которым надо передать по экземпляру нового программного обеспечения,
// чтобы распространить его по всем школам сети в соответствии с соглашениями.
// Обеспечить возможность рассылки нового программного обеспечения из любой школы
// по всем остальным школам. Для этого можно расширять списки получателей некоторых школ,
// добавляя в них новые школы. Требуется найти минимальное суммарное число расширений списков,
// при которых программное обеспечение из любой школы достигло бы всех остальных школ.
// Под расширением понимается добавление одной новой школы-получателя в список получателей
// одной из школ.

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


void dfs (int v, const vector<vector<int>>& g, vector<bool>& used) {
  used[v] = true;
  for (int i=0; i<g[v].size(); ++i) {
    int to = g[v][i];
    if (! used[to])
      dfs (to,g,used);
  }
}

void find_comps(int n,int& comp_count, const vector<vector<int>>& g, vector<bool>& used) {
  for (int i=0; i<n; ++i)
    used[i] = false;
  for (int i=0; i<n; ++i)
    if (! used[i]) {
      dfs (i,g,used);
     comp_count++;
    }
}

int main() {
  int n,t;
  vector<vector<int>> g;
  vector<bool> used;
  vector<vector<int>> matrix;
  ifstream input("input.txt");
  if (input) {
    input >> n;
    used.assign(n,false);
    matrix.assign(n, vector<int>(n));
    g.assign(n, vector<int>());
    for (int i = 0; i < n; i++) {
      input >> t;
      while (t != 0) {
        g[i].push_back(t-1);
        matrix[i][t - 1] = 1;
        input >> t;
      }
    }
  }

 // matrix = { {0,0,0,1,0,1}, {0,0,0,1,0,0} , {0,0,0,1,0,1} ,{1,1,1,0,1,0},{0,0,0,1,0,0}, {1,0,1,0,0,0} };

  ofstream output("output.txt");
  Floyd(matrix);
  PrintMatrix(matrix);
  int outgoing = 0;
  int incoming = 0;

  int c = 0;
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix.size(); j++) {
      if (matrix[j][i] == 0) {
        c++;
      }
    }
    if (c == matrix.size()) {
      outgoing++;
    }
    c = 0;
  }


  int only_out_rows = 0;
  int out_ans = outgoing;
  for (int i = 0; i < matrix.size(); i++) {
    int p = count(matrix[i].begin(),matrix[i].end(),1);
    if (p == 0)
      incoming++;
    if (p == matrix.size()) {
      out_ans = 1;
      only_out_rows++;
    }
  }


 if (outgoing == 0 && incoming == 0 && only_out_rows != n) {
    int comp_count = 0;
    find_comps(n,comp_count,g,used);
    cout << comp_count << endl;
    out_ans = comp_count;
    outgoing = comp_count;
 }

  output << out_ans << endl << max(outgoing,incoming);

  return 0;
}