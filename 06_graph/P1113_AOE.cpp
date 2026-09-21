#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  cin >> n;
  vector<vector<int>> road(n + 1);
  vector<int> cost(n + 1);
  vector<int> min_cost(n + 1, 0);
  vector<int> indegree(n + 1, 0);
  for(int i = 1; i <= n; i++) {
    int num, v;
    cin >> num >> v;
    cost[num] = v;
    while(true) {
      int from;
      cin >> from;
      if(from == 0) break;
      road[from].emplace_back(num);
      indegree[num]++;
    }
  }
  queue<int> q;
  for(int i = 1; i <= n; i++) {
    if(indegree[i] == 0) {
      q.push(i);
      min_cost[i] = cost[i];
    }
  }

  int maxv = 0;
  while(!q.empty()) {
    int cur = q.front();
    q.pop();
    for(auto to : road[cur]) {
      if(min_cost[to] < cost[to] + min_cost[cur]) {
        min_cost[to] = cost[to] + min_cost[cur];
        maxv = max(maxv, min_cost[to]);
      }
      indegree[to]--;
      if(indegree[to] == 0) q.push(to);
    }
  }

  cout << maxv << "\n";

  return 0;
}