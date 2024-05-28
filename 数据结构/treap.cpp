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
 * FHQ_treap 卡常:
 * 1.递归改非递归       x
 * 2.insert split优化   o
 * 3.build 优化         o
 */

__gnu_cxx::sfmt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

template<typename Info, typename Tag>
struct FHQ_treap {
    struct Node;
    using Tp = u32_p<Node>;
    
    using T = typename Info::T;
    struct Node {
        Tp ch[2];
        Info info;
        int key;
        Tag tag;
        bool rev;
    };
    
    Tp __new() {
        Tp t = Tp::__new();
        t->key = rng();
        return t;
    }

    void apply(Tp t, const Tag &tag) {
        if (t) {
            t->info.apply(tag);
            t->tag.apply(tag);
        }
    }

    void push(Tp t) {
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

    void pull(Tp t) {
        t->info.up(t->ch[0]->info, t->ch[1]->info);
    }

    pair<Tp, Tp> split_by_val(Tp t, T val) {
        if (!t) {
            return {t, t};
        }
        // push(t);
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
        Tp ohs;
        if (rank <= t->ch[0]->info.siz) {
            tie(ohs, t->ch[0]) = split_by_rank(t->ch[0], rank);
            pull(t);
            return {ohs, t};
        } else if (rank > t->ch[0]->info.siz + 1) {
            tie(t->ch[1], ohs) = split_by_rank(t->ch[1], rank - 1 - t->ch[0]->info.siz);
            pull(t);
            return {t, ohs};
        } else {
            ohs = t->ch[0];
            t->ch[0] = 0;
            pull(t);
            return {ohs, t};
        }
    }

    Tp merge(Tp u, Tp v) {
        if (!u | !v) return u.x | v.x;
        if (u->key < v->key) {
            push(u);
            u->ch[1] = merge(u->ch[1], v);
            pull(u);
            return u;
        } else {
            push(v);
            v->ch[0] = merge(u, v->ch[0]);
            pull(v);
            return v;
        }
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

    Tp build(int l, int r) {
        if (r - l == 1) {
            Tp t = __new();
            t->info.init(l);
            return t;
        }
        int m = l + r >> 1;
        return merge(build(l, m), build(m, r));
    }

    
    void insert(Tp &t, Tp v) {
        if (!t) {
            t = v;
            return;
        }
        if (t->key < v->key) {
            tie(v->ch[0], v->ch[1]) = split_by_val(t, v->info.val);
            t = v;
            pull(t);
            return;
        }
        // t->info.siz += 1;
        insert(t->ch[v->info.val > t->info.val || 
            (t->info.val == v->info.val && int(rng()) >= 0)], v);
        pull(t);
    }

    void erase(Tp &t, T v) {
        if (t->info.val == v) {
            t = merge(t->ch[0], t->ch[1]);
            return;
        } else {
            // t->info.siz -= 1;
            erase(t->ch[v > t->info.val], v);
            pull(t);
        }
    }

    int less_to_val(Tp t, Info val) {
        int less_siz = 0;
        while (t) {
            if (t->info.val >= val.val) {
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
    Tp prev_to_val(Tp t, Info val) {
        Tp p;
        while (t) {
            if (t->info.val < val.val) {
                p = t;
                t = t->ch[1];
            } else {
                t = t->ch[0];
            }
        }
        return p;
    }
    Tp next_to_val(Tp t, Info val) {
        Tp p;
        while (t) {
            if (t->info.val <= val.val) {
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
        push(t);
        dfs(t->ch[0], dep + 1);
        cout << t->info.val << ' ';
        // for (int i = 0; i < dep; i += 1) cerr << '\t';
        // cerr << t->info << ' ' << t->key << ' ' << t->rev << '\n';
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
    void reve() {}
    void up(const Info &lhs, const Info &rhs) {
        siz = lhs.siz + 1 + rhs.siz;
    }
    void init(int val) {
        this->val = val;
        siz = 1;
    }
    void apply(const Tag &t) {}
    friend ostream &operator<<(ostream &cout, Info rhs) {
        return cout << "Info: " << rhs.val << ' ' << rhs.siz;
    }
};


using treap = FHQ_treap<Info, Tag>;
using Tp = treap::Tp;
treap T;