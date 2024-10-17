# include <ext/random>
__gnu_cxx::sfmt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

struct node;
using Tp = Base<node>;

struct node {
    Tp ch[2];
    int siz, k;
    i64 val;
    i64 sum;
};

Tp news() {
    Tp t = Tp::news();
    t->k = rng();
    return t;
}

Tp news(auto val) {
    Tp t = news();
    t->val = val;
    t->siz = 1;
    t->sum = val;
    return t;
}

void pull(Tp t) {
    t->siz = t->ch[0]->siz + 1 + t->ch[1]->siz;
    t->sum = t->ch[0]->sum + t->val + t->ch[1]->sum;
}

// to [-inf, val) and [val, inf]
pair<Tp, Tp> split1(Tp t, auto val) {
    if (!t) {
        return {t, t};
    }
    Tp u;
    if (t->val < val) {
        tie(t->ch[1], u) = split1(t->ch[1], val);
        pull(t);
        return {t, u};
    } else {
        tie(u, t->ch[0]) = split1(t->ch[0], val);
        pull(t);
        return {u, t};
    }
}


// to [1, rk) and [rk, n]
pair<Tp, Tp> split2(Tp t, int rk) {
    if (!t) {
        return {t, t};
    }
    Tp u;
    if (rk <= t->ch[0]->siz) {
        tie(u, t->ch[0]) = split2(t->ch[0], rk);
        pull(t);
        return {u, t};
    } else if (rk > t->ch[0]->siz + 1) {
        tie(t->ch[1], u) = split2(t->ch[1], rk - 1 - t->ch[0]->siz);
        pull(t);
        return {t, u};
    } else {
        u = t->ch[0];
        t->ch[0] = 0;
        pull(t);
        return {u, t};
    }
}

Tp merge(Tp u, Tp v) {
    if (!u | !v) return u.x | v.x;
    if (u->k < v->k) {
        u->ch[1] = merge(u->ch[1], v);
        pull(u);
        return u;
    } else {
        v->ch[0] = merge(u, v->ch[0]);
        pull(v);
        return v;
    }
}

// 2056

void dfs(Tp t, int dep = 0) {
    if (!t) {
        return;
    }
    dfs(t->ch[0], dep + 1);
    for (int i = 0; i < dep; i += 1) cerr << '\t';
    cerr << t->val << ' ' << t->sum << ' ' << t->siz << '\n';
    dfs(t->ch[1], dep + 1);
}

// less_to_val_siz
int less_to_val(Tp t, auto val) {
    int less_siz = 0;
    while (t) {
        if (t->val >= val) {
            t = t->ch[0];
        } else {
            less_siz += t->ch[0]->siz + 1;
            t = t->ch[1];
        }
    }
    return less_siz;
}


Tp rank(Tp t, int rk) {
    while (true) {
        if (t->ch[0]->siz >= rk) {
            t = t->ch[0];
        } else if (t->ch[0]->siz + 1 < rk) {
            rk -= t->ch[0]->siz + 1;
            t = t->ch[1];
        } else
            break;
    }
    return t;
}

// prev_to_val
Tp prev(Tp t, auto val) {
    Tp p;
    while (t) {
        if (t->val < val) {
            p = t;
            t = t->ch[1];
        } else {
            t = t->ch[0];
        }
    }
    return p;
}
// next_to_val
Tp next(Tp t, auto val) {
    Tp p;
    while (t) {
        if (t->val <= val) {
            t = t->ch[1];
        } else {
            p = t;
            t = t->ch[0];
        }
    }
    return p;
}