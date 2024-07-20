struct node;
using Tp = u32_p<node>;

struct node {
    Tp ch[2], p;
    bool rev;
    int siz = 1;
    node() : ch{nullptr, nullptr}, p(nullptr), rev(false) {}
};
void reverse(Tp t) {
    if (t) {
        std::swap(t->ch[0], t->ch[1]);
        t->rev ^= 1;
    }
}
void push(Tp t) {
    if (t->rev) {
        reverse(t->ch[0]);
        reverse(t->ch[1]);
        t->rev = false;
    }
}
void pull(Tp t) {
    t->siz = t->ch[0] + 1 + t->ch[1];
}
bool isroot(Tp t) {
    return !t->p || (t->p->ch[0] != t && t->p->ch[1] != t);
}
int pos(Tp t) {
    return t->p->ch[1] == t;
}
void pushAll(Tp t) {
    if (!isroot(t)) {
        pushAll(t->p);
    }
    push(t);
}
void rotate(Tp t) {
    Tp q = t->p;
    int x = !pos(t);
    q->ch[!x] = t->ch[x];
    if (t->ch[x]) {
        t->ch[x]->p = q;
    }
    t->p = q->p;
    if (!isroot(q)) {
        q->p->ch[pos(q)] = t;
    }
    t->ch[x] = q;
    q->p = t;
    pull(q);
}
void splay(Tp t) {
    pushAll(t);
    while (!isroot(t)) {
        if (!isroot(t->p)) {
            if (pos(t) == pos(t->p)) {
                rotate(t->p);
            } else {
                rotate(t);
            }
        }
        rotate(t);
    }
    pull(t);
}
void access(Tp t) {
    for (Tp i = t, q; i; q = i, i = i->p) {
        splay(i);
        i->ch[1] = q;
        pull(i);
    }
    splay(t);
}
void makeroot(Tp t) {
    access(t);
    reverse(t);
}
void link(Tp x, Tp y) {
    makeroot(x);
    x->p = y;
}
void split(Tp x, Tp y) {
    makeroot(x);
    access(y);
}
void cut(Tp x, Tp y) {
    split(x, y);
    x->p = y->ch[0] = 0;
    pull(y);
}
int dist(Tp x, Tp y) {
    split(x, y);
    return y->siz - 1;
}