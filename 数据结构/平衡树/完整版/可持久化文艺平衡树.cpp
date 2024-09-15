__gnu_cxx::sfmt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

u32 stk[200];

template<typename Info, typename Tag>
struct PersistentBalanceTree {
    struct Node;
    using Tp = u32_p<Node>;
    
    using T = Info::T;
    struct Node {
        Tp ch[2];
        Info info;
        int key;
        bool rev;
        Tag tag;
    };
    
    Tp __new() {
        Tp t = Tp::__new();
        t->key = rng();
        return t;
    }

    Tp __new(Tp t) {
        if (!t) return t;
        Tp p = Tp::__new();
        p->ch[0] = t->ch[0];
        p->ch[1] = t->ch[1];
        p->info = t->info;
        p->key = t->key;
        p->rev = t->rev;
        p->tag = t->tag;
        return p;
    }

    void apply(Tp t, const Tag &tag) {
        if (t) {
            t->info.apply(tag);
            t->tag.apply(tag);
        }
    }

    void push(Tp t) {
        if (t->rev || t->tag) {
            t->ch[0] = __new(t->ch[0]);
            t->ch[1] = __new(t->ch[1]);
            if (t->rev) {
                swap(t->ch[0], t->ch[1]);
                t->ch[0]->rev ^= 1;
                t->ch[0]->info.reve();
                t->ch[1]->rev ^= 1;
                t->ch[1]->info.reve();
                t->rev = 0;
            }
            if (t->tag) {
                apply(t->ch[0], t->tag);
                apply(t->ch[1], t->tag);
                t->tag = Tag();
            }
        }
    }

    void pull(Tp t) {
        t->info.up(t->ch[0]->info, t->ch[1]->info);
    }

    void rangeReverse(Tp &t, int x, int y) {
        // debug(x, y);
        auto [tmp, r] = split_by_rank(t, y);
        auto [l, m] = split_by_rank(tmp, x);
        m->rev ^= 1;
        m->info.reve();
        t = merge(l, merge(m, r));
    }

    void rangeApply(Tp &t, int x, int y, const Tag &tag) {
        auto [tmp, r] = split_by_rank(t, y);
        auto [l, m] = split_by_rank(tmp, x);
        apply(m, tag);
        t = merge(l, merge(m, r));
    }

    Info rangeQuery(Tp &t, int x, int y) {
        // debug(x, y);
        auto [tmp, r] = split_by_rank(t, y);
        auto [l, m] = split_by_rank(tmp, x);
        Info ans = m->info;
        t = merge(l, merge(m, r));
        return ans;
    }
//  split and merge
    pair<Tp, Tp> split_by_val(Tp &t, T val) {
        if (!t) {
            return {0, 0};
        }
        t = __new(t);
        push(t);
        Tp ohs;
        if (t->info.val < val) {
            tie(t->ch[1], ohs) = split_by_val(t->ch[1], val);
            pull(t);
            return {t, ohs};
        } else {
            tie(ohs, t->ch[0]) = split_by_val(t->ch[0], val);
            pull(t);
            return {ohs, t};
        }
    }

    pair<Tp, Tp> split_by_rank(Tp t, int rank) {
        if (!t) {
            return {t, t};
        }
        push(t);
        t = __new(t);
        Tp ohs;
        if (rank <= t->ch[0]->info.siz) {
            tie(ohs, t->ch[0]) = split_by_rank(t->ch[0], rank);
            pull(t);
            return {ohs, t};
        } else {
            tie(t->ch[1], ohs) = split_by_rank(t->ch[1], rank - 1 - t->ch[0]->info.siz);
            pull(t);
            return {t, ohs};
        }
    }
    
    template<bool isNew = false>
    Tp merge(Tp u, Tp v) {
        if (!u | !v) return u.x | v.x;
        if (u->key < v->key) {
            push(u);
            if (isNew) {
                u = __new(u);
            }
            u->ch[1] = merge<isNew>(u->ch[1], v);
            pull(u);
            return u;
        } else {
            push(v);
            if (isNew) {
                v = __new(v);
            }
            v->ch[0] = merge<isNew>(u, v->ch[0]);
            pull(v);
            return v;
        }
    }
//  split and merge


//  set operator

    // void insert_by_rank(Tp &t, int rank, Tp v) {
    //     auto [l, r] = split_by_rank(t, rank);
    //     t = merge(l, merge(v, r));
    // }

    void insert_by_rank(Tp &t, int rank, Tp v) {
        if (!t) {
            t = v;
            return;
        }
        push(t);
        t = __new(t);
        if (v->key < t->key) {
            tie(v->ch[0], v->ch[1]) = split_by_rank(t, rank);
            t = v;
            pull(t);
            return;
        }
        // debug(rank, t->ch[0]->info.siz);
        if (rank <= t->ch[0]->info.siz) {
            insert_by_rank(t->ch[0], rank, v);
        } else {
            insert_by_rank(t->ch[1], rank - 1 - t->ch[0]->info.siz, v);
        }
        pull(t);
    }

