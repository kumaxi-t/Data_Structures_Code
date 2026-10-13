#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  string p;       // 主串
  string tar;     // 模式串
  cin >> p >> tar;
  // 考研所求的next数组是把原pi数组向右移一位再加一，同时第一位保持0不变
  // 以下所求的next数组实则为pi左移一位，为其前一位的最长匹配真前后缀
  vector<int> next(tar.size(), 0);
  for(int i = 2; i < tar.size(); i++) {
    int len = next[i - 1];
    while(len != 0 && tar[i - 1] != tar[len]) {
      len = next[len];
    }
    if(tar[i- 1] == tar[len]) {
      next[i] = len + 1;
    }
  }
  // for(auto x : next) cout << x << ' ';
  int tar_cur = 0;
  int p_cur = 0;
  while(p_cur < p.size()) {
    if(p[p_cur] == tar[tar_cur]) {
      p_cur++;
      tar_cur++;
      if(tar_cur == tar.size()) {
        cout << p_cur - tar.size() << "\n";
        tar_cur = next[tar_cur];
        continue;
      }
    } else if(tar_cur != 0) {
      tar_cur = next[tar_cur];
    } else {
      p_cur++;
    }
  }

  return 0;
}