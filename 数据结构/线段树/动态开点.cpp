/**
 * 262144000
**/
constexpr int max_size = 262144000;
uint8_t buf[max_size];
uint8_t *head = buf;

using Tp = long long;
template<typename Info, typename Tag>
struct segment_tree {
    int n;
    struct node {
        Info info;
        Tag tag;
        array<int, 2> _ch;
        node(): info(), tag(), _ch{} {}
        node *ch(int x) const {
            return (node *)(_ch[x] + buf);
        }
        void clear() {
            *this = node();
        }
    };
    using p_Tp = node *;
    int root{0};
    int _new(Tp l, Tp r) {
        int cur = (head += sizeof(node)) - buf;
        p_Tp p = p_Tp(buf + cur);
        // p->info = Info::merge(l, r);
        assert(cur < max_size);
        return cur;
    }
    void apply(int &cur, const Tag &v, Tp l, Tp r) {
        if (!cur) {
            cur = _new(l, r);
        }
        p_Tp p = p_Tp(buf + cur);
        p->info.apply(v, l, r);
        p->tag.apply(v);
    }
    void push(int &cur, Tp l, Tp m, Tp r) {
        p_Tp p = p_Tp(buf + cur);
        // assert(l < r);
        if (!bool(p->tag))
            return;
        apply(p->_ch[0], p->tag, l, m);
        apply(p->_ch[1], p->tag, m, r);
        p->tag.clear();
    }
    void pull(int &cur, Tp l, Tp m, Tp r) {
        p_Tp p = p_Tp(buf + cur);
        p->info.update(p->ch(0)->info, p->ch(1)->info, l, m, r);
    }
    Tp floor, ceil;
    segment_tree(Tp floor, Tp ceil) : floor(floor) , ceil(ceil) {}
    void modify(int &cur, const Tag &v, Tp l, Tp r, Tp x) {
        if (!cur)
            cur = _new(l, r);
        p_Tp p = p_Tp(buf + cur);
        Tp m = (l + r) >> 1;
        if (r - l == 1) {
            p->info.apply(v, l, r);
            return;
        }
        // push(cur, l, m, r);
        if (m > x)
            modify(p->_ch[0], v, l, m, x);
        else
            modify(p->_ch[1], v, m, r, x);
        pull(cur, l, m, r);
    }
    void modify(Tp x, const Tag &v) {
        modify(root, v, floor, ceil, x);
    }
    void rangeApply(int &cur, const Tag &v, Tp l, Tp r, Tp x, Tp y) {
        if (!cur)
            cur = _new(l, r);
        p_Tp p = p_Tp(buf + cur);
        Tp m = (l + r) >> 1;
        if (x <= l && r <= y) {
            apply(cur, v, l, r);
            return;
        }
        push(cur, l, m, r);
        if (m > x)
            rangeApply(p->_ch[0], v, l, m, x, y);
        if (m < y)
            rangeApply(p->_ch[1], v, m, r, x, y);
        pull(cur, l, m, r);
    }
    void rangeApply(Tp x, Tp y, const Tag &v) {
        if (x >= y) return;
        rangeApply(root, v, floor, ceil, x, y);
    }
    Info Query(int &cur, Tp l, Tp r, Tp x) {
        if (!cur)
            return Info::merge(l, r);
        p_Tp p = p_Tp(buf + cur);
        Tp m = (l + r) >> 1;
        if (r - l == 1) {
            return p->info;
        }
        // push(cur, l, m, r);
        if (m > x)
            return Query(p->_ch[0], l, m, x);
        else
            return Query(p->_ch[1], m, r, x);
    }
    Info Query(Tp x) {
        return Query(root, floor, ceil, x);
    }
    Info rangeQuery(int &cur, Tp l, Tp r, Tp x, Tp y) {
        if (!cur)
            return Info::merge(l, r);
        p_Tp p = p_Tp(buf + cur);
        Tp m = (l + r) >> 1;
        if (x <= l && r <= y) {
            return p->info;
        }
        push(cur, l, m, r);
        if (m >= y) {
            return rangeQuery(p->_ch[0], l, m, x, y);
        } else if (m <= x) {
            return rangeQuery(p->_ch[1], m, r, x, y);
        } else {
            return Info::merge(rangeQuery(p->_ch[0], l, m, x, y), rangeQuery(p->_ch[1], m, r, x, y), l, m, r);
        }
    }
    Info rangeQuery(Tp x, Tp y) {
        return rangeQuery(root, floor, ceil, x, y);
    }
    double BS(int &cur, Tp l, Tp r, i64 k) {
        if (!cur) cur = _new(l, r);

        p_Tp p = p_Tp(buf + cur);

        // debug(l, r, k, p->info);

        if (r - l == 1) {
            assert(p->info != 0);
            // if (p->info == 0) exit(0);
            return l + 1. * k / p->info;
        }

        Tp m = (l + r) >> 1;
        push(cur, l, m, r);

        if (p->ch(0)->info >= k)
            return BS(p->_ch[0], l, m, k);
        else 
            return BS(p->_ch[1], m, r, k - p->ch(0)->info);
    }
    double BS(i64 k) {
        return BS(root, floor, ceil, k);
    }
    void show(int &cur, Tp l, Tp r, Tp x, Tp y, int dep = 0) {
        if (l >= y || r <= x || !cur) return;
        p_Tp p = p_Tp(buf + cur);
        Tp m = (l + r) >> 1;
        if (r - l > 1)
        show(p->_ch[0], l, m, x, y, dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        cerr << l << ' ' << r << ' '; p->info.show(), p->tag.show();
        cerr << '\n';
        if (r - l > 1)
        show(p->_ch[1], m, r, x, y, dep + 1);
    }
    void show(Tp x, Tp y) {
        show(root, floor, ceil, x, y);
    }
    p_Tp p_Tp_root() { return p_Tp(buf + root); }
};

struct Tag {
    int x = 0;
    void apply(const Tag &rhs) {
        x += rhs.x;
    }
    operator bool() {
        return x != 0;
    }
    void clear() {
        x = 0;
    }
    void show() const {
# ifdef LOCAL
        cerr << "Tag: " << x;
# endif
    }
};

struct Info {
    i64 x = 0;
    operator i64() {
        return x;
    }

    void apply(const Tag &rhs, Tp l, Tp r) {
        x += rhs.x * (r - l);
    }
    void update(const Info &lhs, const Info &rhs, Tp l, Tp m, Tp r) {
        x = lhs.x + rhs.x;
    }
    static Info merge(const Info &lhs, const Info &rhs, Tp l, Tp m, Tp r) {
        Info info = Info();
        info.update(lhs, rhs, l, m, r);
        return info;
    }
    static Info merge(Tp l, Tp r) {
        return {0};
    }
    void show() const {
# ifdef LOCAL
        cerr << "Info: " << x << ' ';
# endif
    }
};

using SegmentTree = segment_tree<Info, Tag>;