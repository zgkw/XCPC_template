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

template<class Info, class Tag>
struct Balance_Tree {
    struct Tree;
    using Tp = u32_p<Tree>;
    
    struct Tree {
        Tp ch[2], p;
        Info info;
        bool rev;
        Tag tag;
    };

    // build operator
    Balance_Tree() {
        Tp()->info.Null();
    }
    Tp __new () {
        return Tp::__new();
    }

    Tp build (int l, int r) {
        if (l > r) return 0;
        int m = l + r >> 1;
        Tp p = __new();
        p->ch[0] = build(l, m - 1);
        if (p->ch[0]) p->ch[0]->p = p;
        {
            // fun
        }
        p->ch[1] = build(m + 1, r);
        if (p->ch[1]) p->ch[1]->p = p;
        pull(p);
        return p;
    }
    template<typename F>
    Tp build (int l, int r, F fun) {
        if (l > r) return 0;
        int m = l + r >> 1;
        Tp p = __new();
        p->ch[0] = build(l, m - 1, fun);
        if (p->ch[0]) p->ch[0]->p = p;
        fun(p, m);
        p->ch[1] = build(m + 1, r, fun);
        if (p->ch[1]) p->ch[1]->p = p;
        pull(p);
        return p;
    }
    // build operator

    // basic operator
    bool pos(Tp t) {
        return t->p->ch[1] == t;
    }

    void apply(Tp t, const Tag &v) {
        if (t) {
            t->info.apply(v);
            t->tag.apply(v);
        }
    }

    void push(Tp t) {
        if (t->rev) {
            t->ch[0]->rev ^= 1;
            t->ch[1]->rev ^= 1;
            swap(t->ch[0], t->ch[1]);
            t->rev = 0;
        }
        if (t->tag) {
            apply(t->ch[0], t->tag);
            apply(t->ch[1], t->tag);
            t->tag = Tag();
        }
    }

    void pull(Tp t) {
        t->info.up(t->ch[0]->info, t->ch[1]->info);
    }

    void rotate(Tp t) {
        Tp q = t->p;
        int x = !pos(t);
        q->ch[!x] = t->ch[x];
        if (t->ch[x]) t->ch[x]->p = q;
        t->p = q->p;
        if (q->p) q->p->ch[pos(q)] = t;
        t->ch[x] = q;
        q->p = t;
        pull(q);
    }

    void pushall(Tp t) {
        if (t->p) pushall(t->p);
        push(t);
    }

    void splay(Tp t, Tp top = 0) {
        pushall(t);
        while (t->p != top) {               
            if (t->p->p != top)
                rotate(pos(t) ^ pos(t->p) ? t : t->p);
            rotate(t);
        }
        pull(t);
    }
    // basic operator

    // shrink operator
    Tp rank(Tp &t, int k) {
        int mid = k;
        while (true) {
            push(t);
            if (k > t->ch[0]->info.siz + t->info.rep_cnt) {
                k -= t->ch[0]->info.siz + t->info.rep_cnt;
                t = t->ch[1];
            } else if (k <= t->ch[0]->info.siz) {
                t = t->ch[0];
            } else break;
        }
        splay(t);
        return t;
    }

    template<bool isRight>
    void split_by_range(Tp &t, int k) { // split range, but not really split
        rank(t, k);
        if constexpr(!isRight) {
            if (k > t->info.l) {
                Tp l = __new();
                (l->ch[0] = t->ch[0])->p = l;
                (l->p = t)->ch[0] = l;
                l->info.init(t->info.l, k - 1, t->info);
                t->info.init(k, t->info.r, t->info);
                pull(l), pull(t);
            }
        } else {
            if (k < t->info.r) {
                Tp r = __new();
                (r->ch[1] = t->ch[1])->p = r;
                (r->p = t)->ch[1] = r;
                r->info.init(k + 1, t->info.r, t->info);
                t->info.init(t->info.l, k, t->info);
                pull(r), pull(t);
            }
        }
    }

    Tp shrink_by_split_range(Tp &t, int l, int r) {
        if (r == t->info.siz && l == 1) {
            return t;
        } else if (r == t->info.siz) {
            split_by_range<1>(t, l - 1);
            return t->ch[1];
        } else if (l == 1) {
            split_by_range<0>(t, r + 1);
            return t->ch[0];
        } else {
            split_by_range<1>(t, l - 1);
            Tp lhs = t;
            split_by_range<0>(t, r + 1);
            splay(lhs, t);
            return lhs->ch[1];
        }
    }

