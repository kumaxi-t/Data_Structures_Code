#include <bits/stdc++.h>
using namespace std;
const int Max = INT_MAX / 2;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  vector<vector<int>> road(n + 1, vector<int>(n + 1, Max));
  for(int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    road[u][v] = min(road[u][v], w);
    road[v][u] = min(road[v][u], w);
  }
  for(int i = 1; i <= n; i++) road[i][i] = 0;

  // 遍历所有点作为中间结点
  // 看是否有两端的点可以通过该结点绕路找到一条更短的路
  for(int k = 1; k <= n; k++) {
    for(int i = 1; i <= n; i++) {
      for(int j = 1; j <= n; j++) {
        if(road[i][k] != Max && road[k][j] != Max) {
          if(road[i][k] + road[k][j] < road[i][j]) {
            road[i][j] = road[i][k] + road[k][j];
          }
        }
      }
    }
  }

  for(int i = 1; i <= n; i++) {
    for(int j = 1; j <= n; j++) {
      cout << road[i][j] << ' ';
    }
    cout << "\n";
  }


  return 0;
}