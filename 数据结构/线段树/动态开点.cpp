/**
 * 262144000
**/
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


using ix = long long;
template<typename Info, typename Tag>
struct segment_tree {
    int n;
    struct node;
    using Tp = u32_p<node>;
    
    struct node {
        Info info;
        Tag tag;
        Tp ch[2];
    };
    Tp root{0};
    Tp _new(ix l, ix r) {
        Tp t = Tp::__new();
        return t;
    }
    void apply(Tp &t, const Tag &v, ix l, ix r) {
        if (!t) {
            t = _new(l, r);
        }
        t->info.apply(v, l, r);
        t->tag.apply(v);
    }
    void push(Tp &t, ix l, ix m, ix r) {
        // assert(l < r);
        if (!bool(t->tag))
            return;
        apply(t->ch[0], t->tag, l, m);
        apply(t->ch[1], t->tag, m, r);
        t->tag = Tag();
    }
    void pull(Tp &t, ix l, ix m, ix r) {
        t->info.update(t->ch[0]->info, t->ch[1]->info, l, m, r);
    }
    ix floor, ceil;
    segment_tree(ix floor, ix ceil) : floor(floor) , ceil(ceil) {}
    void modify(Tp &t, const Tag &v, ix l, ix r, ix x) {
        if (!t)
            t = _new(l, r);
        ix m = (l + r) >> 1;
        if (r - l == 1) {
            t->info.apply(v, l, r);
            return;
        }
        push(t, l, m, r);
        if (m > x)
            modify(t->ch[0], v, l, m, x);
        else
            modify(t->ch[1], v, m, r, x);
        pull(t, l, m, r);
    }
    void modify(ix x, const Tag &v) {
        modify(root, v, floor, ceil, x);
    }
    void rangeApply(Tp &t, const Tag &v, ix l, ix r, ix x, ix y) {
        if (!t)
            t = _new(l, r);
        ix m = (l + r) >> 1;
        if (x <= l && r <= y) {
            apply(t, v, l, r);
            return;
        }
        push(t, l, m, r);
        if (m > x)
            rangeApply(t->ch[0], v, l, m, x, y);
        if (m < y)
            rangeApply(t->ch[1], v, m, r, x, y);
        pull(t, l, m, r);
    }
    void rangeApply(ix x, ix y, const Tag &v) {
        if (x >= y) return;
        rangeApply(root, v, floor, ceil, x, y);
    }
    Info Query(Tp &t, ix l, ix r, ix x) {
        if (!t)
            return Info::merge(l, r);
        ix m = (l + r) >> 1;
        if (r - l == 1) {
            return t->info;
        }
        push(t, l, m, r);
        if (m > x)
            return Query(t->ch[0], l, m, x);
        else
            return Query(t->ch[1], m, r, x);
    }
    Info Query(ix x) {
        return Query(root, floor, ceil, x);
    }
    Info rangeQuery(Tp &t, ix l, ix r, ix x, ix y) {
        if (!t)
            return Info::merge(l, r);
        ix m = (l + r) >> 1;
        if (x <= l && r <= y) {
            return t->info;
        }
        push(t, l, m, r);
        if (m >= y) {
            return rangeQuery(t->ch[0], l, m, x, y);
        } else if (m <= x) {
            return rangeQuery(t->ch[1], m, r, x, y);
        } else {
            return Info::merge(rangeQuery(t->ch[0], l, m, x, y), rangeQuery(t->ch[1], m, r, x, y), l, m, r);
        }
    }
    Info rangeQuery(ix x, ix y) {
        return rangeQuery(root, floor, ceil, x, y);
    }
    double BS(Tp &t, ix l, ix r, i64 k) {
        if (!t) t = _new(l, r);

        // debug(l, r, k, t->info);

        if (r - l == 1) {
            assert(t->info != 0);
            // if (t->info == 0) exit(0);
            return l + 1. * k / t->info;
        }

        ix m = (l + r) >> 1;
        push(t, l, m, r);

        if (t->ch[0]->info >= k)
            return BS(t->ch[0], l, m, k);
        else 
            return BS(t->ch[1], m, r, k - t->ch[0]->info);
    }
    double BS(i64 k) {
        return BS(root, floor, ceil, k);
    }
    void show(Tp &t, ix l, ix r, ix x, ix y, int dep = 0) {
        if (l >= y || r <= x || !t) return;
        ix m = (l + r) >> 1;
        if (r - l > 1)
        show(t->ch[0], l, m, x, y, dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        cerr << l << ' ' << r << ' '; t->info.show(), t->tag.show();
        cerr << '\n';
        if (r - l > 1)
        show(t->ch[1], m, r, x, y, dep + 1);
    }
    void show(ix x, ix y) {
        show(root, floor, ceil, x, y);
    }
    Tp _root() { return root; }
};

struct Tag {
    i64 x = 0;
    void apply(const Tag &rhs) {
        x += rhs.x;
    }
    operator bool() {
        return x != 0;
    }
    void clear() {
        x = 0;
    }
    void show() {
        cerr << "Tag: " << x;
    }
};

struct Info {
    i64 x = 0;
    void apply(const Tag &rhs, ix l, ix r) {
        x += (r - l) * rhs.x;
    }
    void update(const Info &lhs, const Info &rhs, ix l, ix m, ix r) {
        x = lhs.x + rhs.x;
    }
    static Info merge(const Info &lhs, const Info &rhs, ix l, ix m, ix r) {
        Info info = Info();
        info.update(lhs, rhs, l, m, r);
        return info;
    }
    static Info merge(ix l, ix r) {
        return {0};
    }
    void show() {
        cerr << "Info: " << x << ' ';
    }
};

using SegmentTree = segment_tree<Info, Tag>;