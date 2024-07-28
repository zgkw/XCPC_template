using T = int;

constexpr int N = 2e5 + 2, M = 2 * N + 1e5;
int head[N], nxt[2 * M], to[2 * M];
T cap[2 * M];
int cur = 0;
int _n = 0;
int h[N], now[N];

void init(int n) {
    fill(head, head + n, -1);
    _n = n;
    cur = -1;
}

void addEdge(int u, int v, T c) {
    nxt[++cur] = head[u], to[cur] = v, cap[cur] = c, head[u] = cur;
    nxt[++cur] = head[v], to[cur] = u, cap[cur] = 0, head[v] = cur;
}


bool bfs(int s, int t) {
    fill(h, h + _n, -1);
    queue<int> q;
    h[s] = 0;
    now[s] = head[s];
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = head[u]; ~i; i = nxt[i]) {
            int v = to[i];
            T c = cap[i];
            if (c > 0 && h[v] == -1) {
                h[v] = h[u] + 1;
                now[v] = head[v];
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
    for (int &i = now[u]; ~i; i = nxt[i]) {
        int v = to[i];
        T c = cap[i];
        if (c > 0 && h[v] == h[u] + 1) {
            T k = dfs(v, t, min(c, r));
            if (k == 0) {
                h[v] = -1;
            }
            cap[i] -= k;
            cap[i ^ 1] += k;
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

struct Edge {
    int from;
    int to;
    T cap;
    T flow;
};

vector<bool> minCut() {
    vector<bool> c(_n);
    for (int i = 0; i < _n; i ++) {
        c[i] = (h[i] != -1);
    }
    return c;
}

vector<Edge> Edges() {
    vector<Edge> a;
    for (int i = 0; i < cur; i += 2) {
        Edge x;
        x.from = to[i + 1];
        x.to = to[i];
        x.cap = cap[i] + cap[i + 1];
        x.flow = cap[i + 1];
        a.push_back(x);
    }
    return a;
}