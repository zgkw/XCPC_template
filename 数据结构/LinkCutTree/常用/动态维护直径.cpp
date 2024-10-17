struct Info {
    i64 x;
    i64 ans = 0;
    array<i64, 2> max{};
    i64 sum;
    multiset<i64> set;
    multiset<i64> vans;
    void reve() {
        swap(max[0], max[1]);
    }
    void modify(const Info& rhs) {
        x = rhs.x;
    }
    void vup(const Info &rhs) {
        // debug(rhs.max[0]);
        if (rhs.max[0] != 0) {
            set.insert(rhs.max[0]);
        }
        if (rhs.ans != 0) {
            vans.insert(rhs.ans);
        }
    }
    void rv(const Info &rhs) {
        if (rhs.max[0] != 0) {
            set.erase(set.find(rhs.max[0]));
        }
        if (rhs.ans != 0) {
            vans.erase(vans.find(rhs.ans));
        }
    }
    void up(const Info &lhs, const Info &rhs) {
        sum = lhs.sum + rhs.sum + x;
        i64 F = 0, S = 0;
        if (set.size()) {
            auto it = set.rbegin();
            F = *it;
            if (set.size() >= 2) {
                it ++;
                S = *it;
            }
        }
        i64 R = std::max(F, rhs.max[0]);
        i64 L = std::max(F, lhs.max[1]);
        max[0] = std::max(lhs.max[0], lhs.sum + x + R);
        max[1] = std::max(rhs.max[1], rhs.sum + x + L);
        array<i64, 4> vec{F, S, lhs.max[1], rhs.max[0]};
        sort(vec.rbegin(), vec.rend());
        ans = std::max({vec[0] + vec[1] + x, lhs.ans, rhs.ans});
        if (!vans.empty()) {
            ans = std::max(ans, *vans.rbegin());
        }
    }
    void clear() {}
# ifndef ONLINE_JUDGE
    friend ostream &operator<<(ostream &cout, Info u) {
        return cout << u.x << ' ' << u.ans << ' ' << u.set << ' ' << u.max;
    }
# endif
};