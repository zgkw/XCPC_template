using ll = long long;

void solve() {
    ll n, k;
    cin >> n >> k;
    // k层dp, 每层n位
    vector<ll> f(n + 1), g(n + 1); // 滚动数组

    // 斜率优化, 点(X, Y), 斜率K, 
    auto X = [&](int i) { return 1; }; // 
    auto Y = [&](int i) { return 1; }; // 
    auto K = [&](int i) { return 1; }; // 

    // 计算斜率
    auto slope = [&](int i, int j) -> long double{
        if(X(j) == X(i)) return (Y(j) >= Y(i) ? 1e18 : -1e18);
        else {
            return (long double)(Y(j) - Y(i)) / (X(j) - X(i));
        }
    };
    
    // 队列存凸包
    vector<int> q(n + 3);

    for(int i = 1; i <= n; ++i) {
        // g[i] = ... ;
        // 初始化k = 1，一般可以直接计算
    }

    // 下凸包为例
    for(int c = 2; c <= k; ++c) {
        int head = 1, tail = 0;
        q[++tail] = 0;
        for(int i = 1; i <= n; ++i) {
            while(head < tail && slope(q[head], q[head + 1]) <= K(i)) ++head;
            ll B = Y(q[head]) - K(i) * X(q[head]);
            // f[i] = B + ...; f[i] 与 B 之间的式子
            while(head < tail && i != n && slope(q[tail - 1], q[tail]) >= slope(q[tail], i)) --tail;
            q[++tail] = i;
        }
        std::swap(f, g);
    }
    cout << g[n];
}
