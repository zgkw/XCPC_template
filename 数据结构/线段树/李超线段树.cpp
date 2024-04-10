template<typename T, class Line, class Cmp>
struct Li_Chao_SegmentTree {
    int n;
    std::vector<int> id;
    std::vector<T> real;
    std::vector<Line> line;
    Cmp cmp;
    Li_Chao_SegmentTree() {}
    Li_Chao_SegmentTree(int _n) {
        init(_n);
    }
    Li_Chao_SegmentTree(const std::vector<T> &_init) {
        init(_init);
    }
    void init(int _n) {
        std::vector<int> _init(_n);
        iota(_init.begin(), _init.end(), 0);
        init(_init);
    }
    void init(const std::vector<T> &_init) {
        n = _init.size();
        id.assign(4 << std::__lg(n), 0);
        line.push_back(Line());
        real = _init;
        sort(real.begin(), real.end());
        real.erase(std::unique(real.begin(), real.end()), real.end());
        real.push_back(real.back() + 1);
    }
    void rangeChange (int x, int y, Line add) {
        int u = line.size();
        line.push_back(add);
        std::function<void(int, int, int, int)>
        range_Change = [&] (int l, int r, int p, int u) {
            int &v = id[p], m = (l + r) / 2;
            if (cmp(line, u, v, real[m])) {
                swap(u, v);
            }
            if (cmp(line, u, v, real[l])) {
                range_Change(l, m, p * 2, u);
            }
            if (cmp(line, u, v, real[r - 1])) {
                range_Change(m, r, p * 2 + 1, u);
            }
        };
        std::function<void(int, int, int)>
        range_find = [&] (int l, int r, int p) {
            if (real[l] >= y || real[r] <= x) {
                return;
            }
            if (x <= real[l] && real[r] <= y) {
                range_Change(l, r, p, u);
                return;
            }
            int m = (l + r) / 2;
            range_find(l, m, p * 2);
            range_find(m, r, p * 2 + 1);
        };
        range_find(0, n, 1);
    }
    void insert(Line add) {
        rangeChange(real[0], real.back(), add);
    }
    int Query(int x) {
        std::function<int(int, int, int)>
        Query = [&] (int l, int r, int p) {
            if (r - l == 1) {
                return id[p];
            }
            int m = (l + r) / 2;
            int u = id[p], v = -1;
            if (x < real[m]) {
                v = Query(l, m, p * 2);
            } else {
                v = Query(m, r, p * 2 + 1);
            }
            return cmp(line, u, v, x) ? u : v;
        };
        return Query(0, n, 1);                              
    }
    T slope_dp_Query(int x) {
        return line[Query(x)](x);
    }
};

template<typename T>
struct Line {
    T k, b;
    Line(T k = 0, T b = 0) : k(k), b(b){}
    T operator()(T x) {
        return __int128(k) * x + b;
    }
};
template<>
struct Line<double> {
    double k, b;
    Line(double k = 0, double b = 0) : k(k), b(b){}
    template<typename T>
    Line(T x0, T y0, T x1, T y1) {
        if (x0 == x1) {
            k = 0;
            b = std::max(y0, y1);
        } else {
            k = (y0 - y1) / (0. + x0 - x1);
            b = y0 - k * x0;
        }
    }
    double operator()(double x) {
        return k * x + b;
    }
};

template<typename T>
struct Cmp {
    bool operator() (vector<Line<T>> &line, int u, int v, T x) {
        return line[u](x) < line[v](x) || (line[u](x) == line[v](x) && u < v);
    }
};
template<>
struct Cmp<double> {
    bool operator() (vector<Line<double>> &line, int u, int v, double x) {
        constexpr double exp = 1e-9;
        return line[u](x) - line[v](x) > exp || (std::abs(line[u](x) - line[v](x)) <= exp && u < v);
    }
};

template<typename T, typename T1 = int>
using SegmentTree = 
    Li_Chao_SegmentTree<T1, Line<T>, Cmp<T>>;