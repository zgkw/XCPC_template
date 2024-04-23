template<class Info>
struct Link_Cut_Tree {
    struct node {
        int s[2], p, tag;
        Info m_val;
    };
    int n;
    vector<node> tree;

    int &_fa(int x) { return tree[x].p; }
    int &_lc(int x) { return tree[x].s[0]; }
    int &_rc(int x) { return tree[x].s[1]; }
    bool _notroot(int x) {
        return _lc(_fa(x)) == x || _rc(_fa(x)) == x;
    }
    // 不能以0开头
    Link_Cut_Tree(int n) : n(n) { tree.resize(n + 1); }

    void _pull(int x) {
        tree[x].m_val.update(tree[_lc(x)].m_val, tree[_rc(x)].m_val);
    }

    void _push(int x) {
        if (tree[x].tag) {
            swap(_lc(x), _rc(x));
            tree[_rc(x)].tag ^= 1;
            tree[_lc(x)].tag ^= 1;
            tree[x].tag = 0;
        }
    }

    void _maintain(int x) {
        if (_notroot(x)) _maintain(_fa(x));
        _push(x);
    }

    void _rotate(int x) {
        int y = _fa(x), z = _fa(y);
        int k = _rc(y) == x;
        if (_notroot(y))
            tree[z].s[_rc(z) == y] = x;
        _fa(x) = z;
        tree[y].s[k] = tree[x].s[k ^ 1];
        _fa(tree[x].s[k ^ 1]) = y;
        tree[x].s[k ^ 1] = y;
        _fa(y) = x;
        _pull(y), _pull(x);
    }

    void _splay(int x) {
        _maintain(x);
        while (_notroot(x)) {
            int y = _fa(x), z = _fa(y);
            if (_notroot(y))
                ((_rc(z) == y) ^ (_rc(y) == x))
                ? _rotate(x) : _rotate(y);
            _rotate(x);
        }
    }

    void _access(int x) {
        for (int y = 0; x;) {
            _splay(x);
            _rc(x) = y;
            _pull(x);
            y = x;
            x = _fa(x);
        }
    }

    void _makeroot(int x) {
        _access(x);
        _splay(x);
        tree[x].tag ^= 1;
    }

    //y变成原树和辅助树的根
    const Info &split(int x, int y) {
        _makeroot(x);
        _access(y);
        _splay(y);
        return tree[y].m_val;
    }

    int findroot(int x) {
        _access(x);
        _splay(x);
        while (_lc (x))
            _push(x), x = _lc(x);
        _splay(x);
        return x;
    }

    void link(int x, int y) {
        _makeroot(x);
        if (findroot(y) != x) _fa(x) = y;
    }

    void cut(int x, int y) {
        _makeroot(x);
        if (findroot(y) == x
            && _fa(y) == x && !_lc(y)) {
            _rc(x) = _fa(y) = 0;
            _pull(x);
        }
    }

    void modify(int x, const Info &val) {
        _splay(x);
        tree[x].m_val.modify(val);
        _pull(x);
    }
};

struct Info {
    int v = 0; int sum = 0;
    void modify(const Info& rhs) {
        v = rhs.v;
    }
    void update(const Info &lhs, const Info &rhs) {
        sum = lhs.sum ^ v ^ rhs.sum;
    }
};

using Tree = Link_Cut_Tree<Info>;