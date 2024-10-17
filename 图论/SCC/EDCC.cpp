struct EDCC {
    int n, cur = -1, cnt = 0;
    vector<array<int, 2>> e;
    vector<int> h, t;
    vector<int> dfn, low, ans, stk, f;

    EDCC(int n) :n(n), dfn(n, -1), low(n, -1), h(n, -1), f(n, -1) {};

    // in 表示从哪条边下来，如果为根，by为一个极大值
    void tarjan(int u, int in = 1E9) {
        dfn[u] = low[u] = ++cur;
        stk.push_back(u);
        for (int i = h[u]; i != -1; i = t[i]) {
            int v = e[i][1];
            if (i == (in ^ 1)) {
                continue;
            }
            if (dfn[v] == -1) {
                tarjan(v, i);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    ans.push_back(i);
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            int t = cnt ++;
            int v;
            do {
                v = stk.back();
                stk.pop_back();
                f[v] = t;
            } while (v != u);
        }
    }

    void addEdge(int u, int v) {
        t.push_back(h[u]), h[u] = e.size(); e.push_back({u, v});
        t.push_back(h[v]), h[v] = e.size(); e.push_back({v, u});
    }

    vector<int> &answer() {
        for (int i = 0; i < n; i += 1) {
            if (dfn[i] == -1) {
                tarjan(i);
            }
        }
        return ans;
    }
};