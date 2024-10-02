struct ScanLine {
    int n;
    struct Line {
        int x1, x2, y;
        int type;
        bool operator<(Line another) const { return y < another.y; }
    };

    struct Info {
        int l, r;
        int len = 0, cnt = 0;
    };
    vector<Info> info;
    vector<Line> line;
    vector<int> X;

    void add(int x1, int y1, int x2, int y2) {
        line.push_back({x1, x2, y1, 1});
        line.push_back({x1, x2, y2, -1});
        X.push_back(x1);
        X.push_back(x2);
    }

    int work() {
        if (line.size() == 0) {
            int res = 0;
            return res;
        }
        sort(line.begin(), line.end());
        sort(X.begin(), X.end());
        int tot = unique(X.begin(), X.end()) - X.begin();
        vector<Info> init_;
        for (int i = 0; i < tot - 1; i++) {
            init_.push_back({i + 1, i + 1, 0, 0});
        }
        init(init_);
        int ans = 0;
        for (int i = 0; i < (int)line.size() - 1; i++) {
            modify(1, line[i].x1, line[i].x2, line[i].type);
            ans += info[1].len * (line[i + 1].y - line[i].y);
        }
        return ans;
    }

    ScanLine() : n(0) {};

    void init(const vector<Info>& _init) {
        n = (int)_init.size();
        info.assign(n * 25, Info());
        function<void(int, int, int)> build = [&](int p, int l, int r) {
            info[p].l = l;
            info[p].r = r;
            if (l == r) {
                info[p] = _init[l - 1];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m + 1, r);
            pull(p);
        };
        build(1, 1, n);
    }

    void pull(int p) {
        if (info[p].cnt) {
            info[p].len = X[info[p].r] - X[info[p].l - 1];
        } else {
            info[p].len = info[2 * p].len + info[2 * p + 1].len;
        }
    }

    void modify(int p, int L, int R, int val) {
        int l = info[p].l;
        int r = info[p].r;
        if (X[r] <= L || R <= X[l - 1]) {
            return;
        }
        if (L <= X[l - 1] && X[r] <= R) {
            info[p].cnt += val;
            pull(p);
            return;
        }
        modify(2 * p, L, R, val);
        modify(2 * p + 1, L, R, val);
        pull(p);
    }
};
