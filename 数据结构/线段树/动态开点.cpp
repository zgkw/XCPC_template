template<typename Info, typename Tag>
struct segment_tree {
    int n;
    struct node;
    using Tp = Base<node>;
    
    struct node {
        Info info;
        Tag tag;
        Tp ch[2];
    };
    Tp t{0};
    Tp news(i64 l, i64 r) {
        Tp t = Tp::news();
        return t;
    }
    void apply(Tp &t, const Tag &v, i64 l, i64 r) {
        if (!t) {
            t = news(l, r);
        }
        t->info.apply(v, l, r);
        t->tag.apply(v);
    }
    void push(Tp &t, i64 l, i64 m, i64 r) {
        if (!bool(t->tag))
            return;
        apply(t->ch[0], t->tag, l, m);
        apply(t->ch[1], t->tag, m, r);
        t->tag = Tag();
    }
    void pull(Tp &t, i64 l, i64 m, i64 r) {
        t->info.update(t->ch[0]->info, t->ch[1]->info, l, m, r);
    }
    i64 floor, ceil;
    segment_tree(i64 floor, i64 ceil) : floor(floor) , ceil(ceil) {}
    void modify(Tp &t, const Tag &v, i64 l, i64 r, i64 x) {
        if (!t)
            t = news(l, r);
        i64 m = (l + r) >> 1;
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
    void modify(i64 x, const Tag &v) {
        modify(t, v, floor, ceil, x);
    }
    void rangeApply(Tp &t, const Tag &v, i64 l, i64 r, i64 x, i64 y) {
        if (!t)
            t = news(l, r);
        i64 m = (l + r) >> 1;
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
    void rangeApply(i64 x, i64 y, const Tag &v) {
        if (x >= y) return;
        rangeApply(t, v, floor, ceil, x, y);
    }
    Info Query(Tp &t, i64 l, i64 r, i64 x) {
        if (!t)
            return Info::merge(l, r);
        i64 m = (l + r) >> 1;
        if (r - l == 1) {
            return t->info;
        }
        push(t, l, m, r);
        if (m > x)
            return Query(t->ch[0], l, m, x);
        else
            return Query(t->ch[1], m, r, x);
    }
    Info Query(i64 x) {
        return Query(t, floor, ceil, x);
    }
    Info rangeQuery(Tp &t, i64 l, i64 r, i64 x, i64 y) {
        if (!t)
            return Info::merge(l, r);
        i64 m = (l + r) >> 1;
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
    Info rangeQuery(i64 x, i64 y) {
        return rangeQuery(t, floor, ceil, x, y);
    }
    void DFS(Tp &t, i64 l, i64 r, i64 x, i64 y, int dep = 0) {
        if (l >= y || r <= x || !t) return;
        i64 m = (l + r) >> 1;
        if (r - l > 1)
        DFS(t->ch[0], l, m, x, y, dep + 1);
        cerr << string(dep, '\t');
        cerr << l << ' ' << r << ' ' << t->info << t->tag;
        cerr << '\n';
        if (r - l > 1)
            DFS(t->ch[1], m, r, x, y, dep + 1);
    }
    void dfs(i64 x, i64 y) {
        DFS(t, floor, ceil, x, y);
    }
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
    friend ostream &operator<<(ostream &cout, Tag t) {
        return cout << "tag" << ";";
    }
};

struct Info {
    i64 x = 0;
    void apply(const Tag &rhs, i64 l, i64 r) {
        x += (r - l) * rhs.x;
    }
    void update(const Info &lhs, const Info &rhs, i64 l, i64 m, i64 r) {
        x = lhs.x + rhs.x;
    }
    static Info merge(const Info &lhs, const Info &rhs, i64 l, i64 m, i64 r) {
        Info info = Info();
        info.update(lhs, rhs, l, m, r);
        return info;
    }
    static Info merge(i64 l, i64 r) {
        return {0};
    }
    friend ostream &operator<<(ostream &cout, Info t) {
        return cout << "Info" << "; ";
    }
};

using SegmentTree = segment_tree<Info, Tag>;