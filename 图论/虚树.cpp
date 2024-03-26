template<class T,
    class Cmp = std::less<T>>
struct RMQ {
    const Cmp cmp = Cmp();
    static constexpr unsigned B = 64;
    using u64 = unsigned long long;
    int n;
    std::vector<std::vector<T>> a;
    std::vector<T> pre, suf, ini;
    std::vector<u64> stk;
    RMQ() {}
    RMQ(const std::vector<T> &v) {
        init(v);
    }
    void init(const std::vector<T> &v) {
        n = v.size();
        pre = suf = ini = v;
        stk.resize(n);
        if (!n) {
            return;
        }
        const int M = (n - 1) / B + 1;
        const int lg = std::__lg(M);
        a.assign(lg + 1, std::vector<T>(M));
        for (int i = 0; i < M; i++) {
            a[0][i] = v[i * B];
            for (int j = 1; j < B && i * B + j < n; j++) {
                a[0][i] = std::min(a[0][i], v[i * B + j], cmp);
            }
        }
        for (int i = 1; i < n; i++) {
            if (i % B) {
                pre[i] = std::min(pre[i], pre[i - 1], cmp);
            }
        }
        for (int i = n - 2; i >= 0; i--) {
            if (i % B != B - 1) {
                suf[i] = std::min(suf[i], suf[i + 1], cmp);
            }
        }
        for (int j = 0; j < lg; j++) {
            for (int i = 0; i + (2 << j) <= M; i++) {
                a[j + 1][i] = std::min(a[j][i], a[j][i + (1 << j)], cmp);
            }
        }
        for (int i = 0; i < M; i++) {
            const int l = i * B;
            const int r = std::min(1U * n, l + B);
            u64 s = 0;
            for (int j = l; j < r; j++) {
                while (s && cmp(v[j], v[std::__lg(s) + l])) {
                    s ^= 1ULL << std::__lg(s);
                }
                s |= 1ULL << (j - l);
                stk[j] = s;
            } 
        } 
    } 
    T operator()(int l, int r) {
        if (l / B != (r - 1) / B) {
            T ans = std::min(suf[l], pre[r - 1], cmp);
            l = l / B + 1;
            r = r / B;
            if (l < r) {
                int k = std::__lg(r - l);
                ans = std::min({ans, a[k][l], a[k][r - (1 << k)]}, cmp);
            }
            return ans;
        } else {
            int x = B * (l / B);
            return ini[__builtin_ctzll(stk[r - 1] >> (l - x)) + l];
        }
    }
};

struct DFN {
    int n;
    vector<int> dfn, dep, sz, fa;
    RMQ<array<int, 2>> rmq;
    DFN() = default;
    template<class T>
    DFN(const std::vector<std::vector<T>> &adj, T root = 0) {
        init(adj, root);
    }
    template<class T>
    void init(const std::vector<std::vector<T>> &adj, T root = 0) {
        n = adj.size();
        dfn.assign(n, 0);
        dep.assign(n, 0);
        sz.assign(n, 0);
        fa.assign(n, 0);
        virtual_tree.assign(n, {});
        vector<array<int, 2>> inrmq(n);
        int tot = 0;
        auto &pa = fa;
        auto dfs = [&] (auto&&dfs, int now, int fa) -> void {
            dfn[now] = tot ++;
            dep[now] = dep[fa] + 1;
            pa[now] = fa;
            for (auto here : adj[now]) {
                if (here == fa) continue;
                dfs(dfs, here, now);
                sz[now] += sz[here];
            }
            sz[now] += 1;
        };
        dfs(dfs, root, root);
        for (int i = 0; i < n; i += 1) {
            inrmq[dfn[i]] = {dep[i], i};
        }
        rmq.init(inrmq);
    }
    int lca (int lhs, int rhs) {
        if (lhs == rhs) return lhs;
        if (dfn[lhs] > dfn[rhs]) swap(lhs, rhs); 
        return fa[rmq(dfn[lhs] + 1, dfn[rhs] + 1)[1]];
    }
    std::vector<std::vector<int>> virtual_tree;
    std::vector<int> real_key;
    template<class T>
    std::vector<std::vector<int>> &build_virtual_tree(std::vector<T> key) {
        for (auto u : real_key) {
            virtual_tree[u].clear();
        }
        real_key.clear();
        sort(key.begin(), key.end(), [&] (T x, T y) {return dfn[x] < dfn[y];});
        for (int i = 0; i < int(key.size()) - 1; i += 1) {
            real_key.push_back(key[i]);
            real_key.push_back(lca(key[i], key[i + 1]));
        }
        real_key.push_back(key.back());
        sort(real_key.begin(), real_key.end(), [&] (T x, T y) {return dfn[x] < dfn[y];});
        real_key.erase(std::unique(real_key.begin(), real_key.end()), real_key.end());
        for (int i = 0; i < int(real_key.size()) - 1; i += 1 ){
            int Lca = lca(real_key[i], real_key[i + 1]);
            virtual_tree[Lca].push_back(real_key[i + 1]);
            virtual_tree[real_key[i + 1]].push_back(Lca);
        }
        return virtual_tree;
    }
};