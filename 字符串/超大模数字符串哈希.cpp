std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

struct StringHash {
    constexpr static i64 P = i64(1E18) + 7;
    static int B;
    vector<i64> p, h;

    StringHash() = default;

    i64 mul(i64 u, i64 v) {
        return __int128_t(u) * v % P;
    }

    i64 norm(i64 u) {
        if (u >= P) {
            u -= P;
        }
        if (u < 0) {
            u += P;
        }
        return u;
    }

    StringHash(const string& s) {
        int n = s.size();
        p.resize(n);
        h.resize(n);
        p[0] = 1;
        h[0] = s[0];
        for (int i = 1; i < n; i += 1) {
            p[i] = mul(p[i - 1], B);
            h[i] = norm(mul(h[i - 1], B) + s[i]);
        }
    }

    i64 operator()(int l, int r) {
        assert(l <= r);
        i64 ans = 0;
        if (l == 0) {
            return h[r];
        } else {
            return norm(h[r] - mul(h[l - 1], p[r - l + 1]));
        }
        return ans;
    }
};

int StringHash::B = rng() % 1'000'000'000 + 123456789;