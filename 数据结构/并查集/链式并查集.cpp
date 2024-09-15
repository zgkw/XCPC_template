struct DSU {
    std::vector<int> prefa, nxtfa;
    DSU() {}
    DSU(int n) { init(n); }
    void init(int n) {
        prefa.resize(n);
        nxtfa.resize(n);
        std::iota(prefa.begin(), prefa.end(), 0);
        std::iota(nxtfa.begin(), nxtfa.end(), 0);
    }

    int nxt(int x) {
        int t = x;
        while (nxtfa[x] != x) {
            x = nxtfa[x];
        }
        while (t != x) {
            nxtfa[t] = x;
            t = nxtfa[t];
        }
        return x;
    }
    int pre(int x) {
        int t = x;
        while (prefa[x] != x) {
            x = prefa[x];
        }
        while (t != x) {
            prefa[t] = x;
            t = prefa[t];
        }
        return x;
    }
    void remove(int x) {
        nxtfa[x] = nxt(x + 1);
        prefa[x] = pre(x - 1);
    }
};
