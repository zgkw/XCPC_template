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

} PAM;

// 应用：
// 1：求s本质不同回文串个数：自动机状态数
// 2：求所有回文子串分别出现次数:插入的时候cnt[last]++，然后查询的时候倒推cnt[fail[i]]+=cnt[i]
// 3: 以第i个位置为结尾的回文串个数，cnt[i]=cnt[fail[i]]+1，边加边查cnt[last]
// 4:
