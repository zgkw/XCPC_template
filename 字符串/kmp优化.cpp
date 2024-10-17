#include <bits/stdc++.h>
using namespace std;
#define IOS ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#define int long long

#ifdef LOCAL
#include "C:\Program Files\DEBUG\debug.h"
#else
#define debug(...) 114514
#define ps 114514
#endif

// 获得nxt数组
auto get_nxt(string s) {  // 记得手动 s = ' ' + s
    int n = (int)s.size() - 1;
    vector<int> nxt(n + 1);
    nxt[1] = 0;
    for (int i = 2; i <= n; i++) {
        nxt[i] = nxt[i - 1];
        while (nxt[i] && s[i] != s[nxt[i] + 1]) {
            if (nxt[i] + 1 < 2 * nxt[nxt[i] + 1]) {
                nxt[i] = (nxt[i] - 1) % (nxt[i] - nxt[nxt[i]]) + 1;
            } else {
                nxt[i] = nxt[nxt[i]];
            }
        }
        nxt[i] += (s[i] == s[nxt[i] + 1]);
    }
    return nxt;
}

// 找到模式串在文本串中所有出现的位置(注意:1-index)
auto find(string t, string p) {  // p模式,t文本
    int n = t.size();
    int m = p.size();
    vector<int> res;
    auto v = get_nxt(' ' + p + '$' + t);
    for (int i = m + 2; i <= n + m + 1; i++) {
        if (v[i] == m) {
            res.push_back(i - 2 * m);
        }
    }
    return res;
}

// 找到模式串在文本串中所有出现的位置(注意:1-index)
auto find(string t, vector<int> nxt, string p) {  // p模式,t文本
    int n = (int)t.size() - 1;
    int m = (int)p.size() - 1;
    vector<int> res;
    int j = 0;
    for (int i = 1; i <= n; i++) {
        while (j && t[i] != p[j + 1])
            j = nxt[j];
        j += (t[i] == p[j + 1]);
        debug(j);
        if (j == m) {
            res.push_back(i - m + 1);
            j = nxt[j];
        }
    }
    return res;
}

signed main() {
    IOS string s1, s2;
    cin >> s1 >> s2;
    auto v = get_nxt(' ' + s2);
    auto res = find(' ' + s1, v, ' ' + s2);
    for (auto x : res) {
        cout << x << endl;
    }
    // auto v = get_nxt(' ' + s2);
    // debug(v);
    for (int i = 1; i <= s2.size(); i++) {
        cout << v[i] << ' ';
    }
    return 0;
}
