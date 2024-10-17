struct intervalRepulsion {
    intervalRepulsion() {}
    struct line {
        int h, l, r, add;
        friend bool operator<(const line &u, const line &v) {
            return u.h < v.h;
        }
    };
    vector<line> a;
    vector<int> pos, len, tag;
    vector<array<int, 2>> del; // triangle

    void reserve(int n) {
        a.reserve(2 * n), pos.reserve(2 * n);
    }
    
    // 左下和右上 在笛卡尔坐标系中
    void addRectangle(int x, int l, int y, int r) {
        a.emplace_back(x, l, r, 1);
        a.emplace_back(y, l, r, -1);
        pos.push_back(l);
        pos.push_back(r);
    }

    void addRange(int u, int v, int x, int y) {
        if (!(u <= v && x <= y)) {
            return;
        }
        if (x < v) {
            int le = v - x + 1;
            int l = x, r = v;
            le = y - x;
            del.push_back({v - le, le});
        }
        addRectangle(u, x, v + 1, y + 1);
    }

    void pull(int p, int l, int r) {
        if (tag[p]) len[p] = pos[r + 1] - pos[l];
        else len[p] = len[p << 1] + len[p << 1 | 1];
    }

    void modify(int p, int l, int r, int x, int y, int v) {
        if (x <= pos[l] && pos[r + 1] <= y) {
            tag[p] += v;
            pull(p, l, r);
            return;
        }
        int m = l + r >> 1;
        if (x <= pos[m])
            modify(p << 1, l, m, x, y, v);
        if (pos[m + 1] < y)
            modify(p << 1 | 1, m + 1, r, x, y, v);
        pull(p, l, r);
    }

    i64 calc1() {
        if (a.empty()) return 0LL;
        i64 ans = 0;
        int n = a.size();
        sort(a.begin(), a.end());
        sort(pos.begin(), pos.end());
        int m = unique(pos.begin(), pos.end()) - pos.begin();
        len.assign(8 * m, 0);
        tag.assign(8 * m, 0);
        for (int i = 0; i < n - 1; i += 1) {
            modify(1, 0, m - 2, a[i].l, a[i].r, a[i].add);
            ans += 1LL * len[1] * (a[i + 1].h - a[i].h);
        }
        return ans;
    }

    i64 calc2(int L, int R) {
        sort(del.begin(), del.end());
        del.push_back({R + 1, 0});
        int pos = L - 1, siz = 0;
        auto calc = [&] (int x, int y, int h) {
            return 1LL * (x + y) * h / 2;
        };
        i64 ans = 0;
        for (auto [l, le] : del) {
            if (l + le <= pos + siz) {
                continue;
            } else {
                if (pos + siz - 1 < l) {
                    ans += calc(siz, 1, siz);
                } else {
                    int h = l - pos;
                    ans += calc(siz, siz - h + 1, h);
                }
                pos = l, siz = le;
            }
        }
        return ans;
    }

    // 左右区间范围
    i64 answer(int L, int R) {
        i64 le = R - L + 1;
        i64 ans = le * (le + 1) / 2;
        ans = ans - calc1() + calc2(L, R);
        return ans;
    }
};