using i64 = long long;
// 时间复杂度 O(n^3 + n^2 * logp)
// 行列式 mod p， a[1, n][1, n]
constexpr int calcDet(vector<vector<int>> &a, int n, const int p) {
    i64 zf = 1, ans = 1, tmp = 0;

    for(int i = 1; i <= n; ++i) 
        for(int j = 1; j <= n; ++j) 
            a[i][j] %= p;
	
	for (int i = 1; i <= n; i++) {
		int k = i;
		for (int j = i + 1; j <= n; j++)
			if (a[j][i] > a[k][i]) {
				k = j;
			}
		if (!a[k][i]) return 0;
		
		if (k != i) swap(a[i], a[k]), zf = -zf;
		
		for (int j = i + 1; j <= n; j++) {
			if (a[j][i] > a[i][i]) swap(a[i], a[j]), zf = -zf;
			while (a[j][i]) {
				tmp = a[i][i] / a[j][i];
				for (int k = i; k <= n; k++)
					a[i][k] = (a[i][k] + a[j][k] * (p - tmp) % p) % p;
				swap(a[i], a[j]), zf = -zf;
			}
		}
		ans = ans * a[i][i] % p;
	}
	
	if (zf == -1) ans = (-ans + p) % p;
	return ans;
}

// 时间复杂度 O(n^3)
// 行列式 a[0, n)[0, n)
constexpr double calcDet(vector<vector<double>> &a, int n, const double eps = 1e-9) {
    double det = 1;
    for (int i = 0; i < n; ++i) {
        int k = i;
        for (int j = i + 1; j < n; ++j)
            if (abs(a[j][i]) > abs(a[k][i])) k = j;
        if (abs(a[k][i]) < eps) {
            det = 0;
            break;
        }
        swap(a[i], a[k]);
        if (i != k) det = -det;
        det *= a[i][i];
        for (int j = i + 1; j < n; ++j) a[i][j] /= a[i][i];
        for (int j = 0; j < n; ++j)
            if (j != i && abs(a[j][i]) > eps)
                for (int k = i + 1; k < n; ++k) a[j][k] -= a[i][k] * a[j][i];
    }
    return det;
}
