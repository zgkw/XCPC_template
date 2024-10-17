struct CutPoint {
    int n, cur = 0;
    vector<int> h, t, to, dfn, low;
    vector<bool> cut;

    CutPoint(int n): n(n), h(n, -1), dfn(n, -1), low(n, -1), cut(n) {};

    // 传参时 tarjan(u, u)
    void tarjan(int u, int r) {
        dfn[u] = low[u] = cur ++;
        int child = 0;
        for (int i = h[u]; i != -1; i = t[i]) { 
            int v = to[i];
            if (dfn[v] == -1) {
                tarjan(v, r);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    ++child;
                    if (u != r || child > 1) {
                        cut[u] = true;
                    }
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            } 
        }
    }

    void addEdge(int u, int v) {
        t.push_back(h[u]), h[u] = to.size(); to.push_back(v);
        t.push_back(h[v]), h[v] = to.size(); to.push_back(u);
    }
};