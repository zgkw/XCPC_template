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

    void pull(int x) {
        // debug(x);
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

private:


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
            tree[x].mval.virtual_update(tree[rc(x)].mval);
            rc(x) = y;
            tree[x].mval.roll_virtual(tree[rc(x)].mval);
            pull(x);
            y = x;
            x = fa(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        tree[x].mval.reverse();
        tree[x].tag ^= 1;
    }

    //x变为原树的根，y变成辅助树的根
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
        makeroot(y);
        if (findroot(y) != x) {
            fa(x) = y;
            tree[y].mval.virtual_update(tree[x].mval);
        } 
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
        makeroot(x);
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

constexpr int inf = 1e9;

struct Info {
    int c = 0, w = 0;
    i64 vsum = 0; int vsz = 0;
    i64 sum = 0; int szsum = 0;
    i64 lw = 0, rw = 0;
    i64 vsumsum = 0;
    i64 vedge = 0;
    i64 vedgesum = 0;
    void reverse() {
        // debug(sum);
        sum = szsum * (lw + w + rw) - sum - vedgesum; 
        swap(lw, rw);
    }
    void modify(const Info& rhs) {
        c = !c;
    }
    void virtual_update(const Info &rhs) {
        vsum += rhs.Sum();
        vedge += 1ll * rhs.szsum * w;
        vsz += rhs.szsum;
    }
    void roll_virtual(const Info &rhs) {
        vsum -= rhs.Sum();
        vedge -= 1ll * rhs.szsum * w;
        vsz -= rhs.szsum;
    }
    void update(const Info &lhs, const Info &rhs) {
        szsum = lhs.szsum + c + rhs.szsum + vsz;
        lw = lhs.lw + lhs.w + lhs.rw;
        rw = rhs.lw + rhs.w + rhs.rw;
        sum = lhs.sum + c * lw + rhs.sum + (lw + w) * rhs.szsum + (lw) * vsz;
        vsumsum = lhs.vsumsum + vsum + rhs.vsumsum;
        vedgesum = lhs.vedgesum + vedge + rhs.vedgesum;
    }
    i64 Sum() const { return sum + vsumsum + vedgesum; }
    void defaultclear() {
    }
    void show() {
        debug(w, c, vsum, vsz, lw, rw, sum, vsz, szsum);
    }
};

using Tree = LinkCutTree<Info>;