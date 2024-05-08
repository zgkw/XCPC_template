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
        return tree[tree[x].p].s[0] == x || tree[tree[x].p].s[1] == x;
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
            tree[lc(x)].mval.reverse();
            tree[rc(x)].mval.reverse();
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

    void line_modify(int u, int v, const Tag &rhs) {
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
    void show(int u) {
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

struct Tag {
    int set = 0;
    void apply(const Tag &rhs) {
        set = rhs.set;
    }
    void clear() {
        set = 0;
    }
    operator bool() {
        return set != 0;
    }
    void default_clear() {}
};

struct Info {
    int c = 0; int sum = 0, l = 0, r = 0, id = 0;
    void reverse() {
        swap(l, r);
    }
    void modify(const Info& rhs) {
        l = r = c = rhs.c;
    }
    void update(const Info &lhs, const Info &rhs) {
        sum = lhs.sum + (c != lhs.r && lhs.r != 0) + (c != rhs.l && rhs.l != 0) + rhs.sum;
        l = (lhs.r == 0 ? c : lhs.l);
        r = (rhs.l == 0 ? c : rhs.r);
    }
    void apply(const Tag &rhs) {
        l = r = c = rhs.set; sum = 0;
    }
    void show() const {
        debug(id);
        cerr << l << ' ' << c << ' ' << r << ' ' << sum << endl;
    }
    void default_clear() {}
};

using Tree = LazyLinkCutTree<Info, Tag>;