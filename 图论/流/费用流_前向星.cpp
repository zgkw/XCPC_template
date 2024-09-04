template<typename T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;

struct MCFGraph {
    struct Edge {
        int v, c, f;
        Edge(int v, int c, int f) : v(v), c(c), f(f) {}
        template<class ostream>
        friend ostream& operator<<(ostream& cout, Edge e) {
            return cout << "{" << e.v << ", " << e.c << ", " << e.f << "}";
        }
    };
    int n;
    vector<Edge> e;
    vector<int> r, t, pre;
    vector<i64> h, dis;
    bool dijkstra(int S, int T) {
        dis.assign(n, numeric_limits<i64>::max());
        pre.assign(n, -1);
        min_heap<pair<i64, int>> q;
        dis[S] = 0;
        q.emplace(0, S);
        while (!q.empty()) {
            i64 d = q.top().first;
            int u = q.top().second;
            q.pop();
            if (dis[u] < d) continue;
            for (int i = r[u]; ~i; i = t[i]) {
                int v = e[i].v;
                int c = e[i].c;
                int f = e[i].f;
                if (c > 0 && dis[v] > d + h[u] - h[v] + f) {
                    dis[v] = d + h[u] - h[v] + f;
                    pre[v] = i;
                    q.emplace(dis[v], v);
                }
            }
        }
        return dis[T] != numeric_limits<i64>::max();
    }
    MCFGraph(int n, int m = 0) : n(n), r(n, -1) {
        t.reserve(2 * m), e.reserve(2 * m);
    }
    void addEdge(int u, int v, int c, int f) {
        // cerr << u << ' ' << v << ' ' << c << '-' << f << '\n';
        t.push_back(r[u]), r[u] = e.size(), e.emplace_back(v, c, f);
        t.push_back(r[v]), r[v] = e.size(), e.emplace_back(u, 0, -f);
    }
    pair<int, i64> flow(int s, int t) {
        int flow = 0;
        i64 cost = 0;
        h.assign(n, 0);
        while (dijkstra(s, t)) {
            for (int i = 0; i < n; ++i) h[i] += dis[i];
            int aug = numeric_limits<int>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].v) aug = min(aug, e[pre[i]].c);
            for (int i = t; i != s; i = e[pre[i] ^ 1].v) {
                e[pre[i]].c -= aug;
                e[pre[i] ^ 1].c += aug;
            }
            flow += aug;
            cost += i64(aug) * h[t];
        }
        return make_pair(flow, cost);
    }

    struct _Edge {
        int from;
        int to;
        int cap;
        int flow;
        int cost;
    };

    std::vector<_Edge> edges() {
        std::vector<_Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            _Edge x;
            x.from = e[i + 1].v;
            x.to = e[i].v;
            x.cap = e[i].c + e[i + 1].c;
            x.flow = e[i + 1].c;
            x.cost = e[i].f;
            a.push_back(x);
        }
        return a;
    }
};

pair<bool, i64> MCFF(vector<array<int, 5>> &e, int n) {
    int N = n + 2;
    int s = N - 2, t = s + 1;
    vector<int> d(n);
    MCFGraph g(N);
    for (auto [u, v, L, U, c] : e) {
        g.addEdge(u, v, U - L, c);
        d[u] -= L;
        d[v] += L;
    }
    for (int i = 0; i < n; i += 1) {
        if (d[i] > 0) {
            g.addEdge(s, i, d[i], 0);
        } else {
            g.addEdge(i, t, -d[i], 0);
        }
    }
    auto [flow, cost] = g.flow(s, t);
    bool ok = 1;
    for (int i = g.r[s]; ~i; i = g.t[i]) {
        ok &= g.e[i].c == 0;
    }
    for (int i = g.r[t]; ~i; i = g.t[i]) {
        ok &= g.e[i ^ 1].c == 0;
    }
    return {ok, cost};
}