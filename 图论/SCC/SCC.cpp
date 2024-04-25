struct SCC {
    int n, cnt = 0, tot = -1;
    vector<vector<int>> map;
    vector<int> d, id, stack, tag;
    vector<bool> instack;

    SCC(int n): n(n), map(n), d(n, -1), id(n), tag(n, -1), instack(n, 0) {}

private:
    void _scc(int now) {
        d[now] = id[now] = ++tot;
        stack.push_back(now);
        instack[now] = 1;
        for (auto u : map[now]) {
            if (!~d[u]) {
                _scc(u);
                id[now] = min(id[now], id[u]);
            } else if (instack[u]) {
                id[now] = min(id[now], id[u]);
            }
        }
        if (d[now] == id[now]) {
            ++cnt;
            do {
                instack[stack.back()] = 0;
                tag[stack.back()] = cnt;
                stack.pop_back();
            } while (instack[now]);
        }
    }

public:
    void addedge(int u, int v) {
        map[u].push_back(v);
    }
    void scc(int now) {
        --cnt;
        _scc(now);
        ++cnt;
    }
};