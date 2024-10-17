template<typename Info>
struct segment_tree {
    int n;
    struct node;
    using Tp = Base<node>;
    struct node {
        Info info;
        Tp ch[2];
    };
    Tp news() {
        Tp t = Tp::news();
        return t;
    }
    segment_tree(): n(0) {} 
    segment_tree(int n, Info v = Info()) {
        init(std::vector(n, v));
    } 
    template<typename T>
    segment_tree(std::vector<T> _init) {
        init(_init);
    }
    void pull(Tp &t) {
        t->info.update(t->ch[0]->info, t->ch[1]->info);
    }
    template<typename T>
    void init(const std::vector<T> &_init) {
        n = _init.size();
        auto build = [&] (auto &&self, Tp &t, int l, int r) {
            t = news();
            if (r - l == 1) {
                t->info = _init[l];
                return;
            }
            int m = (l + r) / 2;
            self(self, t->ch[0], l, m), self(self, t->ch[1], m, r);
            pull(t);
        };
        build(t, 0, n);
    }
    Tp &modify(Tp &u, const Info &M, int l, int r, int x) {
        Tp v = news();
        if (r - l == 1) {
            v->info = u->info;
            v->info.apply(M);
            return v;
        }
        int m = (l + r) / 2;
        if (m > x) {
            v->ch[1] = u->ch[1];
            v->ch[0] = modify(u->ch[0], M, l, m, x);
        } else {
            v->ch[0] = u->ch[0];
            v->ch[1] = modify(u->ch[1], M, m, r, x);
        }
        pull(v);
    }
    Tp &modify(Tp &t, int x, const Info &M) {
        modify(t, M, 0, n, x);
    }
    Info range_query(Tp u, Tp v, int l, int r, int x, int y) {
        if (x <= l && r <= y) {
            return Info::del(v->info, u->info);
        }
        int m = (l + r) >> 1;
        if (m >= y) {
            return range_query(u->ch[0], v->ch[0], l, m, x, y);
        } else if (m <= x) {
            return range_query(u->ch[1], v->ch[1], m, r, x, y);
        } else {
            return Info::merge(range_query(u->ch[0], v->ch[0], l, m, x, y), range_query(u->ch[1], v->ch[1], m, r, x, y));
        }
    }
    Info range_query(Tp u, Tp v, int x, int y) {
        return range_query(u, v, 0, n, x, y);
    }
    int kth(Tp u, Tp v, int l, int r, i64 k) {
        i64 x = Info::del(u->info, v->info);
        if (x < k) {
            k -= x;
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        int res = kth(u->ch[0], v->ch[0], l, m, k);
        if (res == -1) {
            res = kth(u->ch[1], v->ch[1], m, r, k);
        }
        return res;
    }
    int kth(Tp u, Tp v, i64 k) {
        return kth(u, v, 0, n, k);
    }
    void DFS(Tp t, int l, int r, int dep = 0) {
        if (!t) {
            return;
        }
        int m = (l + r) / 2;
        DFS(t->ch[0], l, m, dep + 1);
        cerr << string(dep, '\t');
        cerr << t->info << endl;
        DFS(t->ch[1], m, r, dep + 1);
    }
    void dfs(Tp t) {
        DFS(t, 0, n);
    }
};

struct Info {
    i64 cnt = 0;
    void apply(const Info &v) {
        cnt += v.cnt;
    }
    void update(const Info &lhs, const Info &rhs) {
        cnt = lhs.cnt + rhs.cnt;
    }
    Info del(const Info &lhs, const Info &rhs) {
        return {lhs.cnt - rhs.cnt};
    }
    Info merge(const Info &lhs, const Info &rhs) {
        Info info = Info();
        info.update(lhs, rhs);
        return info;
    }
    friend ostream &operator<<(ostream &cout, Info t) {
        return cout << "Info" << "; ";
    }
};

using SegmentTree = segment_tree<Info>;