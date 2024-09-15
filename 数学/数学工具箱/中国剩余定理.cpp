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

template<typename T>
T inv(i64 a, i64 P) {
    auto [gcd, x, k] = _Exgcd(a, P);
    return (x % P + P) % P;
}

/**
 * 算法：中国剩余定理
 * 作用：求解一元线性同余方程 (x == a (mod P)) 在模n (所有的模积) 的解
 * 限制：所有模互质
 */

template<typename T>
T chineseRemainderTheorem(vector<i64> &a, vector<i64> &P) {
    T n = accumulate(P.begin(), P.end(), (T) 1, multiplies<T>()), ans = 0;

    for (int i = 0; i < (i64) a.size(); ++i) {
        T P1 = n / P[i], b;
        b = inv<T>(P1, P[i]);
        ans = (ans + a[i] * P1 * b % n) % n;
    }
    return (ans % n + n) % n;
}

template<typename T = i64>
array<T, 3> Exgcd(T a, T b, T res) {
    assert(res % __gcd(a, b) == 0);
    auto [gcd, x, y] = _Exgcd(a, b);
    return {gcd, res / gcd * x, res / gcd * y};
}

/**
 * 算法：扩展中国剩余定理
 * 作用：求解一元线性同余方程（ x == a （ mod m ））在模n（所有模的最小公倍数）的解
 * 无限制：所有模互质
*/
template<typename T>
T extendTheChineseRemainderTheorem(vector<i64> &a, vector<i64> &P) {
    T P1 = P[0], a1 = a[0];
    for (int i = 1; i < a.size(); ++i) {
        T P2 = P[i], a2 = a[i];
        auto [gcd, p, q] = Exgcd(P1, P2, a2 - a1);
        a1 = P1 * p + a1;
        P1 = P1 * P2 / gcd;
        a1 = (a1 % P1 + P1) % P1;
    }
    return a1;
}