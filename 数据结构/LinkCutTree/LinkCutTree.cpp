template<class Info>
struct linkCutTree {
    struct node {
        int s[2], p, tag;
        Info mval;
    };
    int n;
    vector<node> tree;

    int &fa(int x) { return tree[x].p; }
    int &lc(int x) { return tree[x].s[0]; }
    int &rc(int x) { return tree[x].s[1]; }
    // notroot
    bool pos(int x) {
        return tree[tree[x].p].s[0] == x || tree[tree[x].p].s[1] == x;
    }
    // 不能以0开头
    linkCutTree(int n) : n(n) { tree.resize(n + 1); tree[0].mval.defaultclear(); }

    void pull(int x) {
        tree[x].mval.up(tree[lc(x)].mval, tree[rc(x)].mval);
    }

    void push(int x) {
        if (tree[x].tag) {
            swap(lc(x), rc(x));
            tree[lc(x)].mval.reve();
            tree[rc(x)].mval.reve();
            tree[rc(x)].tag ^= 1;
            tree[lc(x)].tag ^= 1;
            tree[x].tag = 0;
        }
    }

    // maintain
    void mt(int x) {
        if (pos(x)) mt(fa(x));
        push(x);
    }
    // rotate
    void rtt(int x) {
        int y = fa(x), z = fa(y);
        int k = rc(y) == x;
        if (pos(y))
            tree[z].s[rc(z) == y] = x;
        fa(x) = z;
        tree[y].s[k] = tree[x].s[k ^ 1];
        fa(tree[x].s[k ^ 1]) = y;
        tree[x].s[k ^ 1] = y;
        fa(y) = x;
        pull(y);
    }
    void splay(int x) {
        mt(x);
        while (pos(x)) {
            int y = fa(x), z = fa(y);
            if (pos(y))
                ((rc(z) == y) ^ (rc(y) == x))
                ? rtt(x) : rtt(y);
            rtt(x);
        }
        pull(x);
    }       
    // access
    void acc(int x) {
        for (int y = 0; x;) {
            splay(x);
            rc(x) = y;
            pull(x);
            y = x;
            x = fa(x);
        }
    }

    // makeroot
    void mrt(int x) {
        acc(x);
        splay(x);
        tree[x].tag ^= 1;
    }

    //y变成原树和辅助树的根
    const Info &split(int x, int y) {
        mrt(x);
        acc(y);
        splay(y);
        return tree[y].mval;
    }

    // findroot
    int find(int x) {
        acc(x);
        splay(x);
        while (lc (x))
            push(x), x = lc(x);
        splay(x);
        return x;
    }

    void link(int x, int y) {
        mrt(x);
        if (find(y) != x) fa(x) = y;
    }

    void cut(int x, int y) {
        mrt(x);
        if (find(y) == x
            && fa(y) == x && !lc(y)) {
            rc(x) = fa(y) = 0;
            pull(x);
        }
    }

    void modify(int x, const Info &val) {
        splay(x);
        tree[x].mval.modify(val);
        pull(x);
    }

    bool same(int x, int y) {
        mrt(x);
        return find(y) == x;
    }
    node &operator[](int x) {
        return tree[x];
    }
    void dfs(int u) {
        auto dfs = [&] (auto &&dfs, int u, int fa, int from) -> void {
            // push(u);
            for (auto i : {0, 1}) {
                if (i == 1) {
                    cerr << '(' << fa << " [" << from << ']' << " -> "  << u << ')' << ' ';
                    debug(tree[u].s[0], tree[u].s[1]);
                    tree[u].mval.show();
                }
                if (tree[u].s[i]) {
                    dfs(dfs, tree[u].s[i], u, i);
                }
            }
        };
        dfs(dfs, u, u, 0);
    }
};

struct Info {
    void reve() {}
    void modify(const Info& rhs) {}
    void up(const Info &lhs, const Info &rhs) {}
    // default
    void clear() {}
};

using Tree = linkCutTree<Info>;