    Tp shrink(Tp &t, int l, int r) {
        if (r == t->info.siz && l == 1) {
            return t;
        } else if (r == t->info.siz) {
            rank(t, l - 1);
            return t->ch[1];
        } else if (l == 1) {
            rank(t, r + 1);
            return t->ch[0];
        } else {
            Tp lhs = rank(t, l - 1);
            rank(t, r + 1);
            splay(lhs, t);
            return lhs->ch[1];
        }
    }

    void pullall(Tp t) {
        for (t = t->p; t; t = t->p) 
            pull(t);
    }
    // shrink operator

    // split and merge
    std::pair<Tp, Tp> split_by_val(Tp t, int x) {
        if (!t) {
            return {t, t};
        }
        Tp v = 0;
        Tp j = t;
        for (Tp i = t; i; ) {
            push(i);
            j = i;
            if (i->info >= x) {
                v = i;
                i = i->ch[0];
            } else {
                i = i->ch[1];
            }
        }
        
        splay(j);
        if (!v) {
            return {j, 0};
        }
        
        splay(v);
        
        Tp u = v->ch[0];
        if (u) {
            v->ch[0] = u->p = 0;
            pull(v);
        }
        return {u, v};
    }

    std::pair<Tp, Tp> split_by_rank(Tp t, int x) {
        if (t->info.siz < x) {
            return {t, 0};
        }

        rank(t, x);
        
        Tp u = t->ch[0];
        if (u) {
            t->ch[0] = u->p = 0;
            pull(t);
        }
        return {u, t};
    }

    Tp merge(Tp l, Tp r) {
        if (l.x * r.x == 0) {
            return l.x | r.x;
        }
        Tp i = l;
        push(i);
        for (; i->ch[1]; i = i->ch[1], push(i));
        splay(i);
        i->ch[1] = r;
        r->p = i;
        pull(i);
        return i;
    }
    // split and merge

    // set operator
    void insert(Tp &t, Tp x) {
        Tp p = 0;
        
        while (t && t->info.x != x->info.x) {
            push(t);
            p = t;
            t = t->ch[x->info.x > t->info.x];
        }

        if (!t) {
            t = x;
            t->p = p;
            if (p) p->ch[t->info.x > p->info.x] = t;
        } else {
            t->info.apply(x->info);
        }
        splay(t);
    }

    void find(Tp &t, const Info &rhs) {
        // if (!t) {
        //     return;
        // }
        while (t->info.x != rhs.x && t->ch[rhs.x > t->info.x]) {
            t = t->ch[rhs.x > t->info.x];
        }
        splay(t);
    }

    Tp prev_by_val(Tp &t, const Info &rhs) {
        Tp p;
        while (t) {
            if (t->info.x >= rhs.x) {
                t = t->ch[0];
            } else {
                p = t;
                t = t->ch[1];
            }
        }
        splay(t = p);
        return p;
    }

    Tp next_by_val(Tp &t, const Info &rhs) {
        Tp p;
        while (t) {
            if (t->info.x <= rhs.x) {
                t = t->ch[1];
            } else {
                p = t;
                t = t->ch[0];
            }
        }
        splay(t = p);
        return p;
    }

    void erase(Tp &t, const Info &rhs) {
        find(t, rhs);
        if (t->info == rhs && t->info.erase()) {
            Tp lhs = t->ch[0], rhs = t->ch[1];
            lhs->p = 0, rhs->p = 0;
            t = merge(lhs, rhs);
        }
        splay(t);
    }
    // set operator

    void dfs(Tp t, int dep = 0) {
        if (!t) {
            return;
        }
        push(t);
        dfs(t->ch[0], dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        std::cerr << t->info << "\n";
        dfs(t->ch[1], dep + 1);
    }

};

struct Tag {
    int set = 0;
    void apply(const Tag &t) {
        set = t.set;
    }
    operator bool() {
        return set;
    }
};

struct Info {
    int x = 1, rep_cnt = 1, siz = 1;
    int l = 0, r = 0;
    int sum = 0;
    void up(const Info &lhs, const Info &rhs) {
        siz = lhs.siz + rep_cnt + rhs.siz;
        sum = lhs.sum + x * rep_cnt + rhs.sum;
    }
    void apply(const Tag &t) {
        x = t.set - 1;
        sum = siz * x;
    }
    void apply(const Info &t) {}
    friend ostream &operator<<(ostream &cout, Info rhs) {
        return cout << rhs.x << ' ' << rhs.rep_cnt << ' ' << rhs.siz << ' ' << rhs.l << ' ' << rhs.r << ' ' << rhs.sum;
    }
    void init(int L, int R, Info from) {
        l = L, r = R; rep_cnt = r - l + 1; x = from.x;
    }
    void Null() {}
};

using BT = Balance_Tree<Info, Tag>;
using Tp = BT::Tp;
BT tree;