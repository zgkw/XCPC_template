template <class E> 
struct csr {
    vector<int> s;
    vector<E> adj;
    csr(int n, const vector<pair<int, E>>& g)
        : s(n + 1), adj(g.size()) {
        for (auto e : g) {
            s[e.first + 1]++;
        }
        for (int i = 1; i <= n; i++) {
            s[i] += s[i - 1];
        }
        auto c = s;
        for (auto e : g) {
            adj[c[e.first]++] = e.second;
        }
    }
};