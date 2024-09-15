/**
 * 算法：扩展BSGS
 * 作用：求解 a ^ x = b ( mod m )
 * 无要求: a与m互质
 * 返回：问题的最小非负x，无解返回-1
 * 建议使用自定义Hash
*/
using i64 = long long;
using ui64 = unsigned long long;
constexpr i64 exBSGS(i64 a, i64 b, i64 m, i64 k = 1) {
    constexpr i64 inf = 1e15;
    auto BSGS = [&](i64 a, i64 b, i64 m, i64 k = 1) {
# ifdef _Hash
        unordered_map <ui64, ui64, Hash> map;
# else
        std::map <ui64, ui64> map;
# endif
        i64 cur = 1, t = sqrt(m) + 1;
        for (i64 B = 1; B <= t; ++B) {
            (cur *= a) %= m;
            map[b * cur % m] = B;
        }
        ll now = cur * k % m;
        for (i64 A = 1; A <= t; ++A) {
            auto it = map.find(now);
            if (it != map.end())
                return A * t - (i64) it->second;
            (now *= cur) %= m;
        }
        return -inf; // 无解
    };
    i64 A = a %= m, B = b %= m, M = m;
    if (b == 1) return 0;
    i64 cur = 1 % m;
    for (int i = 0;; i++) {
        if (cur == B) return i;
        cur = cur * A % M;
        i64 d = __gcd(a, m);
        // if (b % d) return -inf;
        if(b % d) return -1;
        if (d == 1) {
            auto ans = BSGS(a, b, m, k * a % m);
            if (ans == -inf) return -1;
            else return ans + i + 1;
        }
        k = k * a / d % m, b /= d, m /= d;
    }
}
