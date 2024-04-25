struct CutPoint {
    int n, tot = -1, root = -1;
    vector<vector<int>> map;
    vector<int> d, id;
    vector<bool> iscutpoint;

    CutPoint(int n): n(n), map(n), d(n, -1), id(n, -1), iscutpoint(n, 0) {};

private:
    void _cutpoint(int now) {
        d[now] = id[now] = ++tot;
        int child = 0;
        for (auto u: map[now]) {
            if (!~d[u]) {
                _cutpoint(u);
                id[now] = min(id[now], id[u]);
                if (id[u] >= d[now]) {
                    ++child;
                    if (now != root || child >= 2) {
                        iscutpoint[now] = 1;
                    }
                }
            } else id[now] = min(d[u], id[now]);
        }
    }

public:
    void cutpoint(int now, int root) {
        this->root = root;
        _cutpoint(now);
        this->root = -1;
    }

};


struct CutPoint {
    int n, tot = -1, root = -1;
    vector<vector<int>> map;
    vector<int> d, id, stack;
    vector<bool> iscutpoint, instack;
    vector<vector<int>> ans;

    CutPoint(int n) : n(n), map(n), d(n, -1), id(n, -1), iscutpoint(n, 0), instack(n, 0) {};
    
private:
    void _cutpoint(int now) {
        d[now] = id[now] = ++tot;
        stack.push_back(now);
        instack[now] = 1;
        int child = 0;
        for (auto u: map[now]) {
            if (!~d[u]) {
                _cutpoint(u);
                id[now] = min(id[now], id[u]);
                if (id[u] >= d[now]) {
                    ++child;
                    if (now != root || child >= 2) {
                        iscutpoint[now] = 1;
                        ans.push_back(vector<int>(0));
                        auto &bk = ans.back();
                        while (instack[u]) {
                            bk.push_back(stack.back());
                            instack[stack.back()] = 0;
                            stack.pop_back();
                        }
                        bk.push_back(now);
                    }
                }
            } else id[now] = min(d[u], id[now]);
            if (now == root && child) {
                ans.push_back(vector<int>(0));
                auto &bk = ans.back();
                while (instack[now]) {
                    bk.push_back(stack.back());
                    instack[stack.back()] = 0;
                    stack.pop_back();
                }
            }
        }
    }

public:
    void cutpoint(int now, int root) {
        this->root = root;
        _cutpoint(now);
        this->root = -1;
    }
};