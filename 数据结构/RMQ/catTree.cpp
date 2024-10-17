template<typename T, class F> // std::function 会比匿名函数 常数大
struct catTree {
    static constexpr int B = 24;
    int n;
    array<vector<T>, B> a;
    F merge;
    catTree() {}
    catTree(const vector<T> &_init, F merge) {
        init(_init, merge);
    }
    void init(const vector<T> &_init, F merge) {
        this->merge = merge;
        n = _init.size();
        a[0] = _init;
        for (int k = 1, w = 4; k <= __lg(n); k += 1, w <<= 1) {
            a[k].assign(n, {});
            for (int l = 0, mid = w / 2, r = std::min(w, n); mid < n; l += w, mid += w, r = std::min(r + w, n)) {
                a[k][mid - 1] = a[0][mid - 1];
                for (int i = mid - 2; i >= l; i -= 1) {
                    a[k][i] = merge(a[0][i], a[k][i + 1]);
                }
                a[k][mid] = a[0][mid];
                for (int i = mid + 1; i < r; i += 1) {
                    a[k][i] = merge(a[0][i], a[k][i - 1]);
                }
            }
        }
        // debug(a);
    }
    T operator() (int l, int r) {
        if (r - l == 1) {
            return a[0][l];
        }
        int k = __lg(l ^ (r - 1));
        return merge(a[k][l], a[k][r - 1]);
    }
};