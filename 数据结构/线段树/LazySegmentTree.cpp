template<class Tag, class Info>
struct LazySegmenttree {
    int n;
    std::vector <Info> info;
    std::vector <Tag> tag;

    LazySegmenttree() : n(0) {}

    LazySegmenttree(const int &n, const Info &x = Info()) {
        init(n, x);
    }

    template<class T>
    LazySegmenttree(const std::vector <T> &v) {
        init(v);
    }

    void init(int n, const Info &x = Info()) {
        init(std::vector<Info>(n, x));
    }

    template<class T>
    void init(const std::vector <T> &v) {
        n = (int) v.size();
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        std::function<void(int, int, int)> 
        build = [&](int l, int r, int p) {
            if ((r - l) == 1) {
                info[p] = v[l];
                return;
            }
            int mid = (r + l) >> 1;
            build(l, mid, p << 1);
            build(mid, r, p << 1 | 1);
            pull(p);
        };
        build(0, n, 1);
    }

    void apply(int p, const Tag &x) {
        info[p].apply(x);
        tag[p].apply(x);
    }

    void push(int p) {
        apply(p << 1, tag[p]);
        apply(p << 1 | 1, tag[p]);
        tag[p] = Tag();
    }

    void pull(int p) {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    void range_Change(int x, int y, const Tag &tag) {
        std::function<void(int, int, int)> 
        range_Change = [&](int l, int r, int p) {
            if (y <= l || r <= x) return;
            if (x <= l && r <= y) {
                apply(p, tag);
                return;
            }
            int mid = (l + r) >> 1;
            push(p);
            range_Change(l, mid, p << 1);
            range_Change(mid, r, p << 1 | 1);
            pull(p);
        };
        range_Change(0, n, 1);
    }

    Info range_Query(int x, int y) {
        std::function<Info(int, int, int)> 
        range_query = [&](int l, int r, int p) {
            if (y <= l || r <= x) {
                return Info();
            }
            if (x <= l && r <= y) {
                return info[p];
            }
            int mid = (l + r) >> 1;
            push(p);
            return range_query(l, mid, p << 1)
                   + range_query(mid, r, p << 1 | 1);
        };
        return range_query(0, n, 1);
    }

    void show(int x, int y) {
        std::function<void(int, int, int)> 
        show = [&](int l, int r, int p) {
            if (y <= l || r <= x) {
                return;
            }
            if (r - l == 1) {
                info[p].show();
                return;
            }
            int mid = (l + r) >> 1;
            push(p);
            show(l, mid, p << 1);
            show(mid, r, p << 1 | 1);
        };
        show(0, n, 1);
        cerr << endl;
    }

    void show() {
        show(0, n);
    }
};

struct Tag {
    i64 add = 0;

    void apply(const Tag &x) &{
        add += x.add;
    }
};

struct Info {
    i64 val = 0, l = 1;
    void apply(const Tag &x) &{
        val += x.add * l;
    }

    void show() {
        cerr << val << ' ';
    }
};

Info operator+(const Info &a, const Info &b) {
    return {a.val + b.val, a.l + b.l};
}

using SegmentTree = 
    LazySegmenttree<Tag, Info>;