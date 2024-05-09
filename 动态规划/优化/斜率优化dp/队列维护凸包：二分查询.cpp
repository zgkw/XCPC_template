using ll = long long;

void solve() {
    ll n, s;
    cin >> n >> s;

    vector<ll> dp(n + 1, 1e18);
    dp[0] = 0;

    // 斜率优化, 点(X, Y), 斜率K, 
    auto X = [&](int i) { return 1; }; // 
    auto Y = [&](int i) { return 1; }; // 
    auto K = [&](int i) { return 1; }; // 

    // 计算斜率
    auto slope = [&](int i, int j) -> long double{
        if(X(j) == X(i)) return (Y(j) >= Y(i) ? 1e20 : -1e20);
        else {
            return (long double)(Y(j) - Y(i)) / (X(j) - X(i));
        }
    };

    // 队列存凸包
    vector<int> q(n + 5);
    int head = 1, tail = 0;
    q[++tail] = 0;

    // 二分最优策略点
    auto ask = [&](ll k) {
        int l = head, r = tail;
        while(l < r) {
            int mid = (l + r) >> 1;
            if(slope(q[mid], q[mid + 1]) >= k) r = mid;
            else l = mid + 1;
        }
        return q[l];
    };

    // 下凸包为例
    for(int i = 1; i <= n; ++i) {
        int j = ask(K(i));
        ll B = Y(j) - K(i) * X(j);
        // dp[i] = B + ... ;
        while(head < tail && i != n && slope(q[tail - 1], q[tail]) >= slope(q[tail], i)) --tail;
        q[++tail] = i;
    }

    cout << dp[n];
}
