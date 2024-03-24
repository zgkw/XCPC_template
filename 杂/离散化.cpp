template<class T>
struct Discretization {
    std::map <T, int> map;
    std::vector<T> Val;
    Discretization () {};
    Discretization (const std::vector<T> &able) {
        init(able);
    }
    void init(const std::vector<T> &able) {
        for (auto u : able) {
            map[u] = 0;
        }
        int tot = 0;
        for (auto &[u, v] : map) {
            v = tot ++;
            Val.push_back(u);
        }
    }
    int id (T x) {
        return map[x];
    }
    int val(int x) {
        return Val[x];
    }
    int size() {
        return Val.size();
    }
};