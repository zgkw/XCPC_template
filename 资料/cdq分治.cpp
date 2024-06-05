# include <bits/stdc++.h>
using namespace std;

# ifdef LOCAL
    # include "C:\Users\Kevin\Desktop\demo\save\debug.h"
# else
# define debug(...) 114514
# define ps 114514
# endif

void solve() {
    int n, k; cin >> n >> k;
    array<vector<array<int, 4>>, 3> a;
    for (auto &u : a) {
        u.assign(n, {});
    }
    for (int tot = 0; auto &[a, b, c, id] : a[0]) {
        cin >> a >> b >> c; id = tot ++;
    }
    sort(a[0].begin(), a[0].end());
    vector<bool> isnext(n);
    vector<int> ans(n), d(n);
    for (int i = n - 2; i >= 0; i -= 1) {
        if (a[0][i][0] == a[0][i + 1][0] && a[0][i][1] == a[0][i + 1][1] && a[0][i][2] == a[0][i + 1][2]) {
            ans[a[0][i][3]] = ans[a[0][i + 1][3]] + 1;
        }
    }
    auto dfs1 = [&] (auto &&dfs, int l, int r) -> void {
        if (r - l == 1) {
            return;
        }
        int m = (l + r) / 2;
        dfs(dfs, l, m); dfs(dfs, m, r);
        // debug(l, r);
        int cnt = 0;
        for (int i = l, j = m; i < m || j < r;) {
            if (j == r || (i < m && a[1][i][2] <= a[1][j][2])) {
                if (!isnext[a[1][i][3]]) cnt += 1;
                a[2][i + j - m] = a[1][i];
                i += 1;
            } else {
                if (isnext[a[1][j][3]]) ans[a[1][j][3]] += cnt;
                a[2][i + j - m] = a[1][j];
                j += 1;
            }
        }
        for (int i = l; i < r; i += 1) {
            a[1][i] = a[2][i];
        }
    };
    auto dfs = [&] (auto &&dfs, int l, int r) -> void {
        if (r - l == 1) {
            return;
        }
        int m = (l + r) / 2;
        dfs(dfs, l, m); dfs(dfs, m, r);

        for (int i = l, j = m; i < m || j < r;) {
            if (j == r || (i < m && a[0][i][1] <= a[0][j][1])) {
                isnext[a[0][i][3]] = 0;
                a[1][i + j - m] = a[0][i]; 
                i += 1;
            } else {
                isnext[a[0][j][3]] = 1;
                a[1][i + j - m] = a[0][j];
                j += 1;
            }
        }
        for (int i = l; i < r; i += 1) {
            a[0][i] = a[1][i];
        }
        dfs1(dfs1, l, r);
    };
    dfs(dfs, 0, n);
    // debug(ans);
    for (int i = 0; i < n; i += 1) d[ans[i]] ++;
    for (int i = 0; i < n; i += 1) {
        cout << d[i] << '\n';
    }
}
 
signed main () {
# ifndef cin 
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
# endif
    // fin("C:\\Usech[1]\\Kevin\\Desktop\\cpp\\in.in") ;

	int _ = 1;
    // cin >> _;
	while (_ --) {
        // debug();
        solve ();
	}
	return 0 ;
}