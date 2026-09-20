#include <bits/stdc++.h>
using namespace std;
struct Edge{
    int u;
    int v;
    int w;
    bool operator<(const Edge& other) {
        return w < other.w;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    int fa[5010];
    auto find = [&](this auto& find, int x) -> int{
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    };
    for(int i = 1; i <= n; i++) fa[i] = i;
    vector<Edge> graph;
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph.emplace_back(u, v, w);
        
    }
    sort(graph.begin(), graph.end());
    int sum = 0;
    int done = 0;
    for(auto[u, v, w] : graph) {
        int fu = find(u);
        int fv = find(v);
        if(fu == fv) continue;
        fa[fu] = fv;
        sum += w;
        done++;
        if(done == n - 1) break;
    }
    if(done == n - 1) cout << sum << "\n";
    else cout << "orz" << "\n";

    return 0;
}