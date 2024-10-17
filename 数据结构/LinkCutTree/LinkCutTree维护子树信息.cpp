template<class Info>
struct linkCutTree {
    struct node {
        int s[2], p, r;
        Info v;
    };
    int n;
    vector<node> t;

    int &fa(int x) { return t[x].p; }
    int &lc(int x) { return t[x].s[0]; }
    int &rc(int x) { return t[x].s[1]; }
    bool pos(int x) {
        return t[t[x].p].s[0] == x || t[t[x].p].s[1] == x;
    }
    // 不能以0开头
    linkCutTree(int n) : n(n) { t.resize(n + 1); t[0].v.clear(); }

    void pull(int x) {
        // debug(x);
        t[x].v.up(t[lc(x)].v, t[rc(x)].v);
    }
    void push(int x) {
        if (t[x].r) {
            swap(lc(x), rc(x));
            t[lc(x)].v.reve();
            t[rc(x)].v.reve();
            t[rc(x)].r ^= 1;
            t[lc(x)].r ^= 1;
            t[x].r = 0;
        }
    }
    void mt(int x) {
        if (pos(x)) mt(fa(x));
        push(x);
    }

    void rtt(int x) {
        int y = fa(x), z = fa(y);
        int k = rc(y) == x;
        if (pos(y))
            t[z].s[rc(z) == y] = x;
        fa(x) = z;
        t[y].s[k] = t[x].s[k ^ 1];
        fa(t[x].s[k ^ 1]) = y;
        t[x].s[k ^ 1] = y;
        fa(y) = x;
        pull(y);
    }
    void splay(int x) {
        mt(x);
        while (pos(x)) {
            int y = fa(x), z = fa(y);
            if (pos(y)) 
                ((rc(z) == y) ^ (rc(y) == x)) ? rtt(x) : rtt(y);
            rtt(x);
        }
        pull(x);
    }

    void acc(int x) {
        for (int y = 0; x;) {
            splay(x);
            t[x].v.vup(t[rc(x)].v);
            rc(x) = y;
            t[x].v.rv(t[rc(x)].v);
            pull(x);
            y = x;
            x = fa(x);
        }
    }

    void mrt(int x) {
        acc(x);
        splay(x);
        t[x].v.reve();
        t[x].r ^= 1;
    }

    //x变为原树的根，y变成辅助树的根
    const Info &split(int x, int y) {
        mrt(x);
        acc(y);
        splay(y);
        return t[y].v;
    }

    int find(int x) {
        acc(x);
        splay(x);
        while (lc(x))
            push(x), x = lc(x);
        splay(x);
        return x;
    }

    void link(int x, int y) {
        mrt(x);
        mrt(y);
        if (find(y) != x) {
            fa(x) = y;
            t[y].v.vup(t[x].v);
        } 
    }

    void cut(int x, int y) {
        mrt(x);
        if (find(y) == x && fa(y) == x && !lc(y)) {
            rc(x) = fa(y) = 0;
            pull(x);
        }
    }

    void modify(int x, const Info &val) {
        mrt(x);
        t[x].v.modify(val);
        pull(x);
    }

    bool same(int x, int y) {
        mrt(x);
        return find(y) == x;
    }
    node &operator[](int x) {
        return t[x];
    }
    void DFS(int u, int dep = 0) {
        if (!u) {
            return;
        }
        push(u);
        for (auto i : {0, 1}) {
            if (i == 1) {
                cerr << string(dep, '\t');
                cerr << u << ' ' << t[u].v << endl;
            }
            DFS(t[u].s[i], dep + 1);
        }
    }
    void dfs(int u) {
# ifndef ONLINE_JUDGE
        cerr << "\nlct rooted u: " << u << ", P = " << t[u].p << '\n';
        DFS(u);
# endif
    }
};

struct Info {
    void reve() {}
    void modify(const Info& rhs) {}
    void vup(const Info &rhs) {}
    void rv(const Info &rhs) {}
    void up(const Info &lhs, const Info &rhs) {}
    void clear() {}
# ifndef ONLINE_JUDGE
    friend ostream &operator<<(ostream &cout, Info u) {
        return cout;
    }
# endif
};

using Tree = linkCutTree<Info>;