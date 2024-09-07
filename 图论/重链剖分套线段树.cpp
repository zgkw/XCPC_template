template<typename Info, typename Tag>
struct HLD_Seg : public HLD, LazySegmentTree<Info, Tag> {
    using LazySegmentTree<Info, Tag>::rangeApply, LazySegmentTree<Info, Tag>::rangeQuery;
    HLD_Seg(int n) {
        init(n);
    }
    void init(int n) {
        HLD::init(n);
    }
    void Work(int root, const vector<Info> &a) {
        HLD::work(root);
        vector<Info> b(n);
        for (int i = 0; i < n; i += 1) {
            b[in[i]] = a[i];
        }
        LazySegmentTree<Info, Tag>::init(b);
    }
    void LineApply(int u, int v, Tag t) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            rangeApply(in[top[u]], in[u] + 1, t);
            u = parent[top[u]];
        }
        if (in[u] > in[v]) {
            swap(u, v);
        }
        rangeApply(in[u], in[v] + 1, t);
    }
    Info LineQuery(int u, int v) {
        Info ans = Info();
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            ans = Info::merge(ans, rangeQuery(in[top[u]], in[u] + 1));
            u = parent[top[u]];
        }
        if (in[u] > in[v]) {
            swap(u, v);
        }
        ans = Info::merge(ans, rangeQuery(in[u], in[v] + 1));
        return ans;
    }
    void SubApply(int u, Tag t, int r = 0) {
        if (u == r) {
            rangeApply(0, n, t);
        } else if (isAncester(u, r)) {
            if (top[u] == top[r]) {
                r = seq[in[u] + 1];
            } else {
                while (top[parent[top[r]]] != top[u]) {
                    r = parent[top[r]];
                }
                r = top[r];
                if (parent[r] != u) {
                    r = seq[in[u] + 1];
                }
            }
            rangeApply(0, in[r], t);
            rangeApply(out[r], n, t);
        } else {
            rangeApply(in[u], out[u], t);
        }
    }
    Info SubQuery(int u, int r = 0) {
        Info ans = Info();
        if (u == r) {
            return ans = rangeQuery(0, n);
        } else if (isAncester(u, r)) {
            if (top[u] == top[r]) {
                r = seq[in[u] + 1];
            } else {
                while (top[parent[top[r]]] != top[u]) {
                    r = parent[top[r]];
                }
                r = top[r];
                if (parent[r] != u) {
                    r = seq[in[u] + 1];
                }
            }
            ans = Info::merge(rangeQuery(0, in[r]), rangeQuery(out[r], n));
        } else {
            return ans = rangeQuery(in[u], out[u]);
        }
        return ans;
    }
};