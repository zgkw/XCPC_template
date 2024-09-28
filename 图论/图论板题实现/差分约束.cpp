# pragma("ofast")
# include <bits/stdc++.h>
using namespace std;

# ifdef LOCAL
    # include "C:\Users\Kevin\Desktop\demo\save\debug.h"
# else
# define debug(...) 114514
# define ps 114514
# endif

using i64 = long long;

void solve () {
    int n, m;
    cin >> n >> m;
    vector<vector<array<int, 2>>> adj(n + 1);
    for (int i = 0; i < m; i += 1) {
        int u, v, w;
        cin >> u >> v >> w;
        // xu <= v + w
        -- u, -- v;
        adj[v].push_back({u, w});
    }
    for (int i = 0; i < n; i += 1) {
        adj[n].push_back({i, 0});
    }
    priority_queue<array<int, 3>, vector<array<int, 3>>, greater<array<int, 3>>> q;
    q.push({0, n, n});
    constexpr int inf = 1e9;
    vector<int> ans(n + 1, inf);
    vector<int> cnt(n + 1);
    int siz = 0;
    while (!q.empty()) {
        siz ++;
        auto [le, u, p] = q.top();
        q.pop();
        if (le < ans[u]) {
            ans[u] = le;
            cnt[u] = cnt[p] + 1;
            if (cnt[u] > n + 1) {
                cout << "NO" << '\n';
                return;
            }
        } else {
            continue;
        }
        for (auto [v, d] : adj[u]) {
            int to = d + le;
            q.push({to, v, u});
        }
    }
    for (int i = 0; i < n; i += 1) {
        cout << ans[i] << ' ';
    }
    cout << '\n';
} 
// 修一下爆没爆int
// 多测

signed main () {
    ios::sync_with_stdio(0);
    cin.tie(0);
	int t = 1;
    // cin >> t;
	while (t --) {
        solve ();
	}
	return 0;
}