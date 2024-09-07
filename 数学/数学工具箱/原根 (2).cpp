struct Sieves {
    int n;
    vector<int> Prime, Euler, Morbius, Approximate, Approximate_cnt;
    vector<bool> notprime;
    vector<array<i64, 2>> div;

    Sieves() {};

    Sieves(int _n) { init(_n); };

    void init(int _n) {
        n = _n;
        Prime_work();
    }

    void Prime_work() {
        notprime.assign(n + 1, 0);
        notprime[0] = 1;
        notprime[1] = 1;
        for (i64 i = 2; i <= n; ++i) {
            if (notprime[i] == 0) {
                Prime.push_back(i);
            }
            for (i64 j = 0; i * Prime[j] <= n; ++j) {
                notprime[i * Prime[j]] = 1;

                if (i % Prime[j] == 0) break;
            }
        }
    }

    void Euler_work() {
        Euler.assign(n + 1, 0);
        Euler[1] = 1;
        for (i64 i = 2; i <= n; ++i) {
            if (notprime[i] == 0) Euler[i] = i - 1;
            for (i64 j = 0; i * Prime[j] <= n; ++j) {
                i64 now = i * Prime[j];
                if (i % Prime[j] != 0) {
                    Euler[now] = (Prime[j] - 1) * Euler[i];
                } else {
                    Euler[now] = Prime[j] * Euler[i];
                    break;
                }
            }
        }
    }

    void Morbius_work() {
        Morbius.assign(n + 1, 0);
        Morbius[1] = 1;
        for (i64 i = 2; i <= n; ++i) {
            if (notprime[i] == 0) Morbius[i] = -1;
            for (i64 j = 0; i * Prime[j] <= n; ++j) {
                i64 now = i * Prime[j];
                if (i % Prime[j] != 0) {
                    Morbius[now] = -Morbius[i];
                } else break;
            }
        }
    }

    void Div_work() {
        div.resize(n + 1);
        div[0] = {1, 1};
        div[1] = {1, 1};
        for (i64 i = 2; i <= n; ++i) {
            if (notprime[i] == 0) {
                div[i] = {1, i};
            }
            for (i64 j = 0; i * Prime[j] <= n; ++j) {
                div[i * Prime[j]] = {i, Prime[j]};
                if (i % Prime[j] == 0) break;
            }
        }
    }

/**
 * 求约数个数
*/
    void Approximate_work() {
        Approximate.assign(n + 1, 0);
        Approximate_cnt.assign(n + 1, 0);
        Approximate[1] = 1;
        Approximate_cnt[1] = 0;
        for (i64 i = 2; i <= n; ++i) {
            if (notprime[i] == 0) {
                Approximate[i] = 2;
                Approximate_cnt[i] = 1;
            }
            for (i64 j = 0; i * Prime[j] <= n; ++j) {
                i64 now = i * Prime[j];
                if (i % Prime[j] != 0) {
                    Approximate_cnt[now] = 1;
                    Approximate[now] = Approximate[i] * 2;
                } else {
                    Approximate_cnt[now] = Approximate_cnt[i] + 1;
                    Approximate[now] = Approximate[i] / Approximate_cnt[now] * (Approximate_cnt[now] + 1);
                    break;
                }
            }
        }
    }

    std::vector<i64> get_frac(i64 x) {
        vector<i64> f;
        for (; x > 1; f.push_back(div[x][0]), x = div[x][1]);
        return f;
    }

    i64 size() { return (i64) Prime.size(); }

    bool isprime(int n) { return !notprime[n]; }

    i64 eu(int n) { return Euler[n]; }

    i64 mo(int n) { return Morbius[n]; }
};

template<typename T>
T power(T a, i64 b, i64 P) {
    T res = 1;
    for (; b; b >>= 1) {
        if (b & 1) {
            res = 1ll * res * a % P;
        }
        a = 1ll * a * a % P;
    }
    return res;
}

/**
 * 求一个数的所有原根
 * 时间复杂度：O(sqrt(m))
 */
vector<i64> primitive_root(i64 n) {
    Sieves s(n);
    s.Euler_work();
    vector<bool> exist(n + 1);
    exist[2] = 1;
    exist[4] = 1;
    for (i64 p : s.Prime) {
        if ((p & 1) == 0) continue;
        for (i64 now = p; now < exist.size(); now *= p) {
            exist[now] = 1;
            if (now * 2 < exist.size())
                exist[now * 2] = 1;
        }
    }
    if (!exist[n]) return vector<i64>();
    vector <i64> f;
    i64 phi = s.eu(n);
    i64 pphi = s.eu(phi);
    i64 m = phi;
    for (int i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            f.push_back(i);
            while (m % i)
                m /= i;
        }
    }
    if (m != 1) f.push_back(m);
    i64 root = -1;
    auto check = [&](i64 x) {
        for (auto u: f)
            if (power(x, phi / u, n) == 1)
                return false;
        root = x;
        return true;
    };
    for (i64 i = 1;; ++i) {
        if (__gcd(i, n) != 1) continue;
        if (check(i)) break;
    }
    vector <i64> ans;
    for (i64 now = root, i = 1; i <= phi; ++i) {
        if (__gcd(phi, i) == 1)
            ans.push_back(now);
        now = (now * root) % n;
    }
    sort(ans.begin(), ans.end());
    return ans;
}