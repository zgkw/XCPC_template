auto get_nxt(string s) {
    int n = (int)s.size() - 1;
    vector<vector<int>> nxt(n + 2, vector<int>(26, n + 1));
    for (int i = n; i >= 0; i--) {
        for (int j = 0; j < 26; j++) {
            if (i == n)
                nxt[i][j] = n + 1;
            else
                nxt[i][j] = nxt[i + 1][j];
        }
        if (i != n)
            nxt[i][s[i + 1] - 'a'] = i + 1;
    }
    return nxt;
}

auto jump(string s, vector<vector<int>> &nxt) {
    int now = 0;
    for (int i = 0; i < s.size(); i++) {
        now = nxt[now][s[i] - 'a'];
    }
    return now;
}
