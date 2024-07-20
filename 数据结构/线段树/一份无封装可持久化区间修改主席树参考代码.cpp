struct node;
using Tp = u32_p<node>;
Tp _new() {
    return Tp::__new();
}
struct node {
    Tp ch[2];
    int val;
    int add;
};

int cnt = 0;

Tp _new(Tp t) {
    cnt += 1;
    Tp u = _new();
    u->val = t->val;
    u->add = t->add;
    u->ch[0] = t->ch[0];
    u->ch[1] = t->ch[1];
    return u;
}

void apply(Tp &t, int val) {
    t = _new(t);
    t->val += val;
    t->add += val;
}

void push(Tp &t) {
    int val = t->add;
    if (val) {
        apply(t->ch[0], val);
        apply(t->ch[1], val);
        t->add = 0;
    }
}

void rangeAdd(Tp &t, int l, int r, int x, int y, int val) {
    t = _new(t);
    if (x <= l && r <= y) {
        t->add += val;
        t->val += val;
        return;
    }
    push(t);
    int m = l + r >> 1;
    if (m > x) {
        rangeAdd(t->ch[0], l, m, x, y, val);
    }
    if (m < y) {
        rangeAdd(t->ch[1], m, r, x, y, val);
    }
}

int query(Tp &t, int x, int l, int r) {
    t = _new(t);
    if (r - l == 1) {
        return t->val;
    }
    push(t);
    int m = l + r >> 1;
    return m > x ? query(t->ch[0], x, l, m) : query(t->ch[1], x, m, r);
}

void modify(Tp &t, int x, int l, int r) {
    t = _new(t);
    if (r - l == 1) {
        t->val = 0;
        return;
    }
    push(t);
    int m = l + r >> 1;
    m > x ? modify(t->ch[0], x, l, m) : modify(t->ch[1], x, m, r);
}

void merge(Tp &u, Tp &v, Tp &t, int c, int l, int r) {
    if (r <= c) {
        t = u;
        return;
    }
    if (l >= c) {
        t = v;
        return;
    }
    t = _new();
    int m = l + r >> 1;
    u = _new(u);
    push(u);
    v = _new(v);
    push(v);
    merge(u->ch[0], v->ch[0], t->ch[0], c, l, m);
    merge(u->ch[1], v->ch[1], t->ch[1], c, m, r);
}

void dfs(Tp t, int l, int r, int dep = 0) {
# ifdef LOCAL
    if (!t) {
        return;
    }
    int m = l + r >> 1;
    dfs(t->ch[0], l, m, dep + 1);
    cerr << string(dep, '\t');
    cerr << l << ' ' << r << ' ' << t->val << ' ' << t->add << '\n';
    dfs(t->ch[1], m, r, dep + 1);
# endif
}