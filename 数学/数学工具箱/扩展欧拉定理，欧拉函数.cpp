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
 * 算法：欧拉函数
 * 作用：求欧拉函数
 * 时间复杂度：O(sqrt(n))
 */
template<typename T = i64>
T Phi(T n) {
    T ans = n;
    for (i64 i = 2; i * i <= n; i++)
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}

/**
 * 算法：扩展欧拉定理（欧拉降幂）
 * 作用：大指数快速幂
 * 时间复杂度：O(sqrt(m))
 */

i64 exEulertheorem(i64 a, string b, i64 P) {
    i64 gcd = __gcd(a, P);
    i64 phi = Phi(P);
    i64 res = 0;
    bool ok = 0;
    for (auto u: b) {
        res = res * 10 + u - '0';
        while (res >= phi) {
            res -= phi;
            if (!ok) ok = 1;
        }
    }
    if (gcd != 1 && ok) res += phi;
    return power(a, res, P);
}