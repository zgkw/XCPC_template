template<class E>
struct csr {
    vector<int> h;
    vector<E> e;

    csr(int n, const vector<pair<int, E>> &edges)
            : h(n + 1), e(edges.size()) {
        for (auto u : edges) {
            h[u.first + 1]++;
        }
        for (int i = 1; i <= n; i++) {
            h[i] += h[i - 1];
        }
        auto c = h;
        for (auto u : edges) {
            e[c[u.first]++] = u.second;
        }
    }
};


template<typename Cap, typename Cost>
struct MCFGraph {
    MCFGraph() {}

    MCFGraph(int n) : n(n) {}

    void addEdge(int u, int to, Cap c, Cost p) {
        E.push_back({u, to, c, 0, p});
    }

    struct Edge {
        int u, v;
        Cap c, f;
        Cost p;
    };

    vector<Edge> Edges() { return E; }
    static constexpr Cap inf = numeric_limits<Cap>::max();
    using Ans = pair<Cap, Cost>;

    Ans flow(int s, int t, Cap f = inf) {
        return slope(s, t, f).back();
    }

    vector<Ans> slope(int s, int t, Cap f = inf) {
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
        Cap c;
        Cost p;
    };

    vector<Ans> slope(csr<_Edge> &g, int s, int t, Cap f) {
        vector<array<Cost, 2>> d(n);
        vector<int> pre(n), qm;
        vector<bool> vis(n);
        vector<pair<Cost, int>> q;
        auto cmp = greater<pair<Cost, int>>();
        auto ref = [&]() {
            for (int i = 0; i < n; i++) {
                d[i][1] = numeric_limits<Cost>::max();
            }
            fill(vis.begin(), vis.end(), false);
            qm.clear();
            q.clear();

            size_t r = 0;

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
                Cost u = d[v][0], dis = d[v][1];
                for (int i = g.h[v]; i < g.h[v + 1]; i++) {
                    auto e = g.e[i];
                    if (!e.c) continue;
                    Cost p = e.p - d[e.v][0] + u;
                    if (d[e.v][1] - dis > p) {
                        Cost to = dis + p;
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
        Cap r = 0;
        Cost p = 0, cf = -1;
        vector<Ans> ans(1);
        while (r < f) {
            if (!ref()) break;
            Cap c = f - r;
            for (int v = t; v != s; v = g.e[pre[v]].v) {
                c = min(c, g.e[g.e[pre[v]].rev].c);
            }
            for (int v = t; v != s; v = g.e[pre[v]].v) {
                auto &e = g.e[pre[v]];
                e.c += c;
                g.e[e.rev].c -= c;
            }
            Cost D = -d[s][0];
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