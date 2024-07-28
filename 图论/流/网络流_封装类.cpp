template<class T>
struct MaxFlow {
    int n;
    vector<int> r, t, to, h, cur;
    vector<T> c;
    MaxFlow(int n, int m = 0) {
        init(n, m);
    }
    void init(int n, int m = 0) {
        this->n = n;
        r.assign(n, -1);
        h.assign(n, -1);
        cur.assign(n, 0);
        t.reserve(2 * m);
        to.reserve(2 * m);
        c.reserve(2 * m);
    }
    void addEdge(int u, int v, T c) {
        t.push_back(r[u]), to.push_back(v), c.push_back(c), r[u] = t.size() - 1;
        t.push_back(r[v]), to.push_back(u), c.push_back(0), r[v] = t.size() - 1;
    }
    bool bfs(int s, int t) {
        fill(h.begin(), h.end(), -1);
        queue<int> q;
        h[s] = 0;
        cur[s] = r[s];
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = r[u]; ~i; i = t[i]) {
                int v = to[i];
                T c = c[i];
                if (c > 0 && h[v] == -1) {
                    h[v] = h[u] + 1;
                    cur[v] = r[v];
                    if (v == t) {
                        return true;
                    }
                    q.push(v);
                }
            }
        }
        return false;
    }
    T dfs(int u, int t, T f) {
        if (u == t) {
            return f;
        }
        T r = f;
        for (int &i = cur[u]; ~i; i = t[i]) {
            int v = to[i];
            T c = c[i];
            if (c > 0 && h[v] == h[u] + 1) {
                T k = dfs(v, t, min(c, r));
                if (k == 0) {
                    h[v] = -1;
                }
                c[i] -= k;
                c[i ^ 1] += k;
                r -= k;
                if (r == 0) {
                    return f;
                }
            }
        }
        return f - r;
    }
    T flow(int s, int t) {
        T ans = 0;
        while (bfs(s, t)) {
            ans += dfs(s, t, std::numeric_limits<T>::max());
        }
        return ans;
    }

    std::vector<bool> minCut() {
        std::vector<bool> c(n);
        for (int i = 0; i < n; i++) {
            c[i] = (h[i] != -1);
        }
        return c;
    }

    struct Edge {
        int from;
        int to;
        T cap;
        T flow;
        friend ostream &operator<<(ostream &cout, Edge u) {
            return cout << '{' << u.from << ", " << u.to << ", " << u.cap << ", " << u.flow << "}";
        }
    };
    vector<Edge> edges() {
        vector<Edge> a;
        for (int i = 0; i < t.size(); i += 2) {
            Edge x;
            x.from = to[i + 1];
            x.to = to[i];
            x.cap = c[i] + c[i + 1];
            x.flow = c[i + 1];
            a.push_back(x);
        }
        return a;
    }
};