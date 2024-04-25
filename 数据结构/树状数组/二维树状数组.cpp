template<typename T>
struct Two_dimensional_Fenwick {
    struct Base_Fenwick {
        int n, m;
        std::vector <std::vector<T>> s;

        Base_Fenwick(int _n = 0, int _m = 0) {
            init(_n, _m);
        }

        void init(int _n, int _m) {
            n = _n, m = _m;
            s.assign(n + 1, std::vector<T>(m + 1, T()));
        }

        void change(int x, int y, const T &v) {
            if (x <= 0 || y <= 0) return;
            if (x > n) x = n;
            if (y > m) y = m;
            for (int i = x; i <= n; i += i & (-i))
                for (int j = y; j <= m; j += j & (-j))
                    s[i][j] += v;
        }

        T Query(int x, int y) {
            if (x <= 0 || y <= 0) return T();
            if (x > n) x = n;
            if (y > m) y = m;
            T ans = 0;
            for (int i = x; i != 0; i -= i & (-i))
                for (int j = y; j != 0; j -= j & (-j))
                    ans += s[i][j];
            return ans;
        }
    };

    int n, m;
    Base_Fenwick A, B, C, D;

    Two_dimensional_Fenwick(int _n = 0, int _m = 0) {
        init(_n, _m);
    }

    void init(int _n, int _m) {
        n = _n, m = _m;
        A.init(n, m);
        B.init(n, m);
        C.init(n, m);
        D.init(n, m);
    }

    void Base_add(int x, int y, int v) {
        A.change(x, y, v);
        B.change(x, y, v * x);
        C.change(x, y, v * y);
        D.change(x, y, v * x * y);
    }

    T Base_Query(int x, int y) {
        return A.Query(x, y) * (x * y + x + y + 1)
               - B.Query(x, y) * (y + 1)
               - C.Query(x, y) * (x + 1)
               + D.Query(x, y);
    }

    void add(int x0, int y0, int x1, int y1, int v) {
        Base_add(x0, y0, v);
        Base_add(x0, y1 + 1, -v);
        Base_add(x1 + 1, y0, -v);
        Base_add(x1 + 1, y1 + 1, v);
    }

    T Query(int x0, int y0, int x1, int y1) {
        return Base_Query(x1, y1) - Base_Query(x0 - 1, y1)
               - Base_Query(x1, y0 - 1) + Base_Query(x0 - 1, y0 - 1);
    }
};