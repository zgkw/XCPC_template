template<typename T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;

template<typename T>
struct Ceil {
    constexpr static T max() {
        return numeric_limits<T>::max();
    }
};

using f64 = double;
template<>
struct Ceil<f64> {
    constexpr static f64 max() {
        return 1e9;
    }
};

template<typename Cap, typename Cost>
struct MCFGraph {
    struct Edge {
        int v; Cap c; Cost f;
        Edge(int v, Cap c, Cost f) : v(v), c(c), f(f) {}
        template<class ostream>
        friend ostream& operator<<(ostream& cout, Edge e) {
            return cout << "{" << e.v << ", " << e.c << ", " << e.f << "}";
        }
    };
    int n;
    vector<Edge> e;
    vector<int> r, t, pre;
    vector<Cost> h, dis;
    bool dijkstra(int S, int T) {
        dis.assign(n, Ceil<Cost>::max());
        pre.assign(n, -1);
        min_heap<pair<Cost, Cap>> q;
        dis[S] = 0;
        q.emplace(0, S);
        while (!q.empty()) {
            Cost d = q.top().first;
            Cap u = q.top().second;
            q.pop();
            if (dis[u] < d) continue;
            for (int i = r[u]; ~i; i = t[i]) {
                int v = e[i].v;
                Cap c = e[i].c;
                Cost f = e[i].f;
                if (c > 0 && dis[v] > d + h[u] - h[v] + f) {
                    dis[v] = d + h[u] - h[v] + f;
                    pre[v] = i;
                    q.emplace(dis[v], v);
                }
            }
        }
        return dis[T] != Ceil<Cost>::max();
    }
    MCFGraph(int n, int m = 0) : n(n), r(n, -1) {
        t.reserve(2 * m), e.reserve(2 * m);
    }
    void addEdge(int u, int v, Cap c, Cost f) {
        // cerr << u << ' ' << v << ' ' << c << '-' << f << '\n';
        t.push_back(r[u]), r[u] = e.size(), e.emplace_back(v, c, f);
        t.push_back(r[v]), r[v] = e.size(), e.emplace_back(u, 0, -f);
    }
    pair<Cap, Cost> flow(int s, int t) {
        Cap flow = 0;
        Cost cost = 0;
        h.assign(n, 0);
        while (dijkstra(s, t)) {
            for (int i = 0; i < n; ++i) h[i] += dis[i];
            Cap aug = Ceil<Cap>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].v) aug = min(aug, e[pre[i]].c);
            for (int i = t; i != s; i = e[pre[i] ^ 1].v) {
                e[pre[i]].c -= aug;
                e[pre[i] ^ 1].c += aug;
            }
            flow += aug;
            cost += aug * h[t];
        }
        return make_pair(flow, cost);
    }

    struct _Edge {
        int from;
        int to;
        Cap cap;
        Cap flow;
        Cost cost;
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