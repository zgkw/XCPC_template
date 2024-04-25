template<class Info>
struct LinkCutTree {
    struct node {
        int s[2], p, tag;
        Info mval;
    };
    int n;
    vector<node> tree;

    int &fa(int x) { return tree[x].p; }
    int &lc(int x) { return tree[x].s[0]; }
    int &rc(int x) { return tree[x].s[1]; }
    bool notroot(int x) {
        return tree[tree[x].p].s[0] == x || tree[tree[x].p].s[1] == x;
    }
    // 不能以0开头
    LinkCutTree(int n) : n(n) { tree.resize(n + 1); tree[0].mval.defaultclear(); }

private:
    void pull(int x) {
        tree[x].mval.update(tree[lc(x)].mval, tree[rc(x)].mval);
    }

    void push(int x) {
        if (tree[x].tag) {
            swap(lc(x), rc(x));
            tree[lc(x)].mval.reverse();
            tree[rc(x)].mval.reverse();
            tree[rc(x)].tag ^= 1;
            tree[lc(x)].tag ^= 1;
            tree[x].tag = 0;
        }
    }

    void maintain(int x) {
        if (notroot(x)) maintain(fa(x));
        push(x);
    }

    void rotate(int x) {
        int y = fa(x), z = fa(y);
        int k = rc(y) == x;
        if (notroot(y))
            tree[z].s[rc(z) == y] = x;
        fa(x) = z;
        tree[y].s[k] = tree[x].s[k ^ 1];
        fa(tree[x].s[k ^ 1]) = y;
        tree[x].s[k ^ 1] = y;
        fa(y) = x;
        pull(y);
    }

public:
    void splay(int x) {
        maintain(x);
        while (notroot(x)) {
            int y = fa(x), z = fa(y);
            if (notroot(y))
                ((rc(z) == y) ^ (rc(y) == x))
                ? rotate(x) : rotate(y);
            rotate(x);
        }
        pull(x);
    }

    void access(int x) {
        for (int y = 0; x;) {
            splay(x);
            rc(x) = y;
            pull(x);
            y = x;
            x = fa(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        tree[x].tag ^= 1;
    }

    //y变成原树和辅助树的根
    const Info &split(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
        return tree[y].mval;
    }

    int findroot(int x) {
        access(x);
        splay(x);
        while (lc (x))
            push(x), x = lc(x);
        splay(x);
        return x;
    }

    void link(int x, int y) {
        makeroot(x);
        if (findroot(y) != x) fa(x) = y;
    }

    void cut(int x, int y) {
        makeroot(x);
        if (findroot(y) == x
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
        makeroot(x);
        return findroot(y) == x;
    }
    node &operator[](int x) {
        return tree[x];
    }
};

struct Info {
    int v = 1; int id = -1; int sum = 0; int max = 0;
    void reverse() {}
    void modify(const Info& rhs) {
        v = rhs.v;
    }
    void update(const Info &lhs, const Info &rhs) {
        sum = lhs.sum + v + rhs.sum;
        max = std::max({lhs.max, id, rhs.max});
    }
    void defaultclear() {
        v = 0;
    }
};

using Tree = LinkCutTree<Info>;