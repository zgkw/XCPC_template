constexpr int max_size = 262144000;
uint8_t buf[max_size];
uint8_t *head = buf;

using u32 = uint32_t;

template <class T>
struct u32_p {
    u32 x;
    u32_p(u32 x = 0) : x(x) {}
    T *operator->() {
        return (T *)(buf + x);
    }
    operator bool() {
        return x;
    }
    operator u32() {
        return x;
    }
    bool operator==(u32_p rhs) const {
        return x == rhs.x;
    }
    static u32_p __new() {
        // assert(x < max_size);
        return (head += sizeof(T)) - buf;
    }
};

template<typename Info>
struct persistent_segment_tree {
    int n;
    struct node;
    using Tp = u32_p<node>;
    struct node {
        Info info;
        Tp ch[2];
    };
    Tp _new() {
        Tp t = Tp::__new();
        return t;
    }
    vector<Tp> root;
    persistent_segment_tree(): n(0) {} 
    persistent_segment_tree(int _n, Info _v = Info()) {
        init(std::vector(_n, _v));
    } 
    template<typename T>
    persistent_segment_tree(std::vector<T> _init) {
        _init(_init);
    }
    void pull(Tp &t) {
        t->info.update(t->ch[0]->info, t->ch[1]->info);
    }
    template<typename T>
    void init(const std::vector<T> &_init) {
        n = _init.size();
        root.push_back(_new());
        std::function<void(Tp, int, int)>
        build = [&] (Tp t, int l, int r) {
            if (r - l == 1) {
                t->info = _init[l];
                return;
            }
            int m = (l + r) / 2;
            t->ch[0] = _new(), t->ch[1] = _new();
            build(t->ch[0], l, m), build(t->ch[1], m, r);
            pull(t);
        };
        build(root.back(), 0, n);
    }
    void modify(Tp &t0, Tp &t1, const Info &v, int l, int r, int x) {
        if (r - l == 1) {
            t1->info = t0->info;
            t1->info.apply(v);
            return;
        }
        int m = (l + r) >> 1;
        t1->info.cnt = t0->info.cnt + 1;
        if (m > x) {
            t1->ch[0] = _new();
            t1->ch[1] = t0->ch[1];
            modify(t0->ch[0], t1->ch[0], v, l, m, x);
        } else {
            t1->ch[0] = t0->ch[0];
            t1->ch[1] = _new();
            modify(t0->ch[1], t1->ch[1], v, m, r, x);
        }
        // pull(t1);
    }
    void modify(int x, const Info &v, int from = -1) {
        Tp t0 = (from == -1 ? root.back() : root[from]);
        Tp t1 = _new();
        root.push_back(t1);
        modify(t0, t1, v, 0, n, x);
    }
    Info range_query(Tp &t0, Tp &t1, int l, int r, int x, int y) {
        if (x <= l && r <= y) {
            return Info::del(t1->info, t0->info);
        }
        int m = (l + r) >> 1;
        if (m >= y) {
            return range_query(t0->ch[0], t1->ch[0], l, m, x, y);
        } else if (m <= x) {
            return range_query(t0->ch[1], t1->ch[1], m, r, x, y);
        } else {
            return Info::merge(range_query(t0->ch[0], t1->ch[0], l, m, x, y), range_query(t0->ch[1], t1->ch[1], m, r, x, y));
        }
    }
    Info range_query(int from, int to, int x, int y) {
        return range_query(root[from], root[to], 0, n, x, y);
    }
    int kth(Tp t0, Tp t1, int l, int r, i64 k) {
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) >> 1;
        // d
        i64 lhs = t1->ch[0]->info.cnt - t0->ch[0]->info.cnt;
        if (lhs >= k) {
            return kth(t0->ch[0], t1->ch[0], l, m, k);
        } else {
            return kth(t0->ch[1], t1->ch[1], m, r, k - lhs);
        }
    }
    int kth(int from, int to, i64 k) {
        return kth(root[from], root[to], 0, n, k);
    }
    void show(Tp &t, int l, int r, int dep = 0) {
        if (!t) {
            return;
        }
        int m = (l + r) >> 1;
        show(t->ch[0], l, m, dep + 1);
        for (int i = 1; i <= dep; i += 1) cerr << '\t';
        t->info.show();
        cerr << endl;
        show(t->ch[1], m, r, dep + 1);
    }
    void show(int time) {
        show(root[time],0, n);
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
    void show() {
        cerr << "info: " << cnt << ' ';
    }
};

using SegmentTree = persistent_segment_tree<Info>;