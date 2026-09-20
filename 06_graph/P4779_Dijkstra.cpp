#include <bits/stdc++.h>
using namespace std;
struct Edge {
  int to;
  int weight;
};
const int Max = INT_MAX / 2;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m, s;
  cin >> n >> m >> s;
  vector<vector<Edge>> road(n + 1);
  for(int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    road[u].emplace_back(v, w);
  }
  vector<bool> vis(n + 1, false);
  vector<int> min_dist(n + 1, Max);
  min_dist[s] = 0;
  auto cmp = [](const Edge& a, const Edge& b) {
    return a.weight > b.weight;
  };
  priority_queue<Edge, vector<Edge>, decltype(cmp)> pq(cmp);
  pq.emplace(s, 0);
  while(!pq.empty()) {
    auto [cur_pos, dist] = pq.top();
    pq.pop();
    if(vis[cur_pos]) continue;
    vis[cur_pos] = true;
    for(auto [nxt_pos, w] : road[cur_pos]) {
      if(w + dist < min_dist[nxt_pos]) {
        min_dist[nxt_pos] = w + dist;
        pq.emplace(nxt_pos, min_dist[nxt_pos]);
      }
    }
  }
  for(int i = 1; i <= n; i++) {
    cout << min_dist[i] << " ";
  }
  cout << "\n";
  return 0;
}