# include <bits/stdc++.h>
using namespace std;

# ifdef LOCAL
    # include "C:\Users\Kevin\Desktop\demo\save\debug.h"
# else
# define debug(...) 114514
# define ps 114514
# endif

using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;

struct HLD {
    int n;
    std::vector<int> siz, top, dep, parent, in, out, seq;
    std::vector<std::vector<int>> adj;
    int cur;
    
    HLD() {}
    HLD(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        siz.resize(n);
        top.resize(n);
        dep.resize(n);
        parent.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        cur = 0;
        adj.assign(n, {});
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void work(int root = 0) {
        top[root] = root;
        dep[root] = 0;
        parent[root] = -1;
        dfs1(root);
        dfs2(root);
    }
    void dfs1(int u) {
        if (parent[u] != -1) {
            adj[u].erase(std::find(adj[u].begin(), adj[u].end(), parent[u]));
        }
        
        siz[u] = 1;
        for (auto &v : adj[u]) {
            parent[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[adj[u][0]]) {
                std::swap(v, adj[u][0]);
            }
        }
    }
    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u;
        for (auto v : adj[u]) {
            top[v] = v == adj[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }
    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = parent[top[u]];
            } else {
                v = parent[top[v]];
            }
        }
        return dep[u] < dep[v] ? u : v;
    }
    
    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
    
    int jump(int u, int k) {
        if (dep[u] < k) {
            return -1;
        }
        
        int d = dep[u] - k;
        
        while (dep[top[u]] > d) {
            u = parent[top[u]];
        }
        
        return seq[in[u] - dep[u] + d];
    }
    
    bool isAncester(int u, int v) {
        return in[u] <= in[v] && in[v] < out[u];
    }
    
    int rootedParent(int u, int v) {
        std::swap(u, v);
        if (u == v) {
            return u;
        }
        if (!isAncester(u, v)) {
            return parent[u];
        }
        auto it = std::upper_bound(adj[u].begin(), adj[u].end(), v, [&](int x, int y) {
            return in[x] < in[y];
        }) - 1;
        return *it;
    }
    
    int rootedSize(int u, int v) {
        if (u == v) {
            return n;
        }
        if (!isAncester(v, u)) {
            return siz[v];
        }
        return n - siz[rootedParent(u, v)];
    }
    
    int rootedLca(int a, int b, int c) {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }
};

template<class Info, class Tag>
struct LazySegmentTree {
    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }
    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(n * 4, Info());
        tag.assign(n * 4, Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p, l, m, r);
        };
        build(1, 0, n);
    }
    void pull(int p, int l, int m, int r) {
        info[p].update(info[2 * p], info[2 * p + 1], l, m, r);
    }
    void apply(int p, const Tag &v, int l, int r) {
        info[p].apply(v, l, r);
        tag[p].apply(v);
    }
    void push(int p, int l, int m, int r) {
        if (bool(tag[p])) {
            apply(2 * p, tag[p], l, m);
            apply(2 * p + 1, tag[p], m, r);
            tag[p] = Tag();
        }
    }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p, l, m, r);
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p, l, m, r);
    }
    void modify(int p, const Info &v) {
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p, l, m, r);
        if (m >= y) {
            return rangeQuery(2 * p, l, m, x, y);
        } else if (m <= x) {
            return rangeQuery(2 * p + 1, m, r, x, y);
        } else {
            return Info::merge(rangeQuery(2 * p, l, m, x, y), rangeQuery(2 * p + 1, m, r, x, y), l, m, r);
        }
    }
    Info rangeQuery(int l, int r) {
        if (l >= r) return Info();
        return rangeQuery(1, 0, n, l, r);
    }
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        int m = (l + r) / 2;
        if (l >= x && r <= y) {
            apply(p, v, l, r);
            return;
        }
        push(p, l, m, r);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p, l, m, r);
    }
    void rangeApply(int l, int r, const Tag &v) {
        if (l >= r) {
            return;
        }
        return rangeApply(1, 0, n, l, r, v);
    }
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p, l, m, r);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findFirst(int l, int r, F pred) {
        return findFirst(1, 0, n, l, r, pred);
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p, l, m, r);
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findLast(int l, int r, F pred) {
        return findLast(1, 0, n, l, r, pred);
    }
    void show(int p, int l, int r, int x, int y, int dep = 0) {
        if (l >= y || r <= x) return;
        int m = (l + r) >> 1;
        if (r - l > 1)
        show(p * 2, l, m, x, y, dep + 1);
        for (int i = 0; i < dep; i += 1) {
            cerr << '\t';
        }
        cerr << l << ' ' << r << ' '; info[p].show(), tag[p].show();
        cerr << '\n';
        if (r - l > 1)
        show(p * 2 + 1, m, r, x, y, dep + 1);
    }
    void show(int l, int r) {
        show(1, 0, n, l, r);
    }
};

