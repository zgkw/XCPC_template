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
        return (head += sizeof(T)) - buf;
    }
};

struct persistent_DSU {
    int n;
    struct node;
    using Tp = u32_p<node>;
    struct node {
        int f, siz;
        Tp ch[2];
    };
    Tp _new() {
        Tp t = Tp::__new();
        return t;
    }
    vector<Tp> root;
    persistent_DSU(): n(0) {} 
    persistent_DSU(int _n, int _m = 0) {
        init(_n, _m);
    }
    void build(Tp t, int l, int r) {
        if (r - l == 1) {
            t->f = l;
            t->siz = 1;
            return;
        }
        int m = (l + r) / 2;
        t->ch[0] = _new(), t->ch[1] = _new();
        build(t->ch[0], l, m), build(t->ch[1], m, r);
    }
    void init(int _n, int m = 0) {
        n = _n;
        root.reserve(m + 1);
        root.push_back(_new());
        build(root.back(), 0, n);
    }
    void modify0(Tp &t0, Tp &t1, Tp v, int l, int r, int x) {
        if (r - l == 1) {
            t1->f = v->f;
            t1->siz = t0->siz;
            return;
        }
        int m = (l + r) >> 1;
        if (m > x) {
            t1->ch[0] = _new();
            t1->ch[1] = t0->ch[1];
            modify0(t0->ch[0], t1->ch[0], v, l, m, x);
        } else {
            t1->ch[0] = t0->ch[0];
            t1->ch[1] = _new();
            modify0(t0->ch[1], t1->ch[1], v, m, r, x);
        }
    }
    void modify0(int x, Tp v, Tp t0, Tp t1) {
        modify0(t0, t1, v, 0, n, x);
    }
    void modify1(Tp &t0, Tp &t1, Tp v, int l, int r, int x) {
        if (r - l == 1) {
            t1->f = t0->f;
            t1->siz = t0->siz + v->siz;
            return;
        }
        int m = (l + r) >> 1;
        if (m > x) {
            t1->ch[0] = _new();
            t1->ch[1] = t0->ch[1];
            modify1(t0->ch[0], t1->ch[0], v, l, m, x);
        } else {
            t1->ch[0] = t0->ch[0];
            t1->ch[1] = _new();
            modify1(t0->ch[1], t1->ch[1], v, m, r, x);
        }
    }
    void modify1(int x, Tp v, Tp t0, Tp t1) {
        modify1(t0, t1, v, 0, n, x);
    }
    void dfs(Tp t, int l, int r) {
        if (r - l == 1) {
            cerr << "(" << t->f << ", " << t->siz << "), ";
            return;
        }
        int m = (l + r) >> 1;
        dfs(t->ch[0], l, m);
        dfs(t->ch[1], m, r);
    }
    void dfs(int time) {
        dfs(root[time], 0, n);
        cerr << endl;
    }
    Tp Query(Tp t, int l, int r, int x) {
        while (r - l != 1) {
            int m = (l + r) / 2;
            if (m > x) 
                t = t->ch[0], r = m;
            else 
                t = t->ch[1], l = m;
        }
        return t;
    }
    Tp Query(int x, Tp t) {
        return Query(t, 0, n, x);
    }
    Tp find(int x, Tp t) {
        Tp fa = Query(x, t);
        return fa->f == x ? 
            fa : find(fa->f, t);
    }
    bool same(int u, int v, int t = -1) {
        t = t == -1 ? int(root.size()) - 1 : t;
        root.push_back(root[t]);
        Tp lhs = find(u, root[t]), rhs = find(v, root[t]);
        return lhs->f == rhs->f;
    }
    void merge(int u, int v, int t = -1) {
        t = t == -1 ? int(root.size()) - 1 : t;
        Tp lhs = find(u, root[t]), rhs = find(v, root[t]);
        if (lhs->f == rhs->f) {
            root.push_back(root[t]);
            return;
        }
        if (lhs->siz < rhs->siz) {
            swap(lhs, rhs);
        }
        Tp cur0 = _new();
        modify0(rhs->f, lhs, root[t], cur0);
        Tp cur1 = _new();
        modify1(lhs->f, rhs, cur0, cur1);
        root.push_back(cur1);
    }
    void roll(int t) {
        root.push_back(root[t]);
    }
};
using DSU = persistent_DSU;