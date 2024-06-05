# include <bits/stdc++.h>
using namespace std;

# ifdef LOCAL
    # include "C:\Users\Kevin\Desktop\demo\save\debug.h"
# else
# define debug(...) 114514
# define ps 114514
# endif

using ll = long long;
using i64 = long long;

void solve() {
    int n, m; cin >> n >> m;
    vector<vector<int>> a(n);
    vector<int> in(n);
    for (int i = 0; i < m; i += 1) {
        int u, v; cin >> u >> v; -- u, -- v;
        a[u].push_back(v);
        in[v] ++;
    }
    array<int, 2> cnt{}; int s = 0;
    for (int i = 0; i < n; i += 1) {
        if (a[i].size() != in[i]) {
            if (int(a[i].size()) - in[i] == -1) {
                cnt[0] += 1;
            } else if (int(a[i].size()) - in[i] == 1) {
                cnt[1] += 1; s = i;
            } else {
                cout << "No" << endl;
                return;
            }
        }
    }
    for (auto i : {0, 1}) {
        if (cnt[i] > 1) {
            cout << "No" << endl;
            return;
        }
    }
    vector<int> cur(n);
    vector<int> seq;
    auto dfs = [&] (auto&&dfs, int now) -> void {
        // ps;
        if (cur[now] == 0) sort(a[now].begin() , a[now].end());
        for (int &i = cur[now]; i < a[now].size();) {
            dfs(dfs, a[now][i ++]);
        }
        seq.push_back(now);
    };
    dfs(dfs, s);
    reverse(seq.begin(), seq.end());
    for (auto u : seq) {
        cout << u + 1 << ' ';
    }
    cout << endl;
}

signed main () {
# ifndef cin 
    ios::sync_with_stdio (false);
    cin.tie (nullptr) ;
# endif
    // __fin("C:\\Users\\Kevin\\Desktop\\cpp\\in.in") ;

	i64 _ = 1 ;
    // cin >> _ ;
	while (_ --) {
        // debug(_);
        solve ();
	}
	return 0 ;
}