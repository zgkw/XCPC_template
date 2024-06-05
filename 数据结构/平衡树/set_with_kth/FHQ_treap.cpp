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

/**
 * FHQ_treap set卡常:
 * 1.递归改非递归       o
 * 2.insert split优化   o
 */

__gnu_cxx::sfmt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<typename Info>
struct FHQ_treap {
    struct Node;
    using Tp = u32_p<Node>;
    struct Node {
        Tp ch[2];
        Info val;
        int siz, key;
    };

    Tp root;

    void pull(Tp t) {
        t->siz = t->ch[0]->siz + 1 + t->ch[1]->siz;
    }
    // by val
    pair<Tp, Tp> split(Tp t, Info val) {
        if (!t) {
            return {t, t};
        }
        Tp ohs;
        if (t->val < val) {
            tie(t->ch[1], ohs) = split(t->ch[1], val);
            pull(t);
            return {t, ohs};
        } else {
            tie(ohs, t->ch[0]) = split(t->ch[0], val);
            pull(t);
            return {ohs, t};
        }
    }

    Tp merge(Tp u, Tp v) {
        if (!u | !v) return u.x | v.x;
        if (u->key < v->key) {
            u->ch[1] = merge(u->ch[1], v);
            pull(u);
            return u;
        } else {
            v->ch[0] = merge(u, v->ch[0]);
            pull(v);
            return v;
        }
    }

// set operator
    void insert(Tp &t, Tp v) {
        if (!t) {
            t = v;
            // ps;
            return;
        }
        if (t->key < v->key) {
            tie(v->ch[0], v->ch[1]) = split(t, v->val);
            t = v;
            pull(t);
            return;
        }
        t->siz += 1;
        insert(t->ch[v->val > t->val || 
            (t->val == v->val && int(rng()) >= 0)], v);
        pull(t);
    }

    void insert(Info v) {
        Tp t = Tp::__new();
        t->key = rng();
        t->val = v;
        t->siz = 1;
        insert(root, t);
    }

    void erase(Tp &t, Info v) {
        if (t->val == v) {
            t = merge(t->ch[0], t->ch[1]);
            return;
        } else {
            // t->siz -= 1;
            erase(t->ch[v > t->val], v);
            pull(t);
        }
    }

    void erase(Info v) {
        erase(root, v);
    }
    // by val
    int less(Info v) {
        Tp t = root;
        int less_siz = 0;
        while (t) {
            if (t->val >= v) {
                t = t->ch[0];
            } else {
                less_siz += t->ch[0]->siz + 1;
                t = t->ch[1];
            }
        }
        return less_siz;
    }
    // from zero
    Tp rank(Tp t, int k) {
        k += 1;
        while (true) {
            if (t->ch[0]->siz >= k) {
                t = t->ch[0];
            } else if (t->ch[0]->siz + 1 < k) {
                k -= t->ch[0]->siz + 1;
                t = t->ch[1];
            } else
                break;
        }
        return t;
    }
    // from zero
    Tp operator[] (int k) {
        return rank(root, k);
    }
    // by val
    static constexpr int inf = std::numeric_limits<int>::max();
    Info prev(Info v) {
        Tp t = root, p;
        while (t) {
            if (t->val < v) {
                p = t;
                t = t->ch[1];
            } else {
                t = t->ch[0];
            }
        }
        return p ? p->val : -inf;
    }
    // by val
    Info next(Info v) {
        Tp t = root, p;
        while (t) {
            if (t->val <= v) {
                t = t->ch[1];
            } else {
                p = t;
                t = t->ch[0];
            }
        }
        return p ? p->val : inf;
    }
    void dfs(Tp t, int dep = 0) {
        if (!t) {
            return;
        }
        dfs(t->ch[0], dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        cerr << t->val << ' ' << t->key << '\n';
        dfs(t->ch[1], dep + 1);
    }
    void dfs() {return dfs(root);}
};