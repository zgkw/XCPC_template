using ll = long long;
const int maxn = 1e5 + 5;

struct node {
    int id;
    ll x, y, k;
};
// a表示原数组，b为归并辅助数组
vector<node> a(maxn), b(maxn);

ll X(int i) { return a[i].x; }
ll Y(int i) { return a[i].y; }
ll K(int i) { return a[i].k; }
long double slope(int i, int j) {
    if(X(j) == X(i)) return (Y(j) >= Y(i) ? 1e20 : -1e20);
    else {
        return (long double)(Y(j) - Y(i)) / (X(j) - X(i));
    }
};

// dp数组, 切记f[]的初始化
vector<ll> f(maxn, 1e18);

// 按照x进行归并
void merge(int L, int mid, int R)
{
    int p1 = L, p2 = mid + 1;
    int tp = L;
    while(p1 <= mid && p2 <= R) {
        if(a[p1].x <= a[p2].x) b[tp++] = a[p1++];
        else b[tp++] = a[p2++];
    }
    while(p1 <= mid) b[tp++] = a[p1++];
    while(p2 <= R) b[tp++] = a[p2++];
    for(int i = L; i <= R; ++i) a[i] = b[i];
}

void cdq(int L, int R) {
    if(L == R) {
        int pos = a[L].id;
        // f[pos] = ...; //视情况而修改，有些求解为前缀最优，则在此处修改。
        // 例f[pos] = max(f[pos], f[pos - 1]);
        // a[L].x = ;
        // a[L].y = ;
        return ;
    }

    int mid = (L + R) >> 1;
    // 分为左右两边
    int p1 = L, p2 = mid + 1;
    for(int i = L; i <= R; ++i) {
        if(a[i].id <= mid) b[p1++] = a[i];
        else b[p2++] = a[i];
    }
    for(int i = L; i <= R; ++i) a[i] = b[i];

    cdq(L, mid);

    // 下凸包, 上凸包则需要改成 slope() <= slope()
    vector<int> q(R - L + 3);
    int head = 1, tail = 0;
    for(int i = L; i <= mid; ++i) {
        while(head < tail && slope(q[tail - 1], q[tail]) >= slope(q[tail], i)) --tail;
        q[++tail] = i;
    }

    // 下凸包, 上凸包则需要改成 slope() >= K()，同时f[pos] = max(f[pos], B ...)
    for(int i = mid + 1; i <= R; ++i) {
        while(head < tail && slope(q[head], q[head + 1]) <= K(i)) ++head;
        ll B = Y(q[head]) - K(i) * X(q[head]);
        int pos = a[i].id;
        // f[pos] = min(f[pos], B ...);
    }

    cdq(mid + 1, R);

    merge(L, mid, R);
}

void solve() {
    int n;
    cin >> n;
    for(int i = 1; i <= n; ++i) {
        // a[i].id = i;
        // a[i].k = 2 * h[i];
    }

    // 下凸包, 上凸包修改为 x.k > y.k;
    sort(a.begin() + 1, a.begin() + n + 1, [&](node &x, node &y){
        return x.k < y.k;
    });

    f[1] = 0; // 视情况而初始化
    cdq(1, n);
    cout << f[n]; // 一般答案求解为f[n] 
}
