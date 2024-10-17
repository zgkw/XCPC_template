using f64 = double;

// 时间复杂度：O(m * n^2), -1无解，0唯一解，否则无穷解
// a为增广矩阵 行r:[0, n) 列c:[0, m]，a[i][m]为b[0, n)，求解答案为 x[0, m)
int Gauss(vector<vector<f64>> &a, vector<f64> &x, int n, int m, f64 eps = 1e-7){
    int r = 0, c = 0;
    for(r = 0; r < n && c < m; r++, c++) {
        int maxr = r;
        for(int i = r + 1; i < n; i++) {
            if(abs(a[i][c]) > abs(a[maxr][c]))
                maxr = i;
        }
        if(maxr != r) std::swap(a[r], a[maxr]);
        if(fabs(a[r][c]) < eps) {
            r--;
            continue;
        }
        for(int i = r + 1; i < n; i++) {
            if(fabs(a[i][c]) > eps){
                f64 k = a[i][c] / a[r][c];
                for(int j = c; j < m + 1; j++) a[i][j] -= a[r][j] * k;
                a[i][c] = 0;
            }
        }
    } 
    for(int i = r; i < m; i++) {
    	if(fabs(a[i][c]) > eps) return -1;//无解
    }    
    if(r < m) return m - r;//返回自由元个数
    for(int i = m-1; i >= 0; i--) {
        for(int j = i + 1; j < m; j++) a[i][m] -= a[i][j] * x[j];
        x[i] = a[i][m] / a[i][i];
    }
    return 0;//有唯一解
}

struct Complex{
	f64 x, y;
	Complex operator+(const Complex &b) const {
		return Complex({x + b.x, y + b.y});
	}
	Complex operator-(const Complex &b) const {
		return Complex({x - b.x, y - b.y});
	}
	Complex operator*(const Complex &b) const {
		return Complex({x * b.x - y * b.y, x * b.y + y * b.x});
	}
	Complex operator/(const Complex &b) const {
		return Complex({(x * b.x + y * b.y) / (b.x * b.x + b.y * b.y), (y * b.x - x * b.y) / (b.x * b.x + b.y * b.y)});
	}
    f64 mo() {
        return x * x + y * y;
    }
    bool iszero(f64 eps = 1e-7) {
        return mo() < eps;
    }
};
// 时间复杂度：O(m * n^2), -1无解，0唯一解，否则无穷解
// a为增广矩阵 行r:[0, n) 列c:[0, m]，a[i][m]为b[0, n)，求解答案为 x[0, m)
int Gauss(vector<vector<Complex>> &a, vector<Complex> &x, int n, int m, f64 eps = 1e-7){
    int r = 0, c = 0;
    for(r = 0; r < n && c < m; r++, c++) {
        int maxr = r;
        for(int i = r + 1; i < n; i++) {
            if(a[i][c].mo() > a[maxr][c].mo()) 
                maxr = i;
        }
        if(maxr != r) std::swap(a[r], a[maxr]);
        if(a[r][c].iszero(eps)) {
            r--;
            continue;
        }
        for(int i = r + 1; i < n; i++) {
            if(!a[i][c].iszero(eps)) {
                Complex k = a[i][c] / a[r][c];
                for(int j = c; j < m + 1; ++j) a[i][j] = a[i][j] - a[r][j] * k;
                a[i][c] = {0, 0};
            }
        }
    } 
    for(int i = r; i < m; i++) {
        if(!a[i][c].iszero(eps)) return -1;
    }    
    if(r < m) return m - r;//返回自由元个数
    for(int i = m-1; i >= 0; i--) {
        for(int j = i + 1; j < m; j++) a[i][m] = a[i][m] - a[i][j] * x[j];
        x[i] = a[i][m] / a[i][i];
    }
    return 0;//有唯一解
}
