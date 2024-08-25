//jiangly: 
struct PAM {
    static constexpr int ALPHABET_SIZE = 28;
    struct Node {
        int len;
        int link;
        int cnt;
        std::array<int, ALPHABET_SIZE> next;
        Node() : len{}, link{}, cnt{}, next{} {}
    };
    std::vector<Node> t;
    int suff;
    std::string s;
    PAM() { init(); }
    void init() {
        t.assign(2, Node());
        t[0].len = -1;
        suff = 1;
        s.clear();
    }
    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    bool add(char c, char offset = 'a') {
        int pos = s.size();
        s += c;
        int let = c - offset;
        int cur = suff, curlen = 0;

        while (true) {
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos])
                break;
            cur = t[cur].link;
        }
        if (t[cur].next[let]) {
            suff = t[cur].next[let];
            return false;
        }

        int num = newNode();
        suff = num;
        t[num].len = t[cur].len + 2;
        t[cur].next[let] = num;

        if (t[num].len == 1) {
            t[num].link = 1;
            t[num].cnt = 1;
            return true;
        }

        while (true) {
            cur = t[cur].link;
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                t[num].link = t[cur].next[let];
                break;
            }
        }

        t[num].cnt = 1 + t[t[num].link].cnt;

        return true;
    }
};

PAM pam;
// 应用：
// 1：求s本质不同回文串个数：自动机状态数
// 2：求所有回文子串分别出现次数:插入的时候cnt[last]++，然后查询的时候倒推cnt[fail[i]]+=cnt[i]
// 3: 以第i个位置为结尾的回文串个数，cnt[i]=cnt[fail[i]]+1，边加边查cnt[last]
struct Palindrome_Automaton {
    int ch[N][Sigma], fail[N], len[N], cnt[N], tot, last;
    Palindrome_Automaton() {
        tot = 1;
        fail[0] = 1, fail[1] = 1, len[1] = -1;
    }
    int getfail(int x, int i) {
        while (i - len[x] - 1 < 0 || s[i - len[x] - 1] != s[i])
            x = fail[x];
        return x;
    }
    void insert(char c, int i) {
        int x = getfail(last, i), w = c - 'a';
        if (!ch[x][w]) {
            len[++tot] = len[x] + 2;
            int tmp = getfail(fail[x], i);
            fail[tot] = ch[tmp][w];
            cnt[tot] = cnt[fail[tot]] + 1;
            ch[x][w] = tot;
        }
        last = ch[x][w];
    }

} PAM1;
