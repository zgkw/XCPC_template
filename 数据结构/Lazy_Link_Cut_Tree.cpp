template<class Info, class Tag>
struct LazyLinkCutTree {
    struct node {
        int s[2], p, tag;
        Info mval;
        Tag mtag;
    };
    int n;
    vector<node> tree;

    int &fa(int x) { return tree[x].p; }
    int &lc(int x) { return tree[x].s[0]; }
    int &rc(int x) { return tree[x].s[1]; }
    bool notroot(int x) {
        return lc(fa(x)) == x || rc(fa(x)) == x;
    }
    // 不能以0开头
    LazyLinkCutTree(int n) : n(n) { 
        tree.resize(n + 1); 
        tree[0].mtag.default_clear();
        tree[0].mval.default_clear();
    }

private:
    void pull(int x) {
        tree[x].mval.update(tree[lc(x)].mval, tree[rc(x)].mval);
    }

    void apply(int x, const Tag &rhs) {
        if (x) {
            tree[x].mval.apply(rhs);
            tree[x].mtag.apply(rhs);
        }
    }

    void push(int x) {
        if (tree[x].tag) {
            swap(lc(x), rc(x));
            tree[rc(x)].tag ^= 1;
            tree[lc(x)].tag ^= 1;
            tree[x].tag = 0;
        }
        if (bool(tree[x].mtag)) {
            apply(lc(x), tree[x].mtag);
            apply(rc(x), tree[x].mtag);
            tree[x].mtag.clear();
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
        pull(y), pull(x);
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

    void linemodify(int u, int v, const Tag &rhs) {
        split(u, v);
        apply(v, rhs);
    }
    
    bool same(int x, int y) {
        makeroot(x);
        return findroot(y) == x;
    }

    node &operator[](int x) {
        return tree[x];
    }
};

struct Tag {
    int mul = 1, add = 0;
    void apply(const Tag &rhs) {
        mul *= rhs.mul;
        add *= rhs.mul;
        add += rhs.add;
    }
    void clear() {
        mul = 1;
        add = 0;
    }
    operator bool() {
        return mul != 1 || add != 0;
    }
    void defaultclear() {}
};


struct Info {
    int v = 1; int sz = 1; int sum = 0;
    void modify(const Info& rhs) {
        v = rhs.v;
    }
    void update(const Info &lhs, const Info &rhs) {
        sum = lhs.sum + v + rhs.sum;
        sz = lhs.sz + 1 + rhs.sz;
    }
    void apply(const Tag &rhs) {
        v *= rhs.mul;
        v += rhs.add;
        sum *= rhs.mul;
        sum += rhs.add * sz;
    }
    void defaultclear() {
        v = 0, sz = 0, sum = 0;
    }
};

using Tree = LazyLinkCutTree<Info, Tag>;