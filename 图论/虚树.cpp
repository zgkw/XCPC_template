constexpr int maxTop = 3e5;
int stk[maxTop];
struct Tree {
    int n, cur;
    vector<vector<int>> adj;
    vector<int> dfn, dep, siz, fa, seq, dnp;
    SparseTable<int> rmq;
    Tree() = default;
    Tree(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        adj.assign(n, {});
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<vector<int>> vTree;
    void work(int root = 0) {
        dfn.assign(n, 0);
        dep.assign(n, 0);
        siz.assign(n, 1);
        fa.assign(n, 0);
        seq.assign(n, 0);
        dnp.assign(n, 0);
        vTree.assign(n, {});
        cur = 0;
        dfs(root, root);
        rmq.init(dnp, [] (int a, int b) {
            return std::min(a, b);
        });
    }
    void dfs(int now, int f) {
        dnp[cur] = dfn[f];
        seq[dfn[now] = cur ++] = now;
        dep[now] = dep[f] + 1;
        fa[now] = f;
        for (auto here : adj[now]) {
            if (here == f) continue;
            dfs(here, now);
            siz[now] += siz[here];
        }
    }
    int lca (int lhs, int rhs) {
        if (lhs == rhs) return lhs;
        if ((lhs = dfn[lhs]) > (rhs = dfn[rhs])) swap(lhs, rhs); 
        return seq[rmq(lhs + 1, rhs + 1)];
    }
    vector<vector<int>> &build_virtual_tree(vector<int> &key) {
        sort(key.begin(), key.end(), [&] (int x, int y) {return dfn[x] < dfn[y];});
        int Top = 0;
        vTree[0].clear();
        for (int i = key[0] == 0; i < key.size(); i += 1) {
            int Lca = lca(key[i], stk[Top]);
            if (Lca != stk[Top]) {
                while(dfn[Lca] < dfn[stk[Top - 1]]) {
                    vTree[stk[Top - 1]].push_back(stk[Top --]);
                }
                if (Lca != stk[Top - 1]) {
                    vTree[Lca].clear();
                    vTree[Lca].push_back(stk[Top]); stk[Top] = Lca;
                } else {
                    vTree[Lca].push_back(stk[Top --]);
                }
            }
            vTree[key[i]].clear();
            stk[++ Top] = key[i];
        }
        for (int i = 0; i < Top; i += 1) {
            vTree[stk[i]].push_back(stk[i + 1]);
        }
        return vTree;
    }
};