#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  string s1;      // 模式串
  string s2;      // 子串
  cin >> s1 >> s2;
  string s = s1 + '#' + s2;
  vector<int> pi(s.size(), 0);
  for(int i = 1; i < s.size(); i++) {
    int len = pi[i - 1];
    while(len != 0 && s[i] != s[len]) {
      len = pi[len - 1];
    }
    if(s[i] == s[len]) {
      pi[i] = len + 1;
    }
  }

  for(auto x : pi) {
    cout << x << ' ';
  }
  cout << "\n";


  return 0;
}