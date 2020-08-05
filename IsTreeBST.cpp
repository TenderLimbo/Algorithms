#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>

using namespace std;

// Условие

// Задано бинарное дерево. Необходимо проверить, является ли оно поисковым.
// Будем предполагать, что в бинарном поисковом дереве могут быть вершины с
// одинаковыми ключами. Тогда, для того, чтобы дерево было поисковым,
// должно выполняться следующее требование: для каждой вершины xx все ключи в левом поддереве 
// вершины xx меньше ключа вершины xx, а все ключи в правом поддереве больше либо равны ключу
// вершины xx.

struct node {
  int val, min, max;
};

int main() {
  int n, r;
  ifstream input("bst.in");
  ofstream output("bst.out");
  vector<node> t;
  if (input) {
    input >> n >> r;
    t.push_back({r, INT_MIN, INT_MAX});
    int m, p;
    char c;
    for (int i = 0; i < n - 1; i++) {
      input >> m >> p >> c;
      node temp;
      temp.val = m;
      if (c == 'L') {
        if (t[p - 1].val == INT_MIN) {
          output << "NO";
          return 0;
        }
        temp.max = t[p - 1].val - 1;
        temp.min = t[p - 1].min;
      } else {
        temp.min = t[p - 1].val;
        temp.max = t[p - 1].max;
      }
      if (m < temp.min || m > temp.max) {
        output << "NO";
        return 0;
      }
      t.push_back(temp);
    }
  }
  if (output)
    output << "YES";
  return 0;
}