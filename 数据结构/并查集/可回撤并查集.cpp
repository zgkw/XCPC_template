struct DSU {
    vector<int> fa, siz;
    vector<array<int, 4>> h;
    vector<i64> lazy;

    DSU() {}

    DSU(int n) {
        init(n);
    }

    void init(int n) {
        fa.resize(n);
        iota(fa.begin(), fa.end(), 0);
        siz.assign(n, 1);
        lazy.assign(n, 0);
    }

    int find(int x) {
        while (x != fa[x]) {
            x = fa[x];
        }
        return x;
    }

    int size(int x) {
        return siz[find(x)];
    }

    bool same(int u, int v) {
        return find(u) == find(v);
    }

    void merge(int u, int v) {
        int x = find(u);
        int y = find(v);
        if (x == y) return;
        if (siz[x] < siz[y]) std::swap(x, y);
        h.push_back({x, y, siz[x], fa[y]});
        siz[x] = siz[x] + siz[y];
        fa[y] = x;
        int p = y;
        lazy[y] -= lazy[x];
    }

    int clock() {
        return h.size();
    }

    void roll(int to) {
        while (h.size() > to) {
            auto [u, v, sizu, fav] = h.back();
            siz[u] = sizu;
            fa[v] = fav;
            h.pop_back();
            lazy[v] += lazy[u];
        }
    }
};