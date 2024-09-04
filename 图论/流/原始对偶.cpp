template<class E>
struct csr {
    vector<int> h;
    vector<E> e;

    csr(int n, const vector<pair<int, E>> &edges)
            : h(n + 1), e(edges.size()) {
        for (auto u : edges)
            h[u.first + 1]++;
        for (int i = 1; i <= n; i++)
            h[i] += h[i - 1];
        auto c = h;
        for (auto u : edges)
            e[c[u.first]++] = u.second;
    }
};


struct MCFGraph {
    MCFGraph() {}

    MCFGraph(int n) : n(n) {}

    void addEdge(int u, int to, int c, i64 p) {
        E.push_back({u, to, c, 0, p});
    }

    struct Edge {
        int u, v;
        int c, f;
        i64 p;
    };

    vector<Edge> Edges() { return E; }
    static constexpr int inf = numeric_limits<int>::max();
    using Ans = pair<int, i64>;

    Ans flow(int s, int t, int f = inf) {
        return slope(s, t, f).back();
    }

    vector<Ans> slope(int s, int t, int f = inf) {
        int m = E.size();
        vector<int> id(m);

        auto g = [&]() {
            vector<int> d(n), rid(m);
            vector<pair<int, _Edge>> elist;
            elist.reserve(2 * m);
            for (int i = 0; i < m; i++) {
                auto e = E[i];
                id[i] = d[e.u]++;
                rid[i] = d[e.v]++;
                elist.push_back({e.u, {e.v, -1, e.c - e.f, e.p}});
                elist.push_back({e.v, {e.u, -1, e.f, -e.p}});
            }
            auto g = csr<_Edge>(n, elist);
            for (int i = 0; i < m; i++) {
                auto e = E[i];
                id[i] += g.h[e.u];
                rid[i] += g.h[e.v];
                g.e[id[i]].rev = rid[i];
                g.e[rid[i]].rev = id[i];
            }
            return g;
        }();

        auto ans = slope(g, s, t, f);

        for (int i = 0; i < m; i++) {
            E[i].f = E[i].c - g.e[id[i]].c;
        }

        return ans;
    }

    int n;
    vector<Edge> E;

    struct _Edge {
        int v, rev;
        int c;
        i64 p;
    };

    vector<Ans> slope(csr<_Edge> &g, int s, int t, int f) {
        vector<array<i64, 2>> d(n);
        vector<int> pre(n), qm;
        vector<bool> vis(n);
        vector<pair<i64, int>> q;
        auto cmp = greater<pair<i64, int>>();
        auto ref = [&]() {
            for (int i = 0; i < n; i++) {
                d[i][1] = numeric_limits<i64>::max();
            }
            fill(vis.begin(), vis.end(), false);
            qm.clear();
            q.clear();

            int r = 0;

            d[s][1] = 0;
            qm.push_back(s);
            while (!qm.empty() || !q.empty()) {
                int v;
                if (!qm.empty()) {
                    v = qm.back();
                    qm.pop_back();
                } else {
                    while (r < q.size()) {
                        r++;
                        push_heap(q.begin(), q.begin() + r, cmp);
                    }
                    v = q.front().second;
                    pop_heap(q.begin(), q.end(), cmp);
                    q.pop_back();
                    r--;
                }
                if (vis[v]) continue;
                vis[v] = true;
                if (v == t) break;
                i64 u = d[v][0], dis = d[v][1];
                for (int i = g.h[v]; i < g.h[v + 1]; i++) {
                    auto e = g.e[i];
                    if (!e.c) continue;
                    i64 p = e.p - d[e.v][0] + u;
                    if (d[e.v][1] - dis > p) {
                        i64 to = dis + p;
                        d[e.v][1] = to;
                        pre[e.v] = e.rev;
                        if (to == dis) {
                            qm.push_back(e.v);
                        } else {
                            q.push_back({to, e.v});
                        }
                    }
                }
            }
            if (!vis[t]) {
                return false;
            }

            for (int v = 0; v < n; v++) {
                if (!vis[v]) continue;
                d[v][0] -= d[t][1] - d[v][1];
            }
            return true;
        };
        int r = 0;
        i64 p = 0, cf = -1;
        vector<Ans> ans(1);
        while (r < f) {
            if (!ref()) break;
            int c = f - r;
            for (int v = t; v != s; v = g.e[pre[v]].v) {
                c = min(c, g.e[g.e[pre[v]].rev].c);
            }
            for (int v = t; v != s; v = g.e[pre[v]].v) {
                auto &e = g.e[pre[v]];
                e.c += c;
                g.e[e.rev].c -= c;
            }
            i64 D = -d[s][0];
            r += c;
            p += c * D;
            if (cf == D) {
                ans.pop_back();
            }
            ans.push_back({r, p});
            cf = D;
        }
        return ans;
    }
};