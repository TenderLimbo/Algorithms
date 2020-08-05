#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

//Условие

// Определите, сколько на интервале [a, b] содержится натуральных чисел,
//  в двоичной записи которых ровно k единиц.

long long comb(int k, int n) {
  if (k > n || k < 0 || n < 0)
    return 0;
  long long C[n + 1][n + 1];
  for (int i = 0; i <= n; i++) {
    C[i][0] = 1;
    C[i][i] = 1;
    for (int j = 1; j < i; j++) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C[n][k];
}

string binary(long long n) {
  string result;

  do result.push_back('0' + (n & 1));
  while (n >>= 1);

  return result;
}

int main() {
  ifstream input("input.txt");
  long long a = 0, b = 0;
  int k = 0;
  if (input) {
    input >> a >> b >> k;
  }
  b++;
  long long amount = 0;

  string sa = binary(a);
  string sb = binary(b);

  vector<int> a_vector;
  for (int i = 0; i < sa.size(); i++) {
    if (sa[i] == '1')
      a_vector.push_back(i);
  }
  vector<int> b_vector;
  for (int i = 0; i < sb.size(); i++) {
    if (sb[i] == '1')
      b_vector.push_back(i);
  }

  for (int i = 0; i < a_vector.size(); i++)
    cout << a_vector[i] << ' ';
  cout << endl;

  for (int i = 0; i < b_vector.size(); i++)
    cout << b_vector[i] << ' ';
  cout << endl;

  int i = 0;
  while((k - i) >= 0 && (i < a_vector.size())){
    cout << "A : " << k - i << " " << a_vector[a_vector.size() - 1 - i] << endl;
    amount -= comb(k - i, a_vector[a_vector.size() - 1 - i]);
    i++;
  }

  i = 0;
  while((k - i) >= 0 && (i < b_vector.size())){
    cout << "B : "<< k - i << " " << b_vector[b_vector.size() - 1 - i] << endl;
    amount += comb(k - i, b_vector[b_vector.size() - 1 - i]);
    i++;
  }

  ofstream output("output.txt");
  if (output)
    output << amount;
  cout << "ans : " << amount << endl;


  return 0;
}
