template<typename T>
struct Link_Cut_Tree {
# define fa(x) tr[x].p
# define lc(x) tr[x].s[0]
# define rc(x) tr[x].s[1]
# define notroot(x) lc (fa(x)) == x || rc(fa(x)) == x
    struct node {
        T s[2], p, v, sum;
        int tag;
    };
    int n;
    vector <node> tree;
    vector <node> &tr = tree;

    Link_Cut_Tree<T>(int n) : n(n) { tree.resize(n + 1); }

    void pushup(int x) {
        tr[x].sum = tr[lc(x)].sum +
                    tr[x].v + tr[rc(x)].sum;
    }

    void pushdown(int x) {
        if (tr[x].tag) {
            swap(lc(x), rc(x));
            tr[rc(x)].tag ^= 1;
            tr[lc(x)].tag ^= 1;
            tr[x].tag = 0;
        }
    }

    void pushall(int x) {
        if (notroot(x)) pushall(fa(x));
        pushdown(x);
    }

    void rotate(int x) {
        int y = fa(x), z = fa(y);
        int k = rc(y) == x;
        if (notroot(y))
            tr[z].s[rc(z) == y] = x;
        fa(x) = z;
        tr[y].s[k] = tr[x].s[k ^ 1];
        fa(tr[x].s[k ^ 1]) = y;
        tr[x].s[k ^ 1] = y;
        fa(y) = x;
        pushup(y);
        pushup(x);
    }

    void splay(int x) {
        pushall(x);
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
            pushup(x);
            y = x;
            x = fa(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        tr[x].tag ^= 1;
    }

    void split(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
    }

    int findroot(int x) {
        access(x);
        splay(x);
        while (lc (x))
            pushdown(x), x = lc(x);
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
            pushup(x);
        }
    }

    void change(int x, int val) {
        splay(x);
        tr[x].v = val;
        pushup(x);
    }
};