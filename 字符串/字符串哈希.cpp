using i64 = long long;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

bool isprime(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

int findPrime(int n) {
    while (!isprime(n))
        n++;
    return n;
}

template <int N>
struct StringHash {
    static array<int, N> mod;
    static array<int, N> base;
    vector<array<i64, N>> p, h;

    StringHash() = default;

    StringHash(const string& s) {
        int n = s.size();
        p.resize(n);
        h.resize(n);
        fill(p[0].begin(), p[0].end(), 1);
        fill(h[0].begin(), h[0].end(), 1);
        for (int j = 0; j < N; j++) {
            h[0][j] = s[0] % mod[j];
            p[0][j] = 1;
        }
        for (int i = 1; i < n; i++)
            for (int j = 0; j < N; j++) {
                p[i][j] = 1ll * p[i - 1][j] * base[j] % mod[j];
                h[i][j] = ((1ll * h[i - 1][j]) * base[j] + s[i]) % mod[j];
            }
    }

    array<int, N> query(int l, int r) {
        array<int, N> ans{};
        if (l > r)
            return {0, 0};
        for (int i = 0; i < N; i++) {
            ans[i] =
                (1ll * h[r][i] -
                 1ll * (l == 0 ? 0ll : h[l - 1][i]) * p[r - l + 1][i] % mod[i] +
                 mod[i]) %
                mod[i];
        }
        return ans;
    }
};

constexpr int HN = 2;
template <>
array<int, 2> StringHash<HN>::mod = {findPrime(rng() % 900000000 + 100000000),
                                     findPrime(rng() % 900000000 + 100000000)};
template <>
array<int, 2> StringHash<HN>::base{13331, 131};
using Hashing = StringHash<HN>;

