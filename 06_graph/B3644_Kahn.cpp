#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  cin >> n;
  vector<vector<int>> family(n + 1);
  vector<int> indegree(n + 1, 0);
  for(int i = 1; i <= n; i++) {
    while(true) {
      int son;
      cin >> son;
      if(son == 0) break;
      family[i].emplace_back(son);
      indegree[son]++;
    }
  }
  queue<int> q;
  for(int i = 1; i <= n; i++) {
    if(!indegree[i]) {
      q.push(i);
    }
  }
  while(!q.empty()) {
    int cur = q.front();
    q.pop();
    cout << cur << " ";
    for(auto son : family[cur]) {
      indegree[son]--;
      if(indegree[son] == 0) {
        q.emplace(son);
      }
    }
  }
  cout << "\n";
  return 0;
}