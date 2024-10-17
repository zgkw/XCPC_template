template <class E> struct csr {
    vector<int> s;
    vector<E> adj;
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
         
struct SCC {
    int n;
    struct edge {
        int to;
    };
    vector<pair<int, edge>> edges;
    
    SCC(int n) : n(n) {}
 
    void add(int from, int to) { edges.push_back({from, {to}}); }
 
    pair<int, vector<int>> tarjan() {
        auto g = csr<edge>(n, edges);
        int cur = 0, cnt = 0;
        vector<int> vis, low(n), ord(n, -1), ids(n);
        vis.reserve(n);
        auto dfs = [&](auto &&dfs, int v) -> void {
            low[v] = ord[v] = cur++;
            vis.push_back(v);
            for (int i = g.s[v]; i < g.s[v + 1]; i++) {
                auto to = g.adj[i].to;
                if (ord[to] == -1) {
                    dfs(dfs, to);
                    low[v] = min(low[v], low[to]);
                } else {
                    low[v] = min(low[v], ord[to]);
                }
            }
            if (low[v] == ord[v]) {
                while (true) {
                    int u = vis.back();
                    vis.pop_back();
                    ord[u] = n;
                    ids[u] = cnt;
                    if (u == v) break;
                }
                cnt++;
            }
        };
        for (int i = 0; i < n; i++) {
            if (ord[i] == -1) dfs(dfs, i);
        }
        return {cnt, ids};
    }
 
    vector<vector<int>> scc() {
        auto ids = tarjan();
        int cnt = ids.first;
        vector<int> c(cnt);
        for (auto x : ids.second) c[x]++;
        vector<vector<int>> g(ids.first);
        for (int i = 0; i < cnt; i++) {
            g[i].reserve(c[i]);
        }
        for (int i = 0; i < n; i++) {
            g[ids.second[i]].push_back(i);
        }
        return g;
    }
};
     
struct TwoSat {
    int n;
    vector<bool> ans;
    SCC scc;
    
    TwoSat() : n(0), scc(0) {}
    TwoSat(int n) : n(n), ans(n), scc(2 * n) {}
 
    void add(int i, bool f, int j, bool g) {
        scc.add(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
        scc.add(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
    }
    void Not(int u, bool f, int v, bool g) {
        add(u, !f, v, !g) ;
    }
    bool satisfiable() {
        auto id = scc.tarjan().second;
        for (int i = 0; i < n; i++) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
};