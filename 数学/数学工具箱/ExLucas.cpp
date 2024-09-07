using i64 = long long;
i64 mul(i64 a, i64 b, i64 P) {
    return static_cast<__int128>(a) * b % P;
}
i64 power(i64 a, i64 b, i64 P) {
    i64 res = 1 % P;
    for (; b; b >>= 1, a = mul(a, a, P))
        if (b & 1)
            res = mul(res, a, P);
    return res;
}
template<typename T = i64>
constexpr array<T, 3> Exgcd(T a, T b) {
    T x1 = 1, x2 = 0, x3 = 0, x4 = 1;
    while (b != 0) {
        T c = a / b;
        std::tie(x1, x2, x3, x4, a, b) =
                std::make_tuple(x3, x4, x1 - x3 * c, x2 - x4 * c, b, a - b * c);
    }
    return {a, x1, x2}; //x = x1, y = x2;
}
template<typename T = i64>
constexpr array<T, 3> __Exgcd(T a, T b, T res) {
    assert(res % __gcd(a, b) == 0);
    auto [gcd, x, y] = Exgcd(a, b);
    return {gcd, res / gcd * x, res / gcd * y};
}
template<typename T = i64>
constexpr T inv(i64 a, i64 mod) {
    auto [gcd, x, k] = Exgcd<T>((T) a, (T) mod);
    return (x % mod + mod) % mod;
}
template<typename T = i64>
constexpr T Extend_the_Chinese_remainder_theorem
        (vector <i64> &a, vector <i64> &m) {
    T m1 = m[0], a1 = a[0];
    for (int i = 1; i < (i64) a.size(); ++i) {
        T m2 = m[i], a2 = a[i];
        auto [gcd, p, q] = __Exgcd(m1, m2, a2 - a1);
        a1 = m1 * p + a1;
        m1 = m1 * m2 / gcd;
        a1 = (a1 % m1 + m1) % m1;
    }
    return a1;
}
// 扩展卢卡斯定理，O(P logP)
i64 Exlucas(i64 n, i64 m, i64 P) {
    std::vector <i64> p, a;
    function <i64(i64, i64, i64)> calc = [&](i64 n, i64 x, i64 P) mutable -> i64 {
        if (!n) return 1;
        i64 s = 1;
        for (i64 i = 1; i <= P; ++i)  //求阶乘，可预处理降低复杂度
            if (i % x != 0) s = mul(s, i, P);
        s = power(s, n / P, P);
        for (i64 i = n / P * P + 1; i <= n; ++i)
            if (i % x != 0) s = mul(i, s, P);
        return mul(s, calc(n / x, x, P), P);
    };
    function <i64(i64, i64, i64, i64)> multilucas = [&](i64 n, i64 m, i64 x, i64 P) -> i64 {
        i64 cnt = 0;
        for (i64 i = n; i != 0; i /= x) cnt += i / x;
        for (i64 i = m; i != 0; i /= x) cnt -= i / x;
        for (i64 i = n - m; i != 0; i /= x) cnt -= i / x;
        return static_cast<__int128>(1) * power(x, cnt, P) % P * calc(n, x, P) % P
                * inv(calc(m, x, P), P) % P * inv(calc(n - m, x, P), P) % P;
    };
    for (i64 i = 2; i * i <= P; ++i) {
        if (P % i == 0) {
            p.emplace_back(1);
            while (P % i == 0) p.back() *= i, P /= i;
            a.emplace_back(multilucas(n, m, i, p.back()));
        }
    }
    if (P > 1) p.emplace_back(P), a.emplace_back(multilucas(n, m, P, P));
    return Extend_the_Chinese_remainder_theorem(a, p);
}
