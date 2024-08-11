// 时间复杂度：O(m * n^2), -1无解，0唯一解，否则无穷解
// a为增广矩阵 行r:[0, n) 列c:[0, m]，a[i][m]为b[0, n)，求解答案为 x[0, m)
int Gauss(vector<vector<double>> &a, vector<double> &x, int n, int m, double eps = 1e-7){
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
                double k = a[i][c] / a[r][c];
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