    // void erase_by_rank(Tp &t, int rank) {
    //     auto [tmp, r] = split_by_rank(t, rank);
    //     auto [l, m] = split_by_rank(tmp, rank - 1);
    //     t = merge(l, r);
    // }

    void erase_by_rank(Tp &t, int rank) {
        if (!t) return;
        push(t);
        t = __new(t);
        if (rank <= t->ch[0]->info.siz) {
            erase_by_rank(t->ch[0], rank);
            pull(t);
        } else if (rank > t->ch[0]->info.siz + 1) {
            erase_by_rank(t->ch[1], rank - 1 - t->ch[0]->info.siz);
            pull(t);
        } else {
            t = merge<true>(t->ch[0], t->ch[1]);
        }
    }

    void insert_by_val(Tp &t, Tp v) {
        t = __new(t);
        if (!t) {
            t = v;
            return;
        }
        if (t->key < v->key) {
            // push(t);
            tie(v->ch[0], v->ch[1]) = split_by_val(t, v->info.val);
            t = v;
            pull(t);
            return;
        }
        // t->info.siz += 1;
        insert_by_val(t->ch[v->info.val > t->info.val || (t->info.val == v->info.val && int(rng()) >= 0)], v);
        pull(t);
    }

    void erase_by_val(Tp &t, T v) {
        if (!t) return;
        t = __new(t);
        if (t->info.val == v) {
            t = merge(t->ch[0], t->ch[1]);
            return;
        } else {
            // t->info.siz -= 1;
            erase_by_val(t->ch[v > t->info.val], v);
            pull(t);
        }
    }
// not back
    void __insert_by_val(Tp &t, Tp v) {
        int Top = -1;
        Tp *p = &t;
        while (*p && v->key <= (*p)->key) {
            *p = __new(*p);
            stk[++ Top] = *p;
            p = &((*p)->ch[v->info.val > (*p)->info.val || ((*p)->info.val == v->info.val && int(rng()) >= 0)]);
        }
        if (*p) {
            tie(v->ch[0], v->ch[1]) = split_by_val(*p, v->info.val);
            pull(v);
        }
        *p = v;
        if (Top != -1) t = stk[0];
        while (Top != -1) {
            pull(stk[Top --]);
        }
    }

    void __erase_by_val(Tp &t, T v) {
        int Top = -1;
        Tp *p = &t;
        while (*p && (*p)->info.val != v) {
            *p = __new(*p);
            stk[++ Top] = *p;
            p = &((*p)->ch[v > (*p)->info.val]);
        }
        if (*p) {
            *p = merge((*p)->ch[0], (*p)->ch[1]);
        }
        if (Top != -1) t = stk[0];
        while (Top != -1) {
            pull(stk[Top --]);
        }
    }
// not back
    int less_to_val(Tp t, T val) {
        int less_siz = 0;
        while (t) {
            if (t->info.val >= val) {
                t = t->ch[0];
            } else {
                less_siz += t->ch[0]->info.siz + 1;
                t = t->ch[1];
            }
        }
        return less_siz;
    }
    Tp rank(Tp t, int rank) {
        while (true) {
            if (t->ch[0]->info.siz >= rank) {
                t = t->ch[0];
            } else if (t->ch[0]->info.siz + 1 < rank) {
                rank -= t->ch[0]->info.siz + 1;
                t = t->ch[1];
            } else
                break;
        }
        return t;
    }
    Tp prev_to_val(Tp t, T val) {
        Tp p;
        while (t) {
            if (t->info.val < val) {
                p = t;
                t = t->ch[1];
            } else {
                t = t->ch[0];
            }
        }
        return p;
    }
    Tp next_to_val(Tp t, T val) {
        Tp p;
        while (t) {
            if (t->info.val <= val) {
                t = t->ch[1];
            } else {
                p = t;
                t = t->ch[0];
            }
        }
        return p;
    }
    void dfs(Tp t, int dep = 0) {
        if (!t) {
            return;
        }
        dfs(t->ch[0], dep + 1);
        for (int i = 0; i < dep; i += 1) cerr << '\t';
        cerr << t->info << ' ' << t->key << ' ' << t->rev << '\n';
        dfs(t->ch[1], dep + 1);
    }
};

struct Tag {
    constexpr operator bool() {
        return false;
    }
    void apply(const Tag &t) {}
};
struct Info {
    using T = int;
    int val, siz;
    i64 sum;
    void reve() {}
    void up(const Info &lhs, const Info &rhs) {
        siz = lhs.siz + 1 + rhs.siz;
        sum = lhs.sum + val + rhs.sum;
    }
    void init(int val) {
        this->val = val;
        this->sum = val;
        siz = 1;
    }
    void apply(const Tag &t) {}
    friend ostream &operator<<(ostream &cout, Info rhs) {
        return cout << "Info: " << rhs.val << ' ' << rhs.sum << ' ' << rhs.siz;
    }
};

using treap = PersistentBalanceTree<Info, Tag>;
using Tp = treap::Tp;

treap T;