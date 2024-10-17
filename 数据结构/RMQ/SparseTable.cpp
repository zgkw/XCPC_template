template<typename T, class F>
struct SparseTable {
    int n; 
    constexpr static int B = 24;
    array<vector<T>, B> a;
    F merge;
    SparseTable() {}
    SparseTable(const vector<T> &info, F merge) {
        init(info, merge);
    }
    void init(const vector<T> &info, F merge) {
        this->merge = merge;
        n = info.size();
        for (int i = 0; i < B; i += 1) {
            a[i].assign(n, {});
        }
        a[0] = info;
        for (int k = 1; k <= __lg(n); k += 1) {
            for (int i = n - (1 << k); i >= 0; i -= 1) {
                a[k][i] = merge(a[k - 1][i], a[k - 1][i + (1 << k - 1)]);
            }
        }
    }
    T operator() (int l, int r) {
        int k = __lg(r - l);
        return merge(a[k][l], a[k][r - (1 << k)]);
    }
};
