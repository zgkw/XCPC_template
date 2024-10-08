struct SAM {
    static constexpr int ALPHABET_SIZE = 26;
    struct Node {
        int len;
        int link;
        std::array<int, ALPHABET_SIZE> next;
        Node() : len{}, link{}, next{} {}
    };
    std::vector<Node> t;
    SAM() { init(); }
    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }
    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }
    int extend(int p, int c) {
        if (t[p].next[c]) {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                return q;
            }
            int r = newNode();
            t[r].len = t[p].len + 1;
            t[r].link = t[q].link;
            t[r].next = t[q].next;
            t[q].link = r;
            while (t[p].next[c] == q) {
                t[p].next[c] = r;
                p = t[p].link;
            }
            return r;
        }
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        while (!t[p].next[c]) {
            t[p].next[c] = cur;
            p = t[p].link;
        }
        t[cur].link = extend(p, c);
        return cur;
    }
    // int extend(int p, char c, char offset = 'a') {
    //     return extend(p, c - offset);
    // }

    int next(int p, int x) { return t[p].next[x]; }

    // int next(int p, char c, char offset = 'a') { return next(p, c - 'a'); }

    int link(int p) { return t[p].link; }

    int len(int p) { return t[p].len; }

    int size() { return t.size(); }

    string lcs(const string& s, char offset = 'a') {
        int p = 1, l = 0;
        int pos = 0, len = 0;
        int cnt = 0;
        for (auto i : s) {
            while (p != 1 && (next(p, i - offset) == 0)) {
                p = link(p);
                l = t[p].len;
            }
            if (next(p, i - offset)) {
                p = next(p, i - offset);
                l++;
            }
            if (l > len) {
                len = l;
                pos = cnt;
            }
            cnt++;
        }
        return s.substr(pos - len + 1, len);
    };
};

// 应用：
// 1: 检查字符串是否出现
// 给一个文本串 T 和多个模式串 P ，我们要检查字符串 P 是否作为 T
// 的一个子串出现。 我们在
// O(T)
// 的时间内对文本串 T 构造后缀自动机。为了检查模式串 P  是否在 T
// 中出现，我们沿转移（边）从 t0
// 开始根据 P 的字符进行转移。如果在某个点无法转移下去，则模式串 P 不是 T
// 的一个子串。如果我们能够这样处理完整个字符串 P  ，那么模式串在 T  中出现过。
// 对于每个字符串 P  ，算法的时间复杂度为 O(P)
// 此外，这个算法还找到了模式串 P  在文本串中出现的最大前缀长度。

// 2: 出现次数
// 对于一个给定的文本串T ，有多组询问，每组询问给一个模式串P,
// 回答模式串 P 在字符串 T 中作为子串出现了多少次。
// 对文本串 T 构造后缀自动机。
// 接下来做预处理：对于自动机中的每个状态v ，预处理cnt_v
// 使之等于endpos(v) 集合的大小。事实上，对应同一状态 v 的所有子串在文本串 T
// 中的出现次数相同，这相当于集合 endpos 中的位置数。
// 然而我们不能明确的构造集合 endpos ，因此我们只考虑它们的大小cnt
// 为了计算这些值，我们进行以下操作。对于每个状态，
// 如果它不是通过复制创建的（且它不是初始状态t0)，
// 我们将它的 cnt 初始化为 1。然后我们按它们的长度len降序遍历所有状态，
// 并将当前的 cnt_v 的值加到后缀链接指向的状态上，即：
//  cnt_link(v) += cnt_v
// 最后回答询问只需要查找值cnt_t ,其中 t 为模式串对应的状态，
// 如果该模式串不存在答案就为 0。单次查询的时间复杂度为O(P),预处理复杂度O(|T|)

// 3: LCS
// 对S构造后缀自动机,处理T串

