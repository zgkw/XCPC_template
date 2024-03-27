template<class T>
struct Discretization {
    std::map <T, int> map;
    std::vector<T> Val;
    Discretization () {};
    Discretization (const std::vector<T> &able) {
        init(able);
    }
    void init(const std::vector<T> &able) {
        for (auto u : able) {
            map[u] = 0;
        }
        int tot = 0;
        for (auto &[u, v] : map) {
            v = tot ++;
            Val.push_back(u);
        }
    }
    int id (T x) {
        return map[x];
    }
    int val(int x) {
        return Val[x];
    }
    int size() {
        return Val.size();
    }
};

Discretization<ll> inv;

struct Info {
    ll cnt, len;
    ll x, y;
    short tag;
    void apply(const Info& tag) {
        if (tag.tag == 0) {
            x = tag.x;
            y = tag.y;
            len = (cnt > 0 ? inv.val(y + 1) - inv.val(x) : tag.len);
            this->tag = tag.tag;
        } else if (tag.tag == 1) {
            cnt += tag.cnt;
            len = (cnt > 0 ? inv.val(y + 1) - inv.val(x) : 0ll);
        }
    }
};

Info operator+(const Info& lhs, const Info& rhs) {
    return {0, lhs.len + rhs.len, lhs.x, rhs.y, lhs.tag};
}

using Segmenttree = SegmentTree<Info>;