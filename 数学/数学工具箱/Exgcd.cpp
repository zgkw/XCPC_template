/**
 * 算法：扩展欧几里得算法
 * 作用：求解 ax + by = gcd(a, b)
 * 返回：gcd, x, y
 */
template<typename T = i64>
array<T, 3> _Exgcd(T a, T b) {
    T x1 = 1, x2 = 0, x3 = 0, x4 = 1;
    while (b != 0) {
        T c = a / b;
        std::tie(x1, x2, x3, x4, a, b)
            = std::make_tuple(x3, x4, x1 - x3 * c, x2 - x4 * c, b, a - b * c);
    }
    return {a, x1, x2}; //x = x1, y = x2;
}

/**
 * 算法：扩展欧几里得算法
 * 作用：求解 ax + by = res
 * 限制：gcd(a, b) | res 
 */
template<typename T = i64>
array<T, 3> Exgcd(T a, T b, T res) {
    assert(res % __gcd(a, b) == 0);
    auto [gcd, x, y] = _Exgcd(a, b);
    return {gcd, res / gcd * x, res / gcd * y};
}

/**
 * 算法：线性同余方程
 * 作用：求解 ax == b (mod P)
 *       的最小整数解
 * 要求：gcd(a, P) | b
 */
template<typename T>
T linearCongruenceEquation(i64 a, i64 b, i64 P) {
    auto [gcd, x, k] = Exgcd<T>(a, P, b);
    T t = P / gcd;
    return (x % t + t) % t;
}

/**
 * 算法：扩展欧几里得算法求逆元
 * 作用：求解 ax == 1 ( mod n )的最小整数解
 * 要求：a 与 n 互质
 */
template<typename T>
T inv(i64 a, i64 P) {
    auto [gcd, x, k] = _Exgcd(a, P);
    return (x % P + P) % P;
}