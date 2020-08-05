#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


//Условие

// Изначально дана последовательность чисел A длины n (индексация с нуля):
//   a_0, a_1, a_2, ... , a_{n-1}.

// Поступают запросы двух типов.
// Запрос модификации. Задан индекс i и число x. Нужно прибавить к i-му элементу число x.
// Запрос суммы. Задана пара индексов l и r.
// Нужно вычислить сумму элементов на полуинтервале [l, r), и вернуть результат.

void SetTree(const vector<int> &v, vector<long long> &tree, int i, int a, int b) {
  if ((b - a) == 1) {
    tree[i] = v[a];
  } else {
    int m = (a + b) / 2;
    SetTree(v, tree, 2 * i, a, m);
    SetTree(v, tree, 2 * i + 1, m, b);
    tree[i] = tree[2 * i] + tree[2 * i + 1];
  }
}

void Add(vector<long long> &tree, int i, int a, int b, int index, int add) {
  if ((b - a) == 1) {
    tree[i] += add;
    return;
  }
  int m = (a + b) / 2;
  if (index < m)
    Add(tree, 2 * i, a, m, index, add);
  else
    Add(tree, 2 * i + 1, m, b, index, add);
  tree[i] = tree[2 * i] + tree[2 * i + 1];
}

long long Sum(vector<long long> &tree, int i, int a, int b, int l, int r) {
  if (l == a && r == b)
    return tree[i];

  int m = (a + b) / 2;
  if (r <= m)
    return Sum(tree, 2 * i, a, m, l, r);
  if (m <= l)
    return Sum(tree, 2 * i + 1, m, b, l, r);

  return Sum(tree, 2 * i, a, m, l, m) + Sum(tree, 2 * i + 1, m, b, m, r);
}

int my_log(int b) {
  if (b < 0)
    return -1;
  int p = 1;
  int count = 0;
  while (p <= b) {
    p <<= 1;
    count += 1;
  }
  return count - 1;
}


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  cin >> n;
  vector<int> v(n);
  for (int &el : v) {
    cin >> el;
  }
  vector<long long> tree(pow(2,my_log(2*n - 1) + 1));
  SetTree(v, tree, 1, 0, n);
  int q;
  cin >> q;
  string request;
  int a, b;
  for (int i = 0; i < q; i++) {
    cin >> request >> a >> b;
    if (request == "FindSum")
      cout << Sum(tree, 1, 0, n, a, b) << endl;
    else
      Add(tree, 1, 0, n, a, b);
  }
  return 0;
}