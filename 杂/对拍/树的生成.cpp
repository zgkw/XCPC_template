#include <bits/stdc++.h>
using namespace std;
# ifdef LOCAL
    # include "D:\Microsoft VS Code\debug\debug.h"
# else
    # define debug(...) "YES"
    # define ps "YES"
# endif
using ll = long long;
using i64 = long long;

struct DSU {
    std::vector<int> f, siz;
    
    DSU() {}
    DSU(int n) {
        init(n);
    }
    
    void init(int n) {
        f.resize(n);
        std::iota(f.begin(), f.end(), 0);
        siz.assign(n, 1);
    }
    
    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }
    
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        siz[x] += siz[y];
        f[y] = x;
        return true;
    }
    
    int size(int x) {
        return siz[find(x)];
    }
};

void solve() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto rnd = [&](i64 l, i64 r) {
        uniform_int_distribution<long long> _rnd(l, r);
        return _rnd(rng);
    };

    // 深度为sqrt(n)
    auto sqrt_tree = [&](DSU &dsu, int n) -> void {
        int cnt = 0;
        while(cnt < n - 1) {
            int x = rnd(1, n), y = rnd(1, n);
            if(dsu.merge(x, y)) {
                cout << x << ' ' << y << '\n';
                cnt ++;
            }
        }
    };

    // 链式
    auto chain_tree = [&](DSU &dsu, int n) -> void{
        for(int i = 1; i < n; ++i) {
            cout << i << ' ' << i + 1 << '\n';
            dsu.merge(i, i + 1);
        }
    };

    // 菊花图
    auto flower_tree = [&](DSU &dsu, int n) -> void{
        for(int i = 2; i <= n; ++i) {
            cout << 1 << ' ' << i << '\n';
            dsu.merge(1, i);
        }
    };

    // 深度为log(n)
    auto log_tree = [&](DSU &dsu, int n) -> void{
        for(int i = 2; i <= n; ++i) {
            int x = i, y = rnd(1, i - 1);
            cout << x << ' ' << y << '\n';
            dsu.merge(x, y);
        }
    };

    // 毛毛虫
    auto silkworm = [&](DSU &dsu, int n) -> void{
        int cnt = 0;
        vector<int> chain, tail;
        for(int i = 1; i <= n; ++i) {
            if(rnd(0, 1)) chain.push_back(i);
            else tail.push_back(i);
        }
        if(chain.size() < tail.size()) std::swap(chain, tail);
        for(int i = 1; i < (int)chain.size(); ++i) {
            dsu.merge(chain[i - 1], chain[i]);
            cout << chain[i - 1] << ' ' << chain[i] << '\n';
        }
        for(int i = 0; i < (int)tail.size(); ++i) {
            dsu.merge(chain[i], tail[i]);
            cout << chain[i] << ' ' << tail[i] << '\n';
        }
    };
    

    auto build_tree = chain_tree;
    int n = 1e5;
    DSU dsu(n + 1);
    // 树的下标范围: [1, n]
    build_tree(dsu, n);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);cout.tie(0);
    
    int T = 1;
    //cin >> T;
    while(T--) solve();
    
    return 0;
}
