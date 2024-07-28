struct KMP{
    int n;
    std::vector<int> pi;
    std::vector<vector<int>> aut;

    KMP(const std::string &s) {
    	n = (int)s.length();
        prefix_function(s);
        compute_automaton(s);
    }

    void prefix_function(string s) {
        pi.resize(n);
        for (int i = 1; i < n; i++) {
            int j = pi[i - 1];
            while (j > 0 && s[i] != s[j]) j = pi[j - 1];
            if (s[i] == s[j]) j++;
            pi[i] = j;
        }
    }

    void compute_automaton(string s) {
        aut.resize(n, vector<int>(26));
        for (int i = 0; i < n; i++) {
            for (int c = 0; c < 26; c++) {
            if (i > 0 && 'a' + c != s[i])
                aut[i][c] = aut[pi[i - 1]][c];
            else
                aut[i][c] = i + ('a' + c == s[i]);
            }
        }
    }
};

