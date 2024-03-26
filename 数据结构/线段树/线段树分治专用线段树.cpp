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
        };
        build(1, 0, n);
    }
    void rangeChange(int x, int y, const Info &tag) {
        std::function<void(int, int, int, int, int, const Info&)> rangeChange = [&] (int p, int l, int r, int x, int y, const Info &tag) {
            if (l >= y || r <= x) {
                return;
            }
            if (l >= x && r <= y) {
                info[p].apply(tag);
                return;
            }
            int m = (l + r) / 2;
            rangeChange(p << 1, l, m, x, y, tag);
            rangeChange(p << 1 | 1, m, r, x, y, tag);
        };
        rangeChange(1, 0, n, x, y, tag);
    }
};

struct Info {
    vector<array<ll, 2>> x;
    void apply(const Info& tag) {
        for (auto u : tag.x) {
            x.push_back(u);
        }
    }
};

using Segmenttree = SegmentTree<Info>;