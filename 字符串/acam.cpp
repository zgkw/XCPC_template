struct AhoCorasick {
    static constexpr int ALPHABET = 26;
    struct Node {
        int len;
        int link;
        std::array<int, ALPHABET> next;
        Node() : len{0}, link{0}, next{} {}
    };

    std::vector<Node> t;

    AhoCorasick() { init(); }

    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    int add(const std::string& a) {
        int p = 1;
        for (auto c : a) {
            int x = c - 'a';
            if (t[p].next[x] == 0) {
                t[p].next[x] = newNode();
                t[t[p].next[x]].len = t[p].len + 1;
            }
            p = t[p].next[x];
        }
        return p;
    }

    void work() {
        std::queue<int> q;
        q.push(1);

        while (!q.empty()) {
            int x = q.front();
            q.pop();

            for (int i = 0; i < ALPHABET; i++) {
                if (t[x].next[i] == 0) {
                    t[x].next[i] = t[t[x].link].next[i];
                } else {
                    t[t[x].next[i]].link = t[t[x].link].next[i];
                    q.push(t[x].next[i]);
                }
            }
        }
    }

    int next(int p, int x) { return t[p].next[x]; }

    int link(int p) { return t[p].link; }

    int len(int p) { return t[p].len; }

    int size() { return t.size(); }
};

// fail树重要性质:只要将fail 树上每个属于文本串的结点权值置为 1，那么节点 x
// 的子树总权值就是 x 代表的串在文本串中出现的次数。
// 常用结论：一个单词在匹配串S 中出现次数之和，等于它在S
// 的所有前缀中作为后缀出现 的次数之和。

//经典问题：
//查某个文本串s在若干模式串里的出现次数：
//将fail 树上每个属于文本串的结点权值置为 1(注意要处理所有前缀，即：不断跳next)，那么节点 x
//的子树总权值就是 x 代表的串在文本串中出现的次数

//查若干模式串在文本串里的出现次数（出现信息）：
//先在endpos处标记，然后向下传递标记，
//文本串出现次数的答案为跳next过程碰到的所有点的权值和


//常用部分如下
int n;
cin >> n;
AhoCorasick ac;
vector<int> end(n + 1, 0);
for (int i = 1; i <= n; i++) {
    string s;
    cin >> s;
    end[i] = ac.add(s);
}

ac.work();

vector<int> cnt(ac.size());
for (int i = 1; i <= n; i++) {
    cnt[end[i]]++;
}
vector<vector<int>> adj(ac.size());
for (int i = 2; i < ac.size(); i++) {
    adj[ac.link(i)].push_back(i);
}
auto dfs = [&](auto self, int u) -> void {
    for (auto v : adj[u]) {
        cnt[v] += cnt[u];
        self(self, v);
    }
};
dfs(dfs, 1);
