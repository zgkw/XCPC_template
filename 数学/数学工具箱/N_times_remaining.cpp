/**
 * 算法：n次剩余
 * 作用：求解 x ^ a = b ( mod m )
 * 要求: m是质数
 * 返回：x，无解返回-1e15
 * 建议使用自定义Hash
*/
using i64 = long long;
i64 mul(i64 a, i64 b, i64 m) {
    return static_cast<__int128>(a) * b % m;
}
template<class T = i64>
constexpr T power(T a, i64 b) {
    T res = 1;
    for (; b; b /= 2, a *= a)
        if (b % 2) res *= a;
    return res;
}

i64 power(i64 a, i64 b, i64 m) {
    i64 res = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1)
            res = mul(res, a, m);
    return res;
}
std::vector <i64> n_times_remaining(i64 a, i64 b, i64 m) {
    b %= m;
    vector<array<i64, 3>> fs;
    [&] (i64 m) {
        for (i64 i = 2; i * i <= m; i += 1) {
            if (m % i == 0) {
                array<i64, 3> f{i, 1, 0};
                while(m % i == 0) m /= i, f[1] *= i, f[2] += 1;
                fs.push_back(f);
            }
        }
        if (m > 1) fs.push_back({m, m, 1});
    }(m);
    auto get_Step = [&] (i64 a, i64 n, i64 mod) {//求阶
        i64 ans = n;
        for (i64 i = 2; i * i <= n; i++)
            if (n % i == 0) {
                while (ans % i == 0 && power(a, ans / i, mod) == 1) ans /= i;
                for (; n % i == 0; n /= i);
            }
        if (power(a, ans / n, mod) == 1)ans /= n;
        return ans;
    };

    i64 ans = 1;
    auto cntor = [&] (i64 A, i64 B, i64 m, i64 phi) {
        i64 c = get_Step(B, phi, m), y = phi / c, G = __gcd(A, phi);
        if (y % G) ans = 0; ans *= G;
    };
    for (auto [p, pt, t] : fs) {
        if (!ans) break;
        if (b % pt == 0) ans *= power(p, t - (t + a - 1) / a, 1e9);
        else {
            i64 Z = 0, b0 = b;
            for (; b0 % p == 0; Z ++, pt /= p, t--, b0 /= p);
            if (Z % a) ans = 0;
            else {
                cntor(a, b0, pt, pt - pt / p);
                ans *= power(p, Z - Z / a, 1e9);
            }
        }
    }
    return std::vector<i64>{ans};
}
