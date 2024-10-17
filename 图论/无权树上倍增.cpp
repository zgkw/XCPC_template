template <class E> 
struct csr {
    vector<int> s;
    vector<E> adj;
    csr() {}
    csr(int n, const vector<pair<int, E>>& g)
        : s(n + 1), adj(g.size()) {
        for (auto e : g) {
            s[e.first + 1]++;
        }
        for (int i = 1; i <= n; i++) {
            s[i] += s[i - 1];
        }
        auto c = s;
        for (auto e : g) {
            adj[c[e.first]++] = e.second;
        }
    }
};

// static unweighted tree
template<int N>
struct multiTree {
    int n;
    int K;
    vector<array<int, __lg(N) + 1>> p;
    struct Edge {
        int v; i64 w;
    };
    vector<pair<int, int>> e;
    csr<int> g;
    vector<int> dep;
    multiTree(int n): n(n), dep(n), p(n), K(__lg(n) + 1) {}
    void addEdge(int u, int v) {
        e.push_back({u, v});
        e.push_back({v, u});
    }
    void work(int r = 0) {
        g = csr<int>(n, e);
        dfs(r, r);
    }
    void dfs(int u, int f) {
        p[u][0] = f;
        for (int i = 1; i < K; i += 1) {
            p[u][i] = p[p[u][i - 1]][i - 1];
        }
        for (int i = g.s[u]; i < g.s[u + 1]; i += 1) {
            auto v = g.adj[i];
            if (v == f) {
                continue;
            }
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }
    int jump(int u, int d) {
        for (int i = dep[u] - d; i > 0; i -= 1 << __lg(i)) {
            u = p[u][__lg(i)];
        }
        return u;
    }
    int lca(int u, int v) {
        if (dep[u] < dep[v]) {
            swap(u, v);
        }
        u = jump(u, dep[v]);
        if (u == v) {
            return u;
        }
        for (int i = __lg(dep[u]); i >= 0; i -= 1) {
            if (p[u][i] != p[v][i]) {
                u = p[u][i], v = p[v][i];
            }
        }
        return p[u][0];
    }
    int dis1(int u, int v) {
        int m = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[m];
    }
};