#include <bits/stdc++.h>
using namespace std;
struct Edge{
  int to;
  int weight;
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> graph(n + 1);
  for(int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    graph[u].emplace_back(v, w);
    graph[v].emplace_back(u, w);
  }
  auto cmp = [&](const Edge& a, const Edge& b) {
    return a.weight > b.weight;
  };
  priority_queue<Edge, vector<Edge>, decltype(cmp)> pq(cmp);
  for(auto edge : graph[1]) {
    pq.emplace(edge);
  }
  vector<bool> vis (n + 1, false);
  vis[1] = true;
  int sum = 0;
  int done = 0;
  while(!pq.empty()) {
    auto [to, weight] = pq.top();
    pq.pop();
    if(vis[to]) continue;
    vis[to] = true;
    sum += weight;
    done++;
    if(done == n - 1) break;
    for(auto edge : graph[to]) {
      pq.emplace(edge);
    }
  }
  if(done == n - 1) cout << sum << "\n";
  else cout << "orz" << "\n";
  return 0;
}