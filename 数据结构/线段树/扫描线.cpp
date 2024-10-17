struct SegmentTree {
    SegmentTree() {}
    struct line {
        int h, l, r, add;
        friend bool operator<(const line &u, const line &v) {
            return u.h < v.h;
        }
    };
    vector<line> a;
    vector<int> pos, len, tag;

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

    void addRange(int x, int y, int l, int r) {
        addRectangle(x, l, y + 1, r + 1);
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

    i64 answer() {
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
};