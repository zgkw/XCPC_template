// 字符串哈希
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
    vector<array<int, N>> p, h;
    StringHash() = default;
    StringHash(const string& s) {
        int n = s.size();
        p.resize(n);
        h.resize(n);
        fill(p[0].begin(), p[0].end(), 1);
        fill(h[0].begin(), h[0].end(), 1);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < N; j++) {
                p[i][j] = 1ll * (i == 0 ? 1ll : p[i - 1][j]) * base[j] % mod[j];
                h[i][j] =
                    (1ll * (i == 0 ? 0ll : h[i - 1][j]) * base[j] + s[i]) %
                    mod[j];
            }
    }
    array<int, N> query(int l, int r) {
        assert(r >= l - 1);
        array<int, N> ans{};
        if (l > r)
            return {0, 0};
        for (int i = 0; i < N; i++) {
            ans[i] = (h[r][i] -
                      1ll * (l == 0 ? 0ll : h[l - 1][i]) *
                          (r - l + 1 == 0 ? 1ll : p[r - l][i]) % mod[i] +
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

// kmp

// 获得nxt数组
auto get_nxt(string s) {  // 记得手动 s = ' ' + s
    int n = (int)s.size() - 1;
    vector<int> nxt(n + 1);
    nxt[1] = 0;
    for (int i = 2; i <= n; i++) {
        nxt[i] = nxt[i - 1];
        while (nxt[i] && s[i] != s[nxt[i] + 1])
            nxt[i] = nxt[nxt[i]];
        nxt[i] += (s[i] == s[nxt[i] + 1]);
    }
    return nxt;
}

// 找到模式串在文本串中所有出现的位置(注意:1-index)
auto find(string t, string p) {  // p模式,t文本
    int n = t.size();
    int m = p.size();
    vector<int> res;
    auto v = get_nxt(' ' + p + '$' + t);
    for (int i = m + 2; i <= n + m + 1; i++) {
        if (v[i] == m) {
            res.push_back(i - 2 * m);
        }
    }
    return res;
}

// 找到模式串在文本串中所有出现的位置(注意:1-index)
auto find(string t, vector<int> nxt, string p) {  // p模式,t文本
    int n = (int)t.size() - 1;
    int m = (int)p.size() - 1;
    vector<int> res;
    int j = 0;
    for (int i = 1; i <= n; i++) {
        while (j && t[i] != p[j + 1])
            j = nxt[j];
        j += (t[i] == p[j + 1]);
        if (j == m) {
            res.push_back(i - m + 1);
            j = nxt[j];
        }
    }
    return res;
}

// 子序列自动机
// 获得nxt
auto get_nxt_subseq(string s) {
    int n = (int)s.size() - 1;
    vector<vector<int>> nxt(n + 2, vector<int>(26, n + 1));
    for (int i = n; i >= 0; i--) {
        for (int j = 0; j < 26; j++) {
            if (i == n)
                nxt[i][j] = n + 1;
            else
                nxt[i][j] = nxt[i + 1][j];
        }
        if (i != n)
            nxt[i][s[i + 1] - 'a'] = i + 1;
    }
    return nxt;
}

// z函数
// Z函数：S与S[i~n]（S的每个后缀）的LCP长度

auto get_z(string s) {
    int n = (int)s.size() - 1;
    vector<int> z(n + 1);
    z[1] = n;
    int l = 0, r = 0;
    for (int i = 2; i <= n; i++) {
        if (i <= r)
            z[i] = min(z[i - l + 1], r - i + 1);
        while (s[1 + z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}  // 获得模式串的Z函数

auto get_p(string s, string t) {
    int m = (int)t.size() - 1;
    int n = (int)s.size();
    vector<int> p(m + 1);
    auto z = get_z(s);
    int l = 0, r = 0;
    for (int i = 1; i <= m; i++) {
        if (i <= r)
            p[i] = min(z[i - l + 1], r - i + 1);
        while (1 + p[i] <= n && i + p[i] <= m && s[1 + p[i]] == t[i + p[i]])
            p[i]++;
        if (i + p[i] - 1 > r)
            l = i, r = i + p[i] - 1;
    }
    return p;
}  // 获得S与T的每个后缀的LCP长度数组p

auto get_p(vector<int> z, string s, string t) {
    int m = (int)t.size() - 1;
    int n = (int)s.size() - 1;
    vector<int> p(m + 1);
    int l = 0, r = 0;
    for (int i = 1; i <= m; i++) {
        if (i <= r)
            p[i] = min(z[i - l + 1], r - i + 1);
        while (1 + p[i] <= n && i + p[i] <= m && s[1 + p[i]] == t[i + p[i]])
            p[i]++;
        if (i + p[i] - 1 > r)
            l = i, r = i + p[i] - 1;
    }
    return p;
}  // 获得S与T的每个后缀的LCP长度数组p


//manacher

std::vector<int> manacher(std::string s) {
    std::string t = "#";
    for (auto c : s) {
        t += c;
        t += '#';
    }
    int n = t.size();
    std::vector<int> r(n);
    for (int i = 0, j = 0; i < n; i++) {
        if (2 * j - i >= 0 && j + r[j] > i) {
            r[i] = std::min(r[2 * j - i], j + r[j] - i);
        }
        while (i - r[i] >= 0 && i + r[i] < n && t[i - r[i]] == t[i + r[i]]) {
            r[i] += 1;
        }
        if (i + r[i] > j + r[j]) {
            j = i;
        }
    }
    return r;
}
