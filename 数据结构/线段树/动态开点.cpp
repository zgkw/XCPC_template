uint8_t _buf[262144000];
uint8_t *head = _buf;

template<typename value_type, typename modify_type>
struct segment_tree {
    int n;
    using size_type = long long;
    using idx_type = uint32_t;
    struct node {
        value_type m_value;
        modify_type m_modify;
        idx_type ls, rs;
        node () :m_value(), m_modify(), ls(), rs() {}
        void reset() {
            *this = node();
        }
    };
    using ptr = node *;
    idx_type root{0};
    idx_type _new() {
        idx_type cur = (head += sizeof(node)) - _buf;
        // ptr p = ptr(_buf + cur);
        // p->reset();
        return cur;
    }
    void _apply(idx_type &cur, const modify_type &v, size_type l, size_type r) {
        if (!cur) {
            cur = _new();
        }
        ptr p = ptr(_buf + cur);
        p->m_value.apply(v, l, r);
        p->m_modify.apply(v);
    }
    void _push(idx_type &cur, size_type l, size_type r, size_type m) {
        ptr p = ptr(_buf + cur);
        // assert(l < r);
        if (!bool(p->m_modify)) {
            return;
        }
        _apply(p->ls, p->m_modify, l, m);
        _apply(p->rs, p->m_modify, m, r);
        p->m_modify.clear();
    }
    void _pull(idx_type &cur, size_type l, size_type r, size_type m) {
        ptr p = ptr(_buf + cur);
        ptr lc = ptr(_buf + p->ls);
        ptr rc = ptr(_buf + p->rs);
        p->m_value.set(value_type::op(lc->m_value, rc->m_value, l, r, m));
    }
    size_type floor, ceil;
    segment_tree(size_type floor, size_type ceil) : floor(floor) , ceil(ceil) {}
    void _range_modify(idx_type &cur, const modify_type &v, size_type l, size_type r, size_type x, size_type y) {
        if (!cur)
            cur = _new();
        ptr p = ptr(_buf + cur);
        size_type m = (l + r) >> 1;
        if (x <= l && r <= y) {
            _apply(cur, v, l, r);
            return;
        }
        _push(cur, l, r, m);
        if (m > x)
            _range_modify(p->ls, v, l, m, x, y);
        if (m < y)
            _range_modify(p->rs, v, m, r, x, y);
        _pull(cur, l, r, m);
    }
    void range_modify(size_type x, size_type y, const modify_type &v) {
        _range_modify(root, v, floor, ceil, x, y);
    }
    value_type::op_type _range_query(idx_type &cur, size_type l, size_type r, size_type x, size_type y) {
        if (!cur)
            return value_type::op(l, r);
        ptr p = ptr(_buf + cur);
        size_type m = (l + r) >> 1;
        if (x <= l && r <= y) {
            return value_type::op(p->m_value);
        }
        _push(cur, l, r, m);
        if (m >= y) {
            return _range_query(p->ls, l, m, x, y);
        } else if (m <= x) {
            return _range_query(p->rs, m, r, x, y);
        } else {
            return value_type::op(_range_query(p->ls, l, m, x, y), _range_query(p->rs, m, r, x, y));
        }
    }
    value_type::op_type range_query(size_type x, size_type y) {
        return _range_query(root, floor, ceil, x, y);
    }
    void _show(idx_type &cur, size_type l, size_type r) {
        if (!cur)
            return;
        ptr p = ptr(_buf + cur);
        size_type m = (l + r) >> 1;
        if (r - l == 1) {
            debug(p->m_value.x);
            return;
        }
        _push(cur, l, r, m);
            _show(p->ls, l, m);
            _show(p->rs, m, r);
        _pull(cur, l, r, m);
    }
    void show() {
        _show(root, floor, ceil);
    }
    ptr ptr_root() { return ptr(_buf + root); }
};

struct Tag {
    i64 x = 0;
    operator bool() {
        return x != 0;
    }
    void apply(const Tag &rhs) {
        x += rhs.x;
    }
    void clear() {
        x = 0;
    }
};

struct Info {
    i64 x;
    void apply(const Tag &rhs, i64 l, i64 r) {
        x += (r - l) * rhs.x;
    }
    using op_type = Info;
    static Info op(Info lhs, Info rhs, i64 l, i64 r, i64 m) {
        return {lhs.x + rhs.x};
    }
    static Info op(Info lhs, Info rhs) {
        return {lhs.x + rhs.x};
    }
    static Info op(i64 l, i64 r) {
        // assert(false);
        return {0};
    }
    static Info op(Info rhs) {
        return rhs;
    }
    void set(Info rhs) {
        x = rhs.x;
    }
};

using SegmentTree = segment_tree<Info, Tag>;