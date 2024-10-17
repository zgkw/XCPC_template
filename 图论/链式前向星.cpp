template<class E>
struct star {
    vector<int> s, t;
    star() {}
    star(int n, const vector<pair<int, E>> &e): s(n, -1) {
        // debug(e);
        t.reserve(e.size());
        for (int i = 0; i < e.size(); i += 1) {
            int u = e[i].first;
            t.push_back(s[u]), s[u] = i;
        }
    }
    void addEdge(int u, int v) { // 边放在外面
        t.push_back(s[u]), s[u] = t.size() - 1;
        t.push_back(s[v]), s[v] = t.size() - 1;
    }
};