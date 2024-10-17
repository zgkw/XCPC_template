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

// static weighted tree
template<int N = int(1E6)>
struct multiTree {
    int n;
    int K;
    vector<array<int, __lg(N) + 1>> p;
    struct Edge {
        int v; i64 w;
        friend ostream &operator<<(ostream &cout, Edge u) {
            return cout << "(" << u.v << ", " << u.w << ")";
        } 
    };
    vector<pair<int, Edge>> e;
    csr<Edge> g;
    vector<int> dep;
    vector<i64> dep2;
    multiTree() {}
    multiTree(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        K = __lg(n) + 1;
        p.assign(n, {});
        dep.assign(n, 0);
        dep2.assign(n, 0);
    }
    void addEdge(int u, int v, auto w) {
        e.push_back({u, {v, w}});
        e.push_back({v, {u, w}});
    }
    void work(int r = 0) {
        g = csr<Edge>(n, e);
        dfs(r, r);
    }
    void dfs(int u, int f) {
        p[u][0] = f;
        for (int i = 1; i < K; i += 1) {
            p[u][i] = p[p[u][i - 1]][i - 1];
        }
        for (int i = g.s[u]; i < g.s[u + 1]; i += 1) {
            auto [v, w] = g.adj[i];
            if (v == f) {
                continue;
            }
            dep[v] = dep[u] + 1;
            dep2[v] = dep2[u] + w;
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
    int dist(int u, int v) {
        int m = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[m];
    }
    int dis2(int u, int v) {
        int m = lca(u, v);
        return dep2[u] + dep2[v] - 2 * dep[m];
    }
};

template<class E>
struct star {
    vector<int> s, t;
    star() {}
    star(int n, const vector<pair<int, E>> &e): s(n, -1) {
        // debug(e);
        t.reserve(e.size());
        for (int i = 0; i < e.size(); i += 1) {
            int u = e[i].first;
            t.push_back(s[u]), s[u] = i;
        }
    }
};

struct RST : public multiTree<> {
    using multiTree<>::operator=; // 导入基类的所有成员
    int n, N;
    struct Edge {
        int v; i64 w;
        friend ostream &operator<<(ostream &cout, Edge u) {
            return cout << "(" << u.v << ", " << u.w << ")";
        } 
    };
    vector<pair<int, Edge>> e;
    
    vector<i64> s, cir;
    RST(int n): n(n), N(n), s(n), cir(n) {}
    void tarjan() {
        star<Edge> g(n, e);
        int cur = 0;
        vector<int> dfn(n, -1), low(n, -1), fe(n, -1);
        auto tarjan = [&] (auto &&tarjan, int u, int E) -> void {
            dfn[u] = low[u] = cur ++;
            for (int i = g.s[u]; i != -1; i = g.t[i]) {
                auto [v, w] = e[i].second;
                if (dfn[v] == -1) {
                    fe[v] = i;
                    tarjan(tarjan, v, i);
                    low[u] = min(low[u], low[v]);
                    if (dfn[u] < low[v]) {
                        addEdge2(u, v, w);
                    }
                } else if (i != (E ^ 1)) {
                    low[u] = min(low[u], low[v]);
                }
            }
            for (int i = g.s[u]; i != -1; i = g.t[i]) {
                auto [v, w] = e[i].second;
                if (dfn[u] < dfn[v] && fe[v] != i) {
                    int t = N ++;
                    i64 sum = w;
                    for (int j = v; j != u; j = e[fe[j]].first) {
                        s[j] = sum;
                        sum += e[fe[j]].second.w;
                    }
                    for (int j = v; j != u; j = e[fe[j]].first) {
                        addEdge2(t, j, std::min(s[j], sum - s[j]));
                        cir[j] = sum;
                    }
                    addEdge2(u, t, 0);
                }
            }
        };
        tarjan(tarjan, 0, 1E9);
        init(N);
        work();
    }
    void addEdge(int u, int v, i64 w) {
        e.push_back({u, {v, w}});
        e.push_back({v, {u, w}});
    }
    void addEdge2(int u, int v, i64 w) {
        multiTree::addEdge(u, v, w);
    }
    i64 dis(int u, int v) {
        int m = lca(u, v);
        if (m < n) {
            return dep2[u] + dep2[v] - 2 * dep2[m];
        }
        int d = dep[m];
        int x = jump(u, d + 1), y = jump(v, d + 1);
        i64 delta = std::abs(s[x] - s[y]);
        return dep2[u] + dep2[v] - dep2[x] - dep2[y] + std::min(delta, cir[x] - delta);
    }
};