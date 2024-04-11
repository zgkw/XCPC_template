template<class Node, class Info>
struct Chair_Tree {
    int n;
    std::vector<Node*> root;
    Chair_Tree() : n(0) {}
    Chair_Tree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    template<class T>
    Chair_Tree(std::vector<T> init_) {
        init(init_());
    }
    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        Node* now = new Node;
        root.emplace_back(now);
        std::function<void(Node*, int, int)> 
        build = [&] (Node* now, int l, int r) {
            if (r - l == 1) {
                now->val = init_[l];
                return;
            }
            int m = (l + r) / 2;
            now->ch[0] = new Node;
            now->ch[1] = new Node;
            build(now->ch[0], l, m);
            build(now->ch[1], m, r);
            pull(now);
        };
        build(now, 0, n);
    }
    void pull(Node* now) {
        now->val = now->ch[0]->val + now->ch[1]->val;
    }
    void modify(int x, const Info &v, int from = -1) {
        Node* last = (from == -1 ? root.back() : root[from]);
        Node* now = new Node;
        root.emplace_back(now);
        std::function<void(Node*, Node*, int, int)> 
        modify = [&] (Node* last, Node* now, int l, int r) -> void {
            if (r - l == 1) {
                now->val = last->val;
                now->val.apply(v);
                return;
            }
            int m = (l + r) / 2;
            if(x < m) {
                now->ch[0] = new Node;
                now->ch[1] = last->ch[1];
                modify(last->ch[0], now->ch[0], l, m);
            } else {
                now->ch[0] = last->ch[0];
                now->ch[1] = new Node;
                modify(last->ch[1], now->ch[1], m, r);
            }
            pull(now);
        };
        modify(last, now, 0, n);
    }
    Info rangeQuery(int from, int to, int x, int y) {
        Node* last = root[from], *now = root[to];
        std::function<Info(Node*, Node*, int, int)>
        rangeQuery = [&] (Node* last, Node* now, int l, int r) {
            if (l >= y || r <= x) {
                return Info();
            }
            if (l >= x && r <= y) {
                return now->val - last->val;
            }
            int m = (l + r) / 2;
            return rangeQuery(last->ch[0], now->ch[0], l, m)
                    + rangeQuery(last->ch[1], now->ch[1], m, r);
        };
        return rangeQuery(last, now, 0, n);
    };
    Info kth(int from, int to, int k) {
        Node* last = root[from], * now = root[to];
        std::function<Info(Node*, Node*, int, int, int)> 
        kth = [&] (Node* last, Node* now, int l, int r, int k) {
            if (r - l == 1) {
                return Info(l, now->val(k));
            }
            int m = (l + r) / 2;
            Info ls = now->ch[0]->val - last->ch[0]->val;
            if (k <= ls) {
                return kth(last->ch[0], now->ch[0], l, m, k);
            } else {
                return kth_merge(now->ch[0]->val - last->ch[0]->val, 
                        kth(last->ch[1], now->ch[1], m, r, k - int(ls)));
            }
        };
        return kth(last, now, 0, n, k);
    }
    void show(int t) {
        Node* now = root[t];
        std::function<void(Node*, int, int)> 
        show = [&] (Node* now, int l, int r) {
            if (r - l == 1) {
                now->val.show();
                return;
            }
            int m = (l + r) / 2;
            show(now->ch[0], l, m);
            show(now->ch[1], m, r);
        };
        show(now, 0, n);
        cerr << endl;
    }
    int now_time () {
        return int(root.size()) - 1;
    }
};

struct Info {
    i64 cnt;
    i64 sum;
    i64 real;
    Info(i64 cnt = 0, i64 sum = 0) : cnt(cnt), sum(sum) {}
    void apply(const Info &v) {
        cnt += v.cnt;
        sum += 1ll * v.cnt * real;
    }
    void show() {
        debug(cnt, sum, real);
    }
    operator int() {
        return cnt;
    }
    i64 operator() (int k) {
        return 1ll * k * real;
    }
    friend Info kth_merge(Info lhs, Info rhs) {
        return Info(rhs.cnt, lhs.sum + rhs.sum);
    }
};

Info operator+ (Info lhs, Info rhs) {
    return Info(lhs.cnt + rhs.cnt, lhs.sum + rhs.sum);
}
Info operator- (Info lhs, Info rhs) {
    return Info(lhs.cnt - rhs.cnt, lhs.sum - rhs.sum);
}

struct Node {
    array<Node*, 2> ch{};
    Info val;
};

using SegmentTree = Chair_Tree<Node, Info>;