struct node;
using Tp = Base<node>;
struct node {
    Tp ch[2], p;
    i64 val;
    i64 tag;
    i64 sum;
    int siz;
};

Tp news() {
    return Tp::news();
}

Tp news(auto val) {
    Tp t = news();
    t->val = val;
    t->siz = 1;
    t->sum = val;
    t->tag = 0;
}

bool pos(Tp t) {
    return t->p->ch[1] == t;
}

void apply(Tp t, auto tag) {
    if (t) {
        t->val += tag;
        t->sum += 1ll * siz * tag;
        t->tag += tag;
    }
};

void push(Tp t) {
    if (t->tag) {
        apply(t->ch[0], t->tag);
        apply(t->ch[1], t->tag);
        t->tag = decltype(t->tag)();
    }
}

void pull(Tp t) {
    t->siz = t->ch[0]->siz + 1 + t->ch[1]->siz;
    t->sum = t->ch[0]->sum + 1 + t->ch[1]->sum;
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

pair<Tp, Tp> split1(Tp t, auto x) {
    if (!t) {
        return {t, t};
    }
    Tp v = 0;
    Tp j = t;
    for (Tp i = t; i; ) {
        push(i);
        j = i;
        if (i->val >= x) {
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

// 从 1 开始
Tp findK(Tp &t, int k) {
    int mid = k;
    while (true) {
        push(t);
        if (k > t->ch[0]->siz + 1) {
            k -= t->ch[0]->siz + 1;
            t = t->ch[1];
        } else if (k <= t->ch[0]->siz) {
            t = t->ch[0];
        } else {
            break;
        }
    }
    splay(t);
    return t;
}

// [1, x) and [x, n]
pair<Tp, Tp> split2(Tp t, int x) {
    if (t->siz < x) {
        return {t, 0};
    }

    findK(t, x);
    
    Tp u = t->ch[0];
    if (u) {
        t->ch[0] = u->p = 0;
        pull(t);
    }
    return {u, t};
}

Tp merge(Tp l, Tp r) {
    if (!l || !r) {
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

void dfs(Tp t, int dep = 0) {
    if (!t) {
        return;
    }
    push(t);
    dfs(t->ch[0], dep + 1);
    for (int i = 0; i < dep; i += 1) cerr << '\t';
    cerr << t->val << "\n";
    dfs(t->ch[1], dep + 1);
}


// 写出来之后感觉不太会用到，所以没测，谨慎使用