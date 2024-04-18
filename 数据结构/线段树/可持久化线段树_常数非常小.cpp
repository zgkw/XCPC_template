constexpr int max_size = 262144000;
uint8_t _buf[max_size];
uint8_t *head = _buf;

template<typename Info>
struct persistent_segment_tree {
    int n;
    struct node {
        Info m_info;
        int ls, rs;
        node () : m_info(), ls(), rs() {}
        void reset () {
            *this = node();
        }
    };
    using pointer = node *;
    int _new() {
        assert(head < _buf + max_size);
        return (head += sizeof(node)) - _buf;
    }
    vector<int> root;
    persistent_segment_tree(): n(0) {} 
    persistent_segment_tree(int _n, Info _v = Info()) {
        _init(std::vector(_n, _v));
    } 
    template<typename T>
    persistent_segment_tree(std::vector<T> _init) {
        _init(_init);
    }
    void _pull(int cur1) {
        pointer p1 = pointer(_buf + cur1);
        pointer lc = pointer(_buf + p1->ls);
        pointer rc = pointer(_buf + p1->rs);
        p1->m_info.set(Info::op(lc->m_info, rc->m_info));
    }
    template<typename T>
    void _init(std::vector<T> _init) {
        n = _init.size();
        root.push_back(_new());
        std::function<void(int, int, int)>
        build = [&] (int cur, int l, int r) {
            pointer p = pointer(_buf + cur);
            if (r - l == 1) {
                p->m_info = _init[l];
                return;
            }
            int m = (l + r) / 2;
            p->ls = _new(), p->rs = _new();
            build(p->ls, l, m), build(p->rs, m, r);
            _pull(cur);
        };
        build(root.back(), 0, n);
    }
    template<typename Tag>
    void _modify(int cur0, int cur1, const Tag &v, int l, int r, int x) {
        pointer p0 = pointer(_buf + cur0), p1 = pointer(_buf + cur1);
        if (r - l == 1) {
            p1->m_info = p0->m_info;
            p1->m_info.apply(v);
            return;
        }
        int m = (l + r) >> 1;
        if (m > x) {
            p1->ls = _new();
            p1->rs = p0->rs;
            _modify(p0->ls, p1->ls, v, l, m, x);
        } else {
            p1->ls = p0->ls;
            p1->rs = _new();
            _modify(p0->rs, p1->rs, v, m, r, x);
        }
        _pull(cur1);
    }
    template<typename Tag>
    void modify(int x, const Tag &v, int from = -1) {
        int cur0 = (from == -1 ? root.back() : root[from]);
        int cur1 = _new();
        root.push_back(cur1);
        _modify(cur0, cur1, v, 0, n, x);
    }
    typename Info::op_t _range_query(int cur0, int cur1, int l, int r, int x, int y) {
        pointer p0 = pointer(_buf + cur0), p1 = pointer(_buf + cur1);
        if (x <= l && r <= y) {
            return Info::del(p1->m_info, p0->m_info);
        }
        int m = (l + r) >> 1;
        if (m >= y) {
            return _range_query(p0->ls, p1->ls, l, m, x, y);
        } else if (m <= x) {
            return _range_query(p0->rs, p1->rs, m, r, x, y);
        } else {
            return Info::op(_range_query(p0->ls, p1->ls, l, m, x, y), _range_query(p0->rs, p1->rs, m, r, x, y));
        }
    }
    typename Info::op_t range_query(int from, int to, int x, int y) {
        return _range_query(root[from], root[to], 0, n, x, y);
    }
    typename Info::op1_t _kth(int cur0, int cur1, int l, int r, i64 k) {
        pointer p0 = pointer(_buf + cur0), p1 = pointer(_buf + cur1);
        pointer ls0 = pointer(_buf + p0->ls), ls1 = pointer(_buf + p1->ls);
        if (r - l == 1) {
            return Info::op1(l, Info::op1(k));
        }
        int m = (l + r) >> 1;
        typename Info::op1_t lhs = Info::del1(ls1->m_info, ls0->m_info);
        if (int(lhs) >= k) {
            return _kth(p0->ls, p1->ls, l, m, k);
        } else {
            return Info::op1(lhs, _kth(p0->rs, p1->rs, m, r, k - int(lhs)));
        }
    }
    typename Info::op1_t kth(int from, int to, i64 k) {
        return _kth(root[from], root[to], 0, n, k);
    }
    void _show(int cur, int l, int r) {
        pointer p = pointer(_buf + cur);
        if (r - l == 1) {
            p->m_info.show();
            return;
        }
        int m = (l + r) >> 1;
        _show(p->ls, l, m);
        _show(p->rs, m, r);
    }
    void show(int time) {
        _show(root[time],0, n);
    }
};

struct Info {
    i64 cnt = 0;
    using op_t = int;
    using op1_t = int;
    operator op_t() {
        return cnt;
    }
    void set(op_t rhs) {
        cnt = rhs;
    }
    static op_t op(op_t lhs, op_t rhs) {
        return lhs + rhs;
    }
    static op_t del(op_t lhs, op_t rhs) {
        return lhs - rhs;
    }
    static array<ll, 1> op1 (i64 k) {
        return array<ll, 1>{0};
    }
    static op1_t op1(int x, array<ll, 1> mul) {
        return x;
    }
    static op1_t op1(op1_t lhs, op1_t rhs) {
        return rhs;
    }
    static op1_t del1(op1_t lhs, op1_t rhs) {
        return lhs - rhs;
    }
    void apply(Info x) {
        cnt += x.cnt;
    }
    void show() {
        cerr << cnt << ' ';
    }
};

using SegmentTree = persistent_segment_tree<Info>;