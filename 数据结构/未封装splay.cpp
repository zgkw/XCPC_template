constexpr int inf = 1e9;

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

struct Tree {
    int add = 0;
    int val = 0;
    int id = 0;
    u32_p<Tree> ch[2], p;
};

using Tp = u32_p<Tree>;

Tp __new() {
    return Tp::__new();
}

int pos(Tp t) {
    return t->p->ch[1] == t;
}

void add(Tp t, int v) {
    t->val += v;
    t->add += v;
}

void push(Tp t) {
    if (t->ch[0]) {
        add(t->ch[0], t->add);
    }
    if (t->ch[1]) {
        add(t->ch[1], t->add);
    }
    t->add = 0;
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
}

void splay(Tp t) {
    std::vector<Tp > s;
    for (Tp i = t; i->p; i = i->p) s.push_back(i->p);
    while (!s.empty()) {
        push(s.back());
        s.pop_back();
    }
    push(t);
    while (t->p) {
        if (t->p->p) {
            if (pos(t) == pos(t->p)) rotate(t->p);
            else rotate(t);
        }
        rotate(t);
    }
}

void insert(Tp &t, Tp x, Tp p = 0) {
    if (!t) {
        t = x;
        x->p = p;
        return;
    }
    
    push(t);
    if (x->val < t->val) {
        insert(t->ch[0], x, t);
    } else {
        insert(t->ch[1], x, t);
    }
}

void dfs(Tp t) {
    if (!t) {
        return;
    }
    push(t);
    dfs(t->ch[0]);
    std::cerr << t->val << " ";
    dfs(t->ch[1]);
}

std::pair<Tp , Tp > split(Tp t, int x) {
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
    }
    return {u, v};
}

Tp merge(Tp l, Tp r) {
    if (!l) {
        return r;
    }
    if (!r) {
        return l;
    }
    Tp i = l;
    while (i->ch[1]) {
        i = i->ch[1];
    }
    splay(i);
    i->ch[1] = r;
    r->p = i;
    return i;
}