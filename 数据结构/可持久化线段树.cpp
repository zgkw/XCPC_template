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
        std::function<void(Node*, int, int)> build = [&] (Node* now, int l, int r) {
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
    void modify(int x, const Info &v) {
        Node* last = root.back();
        Node* now = new Node;
        root.emplace_back(now);
        std::function<void(Node*, Node*, int, int, int, const Info&)> modify = [&] (Node* last, Node* now, int l, int r, int x, const Info &v) -> void {
            if (r - l == 1) {
                now->val = last->val + v;
                return;
            }
            int m = (l + r) / 2;
            if(x < m) {
                now->ch[0] = new Node;
                now->ch[1] = last->ch[1];
                modify(last->ch[0], now->ch[0], l, m, x, v);
            } else {
                now->ch[0] = last->ch[0];
                now->ch[1] = new Node;
                modify(last->ch[1], now->ch[1], m, r, x, v);
            }
            pull(now);
        };
        modify(last, now, 0, n, x, v);
    }
    int kth(int l, int r, int k) {
        Node* x = root[l - 1], * y = root[r - 1];
        std::function<int(Node*, Node*, int, int, int)> kth = [&] (Node* x, Node* y, int l, int r, int k) {
            if (r - l == 1) {
                return l;
            }
            int m = (l + r) / 2;
            int ls = y->ch[0]->val - x->ch[0]->val;
            if (k <= ls) {
                return kth(x->ch[0], y->ch[0], l, m, k);
            } else {
                return kth(x->ch[1], y->ch[1], m, r, k - ls);
            }
        };
        return kth(x, y, 0, n, k);
    }
    void show(int t) {
        Node* now = root[t];
        std::function<void(Node*, int, int)> show = [&] (Node* now, int l, int r) {
            if (r - l == 1) {
                cerr << now->val << ' ';
                return;
            }
            int m = (l + r) / 2;
            show(now->ch[0], l, m);
            show(now->ch[1], m, r);
        };
        show(now, 0, n);
        cerr << endl;
    }
};

struct Node {
    array<Node*, 2> ch{};
    int val;
};

using SegmentTree = Chair_Tree<Node, int>;