template<class Info>
struct SegmentTree {
    int n;
    std::vector<Info> info;
    SegmentTree() : n(0) {}
    SegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    template<class T>
    SegmentTree(std::vector<T> init_) {
        init(init_);
    }
    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << std::__lg(n), Info());
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
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p].apply(v, l, r);
            return;
        }
        int m = (l + r) / 2;
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p, l, m, r);
    }
    void modify(int p, const Info &v) {
        if(p >= n) return;
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        if (m >= y) {
            return rangeQuery(2 * p, l, m, x, y);
        } else if (m <= x) {
            return rangeQuery(2 * p + 1, m, r, x, y);
        } else {
            return Info::merge(rangeQuery(2 * p, l, m, x, y), rangeQuery(2 * p + 1, m, r, x, y), std::max(l, x), m, std::min(r, y));
        }
    }
    Info rangeQuery(int l, int r) {
        if (l >= r) return Info();
        return rangeQuery(1, 0, n, l, r);
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
        cerr << l << ' ' << r << ' '; info[p].show();
        cerr << '\n';
        if (r - l > 1)
        show(p * 2 + 1, m, r, x, y, dep + 1);
    }
    void show(int l, int r) {
        show(1, 0, n, l, r);
    }
};

struct Info {
    void apply(const Info &rhs, int l, int r) {}
    void update(const Info &lhs, const Info &rhs, int l, int m, int r) {}
    static Info merge(const Info &lhs, const Info &rhs, int l, int m, int r) {
        Info info = Info();
        info.update(lhs, rhs, l, m, r);
        return info;
    }
    void show() const {
        cerr << "info: ";
    }
};

using Tree = SegmentTree<Info>;