constexpr i64 inf = 1e18;

struct Tag {
    i64 d = 0;
    void apply(Tag t) {
        d += t.d;
    }
    operator bool() {
        return d != 0;
    }
    void show() const {
# ifdef LOCAL
        cerr << "tag: " << d << ";";
# endif
    }
};

constexpr int N = 20;

struct Info {
    i64 v = 0;
    void apply(const Tag &t, int l, int r) {
        v += 1ll * t.d * (r - l);
    }
    void update(const Info &lhs, const Info &rhs, int l, int m, int r) {
        v = lhs.v + rhs.v;
    }   
    static Info merge(const Info &lhs, const Info &rhs, int l, int m, int r) {
        Info info = Info();
        info.update(lhs, rhs, l, m, r);
        return info;
    }
    void show() {
# ifdef LOCAL
        cerr << "info: " << v << "; ";
# endif
    }
};

using lazySegmentTree = LazySegmentTree<Info, Tag>;

void solve () {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i += 1) {
        cin >> a[i];
    }
    HLD g(n);
    for (int i = 0; i < n - 1; i += 1) {
        int u, v;
        cin >> u >> v;
        -- u, -- v;
        g.addEdge(u, v);
    }
    g.work();
    vector<Info> b(n);
    for (int i = 0; i < n; i += 1) {
        b[g.in[i]] = {a[i]};
    }
    for (int i = 0; i < n; i += 1) {
        cerr << b[i].v << ' ';
    }
    lazySegmentTree seg(b);
    int r = 0;
    auto modify = [&] (int u, int v, int c) {
        while (g.top[u] != g.top[v]) {
            if (g.dep[g.top[u]] < g.dep[g.top[v]]) {
                swap(u, v);
            }
            seg.rangeApply(g.in[g.top[u]], g.in[u] + 1, {c});
            u = g.parent[g.top[u]];
        }
        if (g.in[u] > g.in[v]) {
            swap(u, v);
        }
        seg.rangeApply(g.in[u], g.in[v] + 1, {c});
    };
    auto subModify = [&] (int u, int r, int v) {
        if (u == r) {
            seg.rangeApply(0, n, {v});
        } else if (g.isAncester(u, r)) {
            if (g.top[u] == g.top[r]) {
                r = g.seq[g.in[u] + 1];
            } else {
                while (g.top[g.parent[g.top[r]]] != g.top[u]) {
                    r = g.parent[g.top[r]];
                }
                r = g.top[r];
                if (g.parent[r] != u) {
                    r = g.seq[g.in[u] + 1];
                }
            }
            seg.rangeApply(0, g.in[r], {v});
            seg.rangeApply(g.out[r], n, {v});
        } else {
            seg.rangeApply(g.in[u], g.out[u], {v});
        }
    };
    auto subQuery = [&] (int u, int r) {
        i64 ans = 0;
        if (u == r) {
            return ans += seg.rangeQuery(0, n).v;
        } else if (g.isAncester(u, r)) {
            if (g.top[u] == g.top[r]) {
                r = g.seq[g.in[u] + 1];
            } else {
                while (g.top[g.parent[g.top[r]]] != g.top[u]) {
                    r = g.parent[g.top[r]];
                }
                r = g.top[r];
                if (g.parent[r] != u) {
                    r = g.seq[g.in[u] + 1];
                }
            }
            ans += seg.rangeQuery(0, g.in[r]).v + seg.rangeQuery(g.out[r], n).v;
        } else {
            return ans += seg.rangeQuery(g.in[u], g.out[u]).v;
        }
        return ans;
    };
    for (int tt = 1; tt <= m; tt += 1) {
        int o;
        cin >> o;
        switch(o) {
            case 1: {
                int i;
                cin >> i;
                -- i;
                r = i;
                break;
            }
            case 2: {
                int u, v, x;
                cin >> u >> v >> x;
                -- u, -- v;
                subModify(g.rootedLca(u, v, r), r, x);
                break;
            }
            case 3: {
                int u;
                cin >> u;
                -- u;
                cout << subQuery(u, r) << '\n';
                break;
            }
        }
    }
}

// 修一下爆没爆int
// 多测

signed main () {
# ifndef cin 
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
# endif
	i64 _ = 1;
    // cin >> _;
	while (_ --) {
        solve ();
	}
	return 0;
}