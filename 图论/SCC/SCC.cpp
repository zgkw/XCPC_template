struct SCC {
    int n, cnt = 0, cur = 0;
    vector<int> h, t, to;
    vector<int> dfn, low, stk, f;
    vector<bool> in;

    SCC(int n): n(n), h(n, -1), dfn(n, -1), low(n), f(n, -1), in(n, 0) {}

    void tarjan(int u) {
        dfn[u] = low[u] = cur ++;
        stk.push_back(u);
        in[u] = 1;
        for (int i = h[u]; i != -1; i = t[i]) {
            int v = to[i];
            if (dfn[v] == -1) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            int t = cnt ++;
            do {
                in[stk.back()] = 0;
                f[stk.back()] = t;
                stk.pop_back();
            } while (in[u]);
        }
    }

    void addEdge(int u, int v) {
        t.push_back(h[u]), h[u] = to.size(), to.push_back(v);
    }

    vector<vector<int>> Graph() {
        vector<vector<int>> g(cnt);
        for (int u = 0; u < n; u += 1) {
            for (int i = h[u]; i != -1; i = t[i]) {
                int v = to[i];
                if (f[u] != f[v]) {
                    g[f[u]].push_back(f[v]);
                }
            }
        }
        return g;
    }

    vector<array<int, 2>> Edges() {
        vector<array<int, 2>> e;
        for (int u = 0; u < n; u += 1) {
            for (int i = h[u]; i != -1; i = t[i]) {
                int v = to[i];
                if (f[u] != f[v]) {
                    e.push_back({f[u], f[v]});
                }
            }
        }
        sort(e.begin(), e.end());
        e.erase(unique(e.begin(), e.end()), e.end());
        return e;
    }
};