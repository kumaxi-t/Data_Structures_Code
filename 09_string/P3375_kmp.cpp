#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string s1;
    string s2;
    cin >> s1 >> s2;
    string s = s2 + "#" + s1;
    vector<int> pi(s.size(), 0);
    vector<int> ans;
    for(int i = 1; i < s.size(); i++) {
        int len = pi[i - 1];
        while(len != 0 && s[i] != s[len]) {
            len = pi[len - 1];
        }
        if(s[i] == s[len]) {
            pi[i] = len + 1;
        }
        if(pi[i] == s2.size()) ans.push_back(i - 2 * s2.size() + 1);
    }
    for(auto x : ans) {
        cout << x << "\n";
    }
    for(int i = 0; i < s2.size(); i++) {
        cout << pi[i] << ' ';
    }
    cout << "\n";
    return 0;
}