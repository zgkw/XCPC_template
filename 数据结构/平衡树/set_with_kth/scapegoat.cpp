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
        // assert(x < max_size);
        return (head += sizeof(T)) - buf;
    }
};

constexpr double alpha = 0.75;
template<typename Info>
struct scapegoat_tree {
    struct node;
    using Tp = u32_p<node>;
    struct node {
        Tp ch[2];
        Info val;
        int siz, fac;
        bool exist;
    };

    Tp root = 0;

    Tp __new() {
        return Tp::__new();
    }

    void reset(Tp &t) {
        t->siz = t->fac = 1;
        t->exist = true;
        t->ch[0] = t->ch[1] = 0;
    }

    void reset(Tp &t, Info val) {
        t->siz = t->fac = 1;
        t->exist = true;
        t->ch[0] = t->ch[1] = 0;
        t->val = val;
    }

    Tp __new(Info val) {
        Tp t = __new();
        reset(t, val);
        return t;
    }

    scapegoat_tree() {}

    bool imbalance(Tp t) {
        return max({t->ch[0]->siz, t->ch[1]->siz}) 
                    > t->siz * alpha 
        || t->siz * alpha > t->fac;
    }

    vector<Tp> v;
    void collect(Tp t) {
        if (!t) return;
        collect(t->ch[0]);
        if (t->exist)
            v.push_back(t);
        collect(t->ch[1]);
    }
    void pull(Tp t) {
        t->siz = t->ch[0]->siz + 1 + t->ch[1]->siz;
        t->fac = t->ch[0]->fac + t->exist + t->ch[1]->fac;
    }
    void lift(int l, int r, Tp &t) {
        if (l == r) {
            t = v[l];
            reset(t);
            return;
        }
        int m = l + r >> 1;
        while (l < m && v[m]->val == v[m - 1]->val) {
            -- m;
        }
        t = v[m];
        if (l != m) lift(l, m - 1, t->ch[0]);
        else t->ch[0] = 0;
        lift(m + 1, r, t->ch[1]);
        pull(t);
    }
    void rebuild(Tp &t) {
        v.clear();
        collect(t);
        if (v.empty()) {
            t = 0;
            return;
        }
        lift(0, v.size() - 1, t);
    }

    void check(Tp &t, Tp E) {
        if (t == E) return;
        if (imbalance(t)) {
            rebuild(t);
            return;
        }
        check(t->ch[E->val >= t->val], E);
    }

    void insert(Tp &t, Info val) {
        if (!t) {
            t = __new(val);
            // dfs();
            check(root, t);
            return;
        }
        t->siz ++;
        t->fac ++;
        insert(t->ch[val >= t->val], val);
    }
    void insert(Info val) {
        insert(root, val);
    }
    void erase(Tp &t, Info val) {
        if (t->exist && t->val == val) {
            t->exist = false;
            t->fac --;
            check(root, t);
            return;
        }
        t->fac--;
        erase(t->ch[val >= t->val], val);
    }
    void erase(Info val) {
        erase(root, val);
    }
    int less(Info val) {
        Tp t = root;
        int less = 0;
        while (t) {
            if (val <= t->val) {
                t = t->ch[0];
            } else {
                less += t->exist + t->ch[0]->fac;
                t = t->ch[1];
            }
        }
        return less;
    }
    // from zero
    Tp operator[](int k) {
        k += 1;
        Tp t = root;
        while (t) {
            if (t->ch[0]->fac >= k) {
                t = t->ch[0];
            } else if (t->ch[0]->fac + t->exist < k) {
                k -= t->ch[0]->fac + t->exist;
                t = t->ch[1];
            } else 
                break;
        }
        return t;
    }
    void dfs(Tp t, int dep = 0) {
        if (!t) return;
        dfs(t->ch[0], dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        cerr << t->val << ' ' << t->siz << ' ' << t->fac << endl;
        dfs(t->ch[1], dep + 1);
    }
    void dfs() { return dfs(root); }
}; //scapegoat_tree

using scet = scapegoat_tree<int>;