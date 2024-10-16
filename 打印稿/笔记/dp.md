## 分类

### 背包

#### 可逆背包

[可逆背包 - LiuRunky - 博客园 (cnblogs.com)](https://www.cnblogs.com/LiuRunky/p/Recovered_Knapsack.html)

经典问题：

背包容量为 $w$，n个物品，禁止选第 $i$ 件物品的可行方案数

思路：

先对n个物品做一次01背包，得到dp[i]表示总容量为i的方案共有多少种

现在禁止选择第i个物品，ans[j]表示在该限制下总容量为j的方案共有多少种，则ans[j]=dp[j]−ans[j−w[i]]（即：dp[j]包括选了i和没选i）。

### 数位dp

```c++
// 第i位，前i位，均表示从个位开始的第几位
vector<int> r(N); 表示最大值从个位开始第i位的数字
vector<int> dp(N); 表示没有限制下第i位的答案
vector<int> pw(N); 表示power(10, i)
vector<int> num(N); 表示最大值%pw[i]，即前i位数的最大值限制

// pos 表示当前所处第pos位,
// lim表示是否受最大数限制，1为true
// ok表示是否前导全为0, 1为true
auto dfs = [&](auto &&dfs, int pos, int lim, int ok) -> ll {
    if(pos == 0) return 1ll; //或者 0ll
	if(lim == 0 && ok == 0 && dp[pos]) return dp[pos]; 
    
    int up = (lim == 1 ? r[pos] : 9);
    ll ret = 0;
    for(int i = 0; i <= up; ++i) {
		if(ok == 1 && i == 0) 
            ret += dfs(dfs, pos - 1, lim && (i == r[pos]), 1);
        else 
            ret += dfs(dfs, pos - 1, lim && (i == r[pos]), 0);
    }
    if(lim == 0 && ok == 0) dp[pos] = ret;
    return ret;
};

auto ask = [&](ll x) {
    int len = 0;
    while(x) {
        len ++:
        r[len] = x%10;
        x /= 10;
	}
    
    pw[0] = 1;
    for(int i = 1; i <= len; ++i) {
		num[i] = num[i - 1] + r[i] * pw[i - 1];
         pw[i] = pw[i - 1] * 10; 
    }
    
    return dfs(dfs, len, 1, 1);
};
```



### 计数dp



### 状压dp

#### sosdp

##### n维前缀和

```c++
// O(deg * s^deg)
for(int d = 1; d <= deg; ++d) { // 枚举维度
	// 对每个维度分别对整体求前缀和
    // O(s^deg)，deg个维度，每个维度大小为s
}
```

##### 子集枚举

**原理 **

当我们用 $0/1$来表示是否取某个数时，集合 $(1,2,3, ... ,n)$ 可以用 $0 \sim 2^n - 1$ 的数来表示集合，即状压。而当我们从线性空间角度来考虑时，可以将 $n$ 个数划分为 $n$ 个维度，其中每个维度的取值集合为 $\{0,1\}$ ，此时求子集的代码如下：

```c++
// O(n * 2^n)
for(int j = 0; j < n; ++j) { // 枚举维度
	for(int i = 0; i < (1 << n); ++i) { // 枚举所有集合
        if((i >> j) & 1) { // i为当前集合，i ^ (1 << j)为子集
            // 子集转移到超集
            dp[i] <- dp[i ^ (1 << j)];
            // 或者 超集转移到子集
            dp[i] -> dp[i ^ (1 << j)];
            // 例如，i及其子集的价值和
            dp[i] += dp[i ^ (1 << j)];
		}
    }
}
```

##### 超集枚举

**原理**

理解了子集过后，将二进制中的每一个 $1$ 当作 $0$ 对待，$0$ 当作 $1$ 对待求出来的就是超集了

求超集代码如下：

```c++
for(int j = 0; j < n; ++j) {
	for(int i = 0; i < (1 << n); ++i) {
		if(!(i >> j) & 1) { // 此时i为当前集合，i ^ (1 << j)为超集
			// 转移
            // 例如，i及其超集的价值和
            dp[i] += dp[i ^ (1 << j)]
        }
    }
}
```

另一种枚举超集的代码如下：

```c++
for(int i = 1; i < (1 << n); i *= 2) {
    for(int j = 0; j < (1 << n); j += 2 * i) {
        for(int k = 0; k < i; ++k) { // 其中j + k为当前集合，i + j + k为超集
            // 转移
            // 例如，j + k及其超集的价值和
            dp[j + k] += dp[i + j + k];
        }
    } 
} 
```



### 概率与期望dp

#### 总结：

1. 运用已知条件去推，写出式子

   

2. 式子的优化：

   1. 通过设计状态优化转移：

      式子的给出，就已经意味着拥有一种转移方案，虽然一般不能直接这样转移，由于复杂度问题。此时我们可以尝试添加设计新的dp状态，来达到以下之一的目的：

      1. 原式子存在环路，通过设所有点的值为某一点值（即标定点的值）的多项式表示。转换成只需要求解每点值的多项式的各系数，最终转移到标定点上，就可以求解标定点的值，既可以得出所有点的值。

         特殊的，如果只存在自环，可以通过式子移项消去

         [Maze - HDU 4035 - Virtual Judge (vjudge.net)](https://vjudge.net/problem/HDU-4035)

      2. 另设前缀、最值等dp函数，降低转移复杂度。

      3. 如果题目中去除某些特殊要求，即为普通dp转移。可以尝试进行多个dp转移，其中不含特殊要求的普通dp可以作为中转站。

         [E-来硬的_牛客小白月赛92 (nowcoder.com)](https://ac.nowcoder.com/acm/contest/81126/E)

      4. 题目要求求解某些状态下的答案时，可以尝试对这些状态进行分解，最后利用容斥或简单求和得出答案

         

     2. 通过式子转移的顺逆优化转移：

        - 当最终状态多个，如果起点状态少量或可定时，可以尝试逆着转移，当然可能也需要同时改变式子中dp状态含义

          [P2473 [SCOI2008\] 奖励关 - 洛谷 | 计算机科学教育新生态 (luogu.com.cn)](https://www.luogu.com.cn/problem/P2473)

          

     3. 式子太过复杂可能是因为没有看清问题本质，题目所求概率与什么有关，有些因素是否可以去掉。全局概率是否可以直接求，转换为局部概率求解是否更优？

        - [P3239 [HNOI2015\] 亚瑟王 - 洛谷 | 计算机科学教育新生态 (luogu.com.cn)](https://www.luogu.com.cn/problem/P3239)

        


3. 式子的设计的基本特点：

   1. 在概率dp中

      - ${\sum}$${dp_{from}}*{p_{from - > now}} = {dp_{now}}$ 

        其中 ${\sum}p_{from -> now} = 1$

        其中 ${\sum}p_{now -> to} = 1$

     2. 在期望dp中

        - $dp_{now} = {^1_n}{\sum}( dp_{to} + w_{now->to} )$

          [E - Toward 0 (atcoder.jp)](https://atcoder.jp/contests/abc350/tasks/abc350_e)   
          
          

#### 图上概率dp经典trick总结 

- 此trick来自此讨论区，如需证明前往讨论区：

  [P3412 仓鼠找sugar II - 洛谷 | 计算机科学教育新生态 (luogu.com.cn)](https://www.luogu.com.cn/problem/solution/P3412)

  

  $x \rightarrow y$ 的期望：

  当 $x \rightarrow y$ 的路径唯一时, 设 $x \rightarrow y$ 的路径为

  ​			${ x \rightarrow u_{1} \rightarrow u_{2} \rightarrow ... \rightarrow u_{lca} \rightarrow ... \rightarrow u_{n - 1} \rightarrow u_{n} \rightarrow y}$

  定义：${d_{u}}$ 表示结点 $u$ 的度数，$fa(u)$ 表示u的父亲，${f_u}$表示 $u \rightarrow fa(u)$ 的期望步数，

  ​			其中 $u \not= root$ , $f_{root}$ 无定义，则
  $$
  f_u = d_u + \sum_{fa_{(v)} = u}{f_v}
  $$
  ​			令$g_u$ 表示 $fa_{(u)} \rightarrow u$ 的期望步数，定义 $g_{root} = 0$，则
  $$
  g_u = f_{fa_{(u)}} - f_u + g_{fa_{(u)}}
  $$
  ​			令 $F(u, v)$ 表示 $u, v$ 这条链上所有点的 $f$ 的和，不含 $v$ , 

  ​					即表示 $u$ 走到祖先 $v$ 的期望步数

  ​			令 $G(u, v)$ 表示 $u, v$ 这条链上所有点的 $g$ 的和，不含 $u$ , 

  ​					即表示 $u$ 走到子孙 $v$ 的期望步数

  此时：$x \rightarrow y$ 的期望 = $F(x, lca(x, y)) + G(lca(x, y), y)$

  

### 优化

#### 单调队列优化

#### 斜率优化

- $x$ 单调, $k$ 单调:

  - $x$ 具有单调性，可用__队列维护凸包__，同时由于 $k$ 具有单调性，于是可以删除队头，查找复杂度均摊为O(1)

- $x$ 单调, $k$ 不单调

  - $x$ 具有单调性，可用__队列维护凸包__，由于 $k$ 不具有单调性, 于是需要__队列上二分__

    

- $x$不单调, $k$ 不单调

  - $x、k$ 不具有单调性, 用__CDQ__或__李超线段树__进行决策
  



## 板子与经典结论

### 数位dp

```c++
// 第i位，前i位，均表示从个位开始的第几位
vector<int> r(N); 表示最大值从个位开始第i位的数字
vector<int> dp(N); 表示没有限制下第i位的答案
vector<int> pw(N); 表示power(10, i)
vector<int> num(N); 表示最大值%pw[i]，即前i位数的最大值限制

// pos 表示当前所处第pos位,
// lim表示是否受最大数限制，1为true
// ok表示是否前导全为0, 1为true
auto dfs = [&](auto &&dfs, int pos, int lim, int ok) -> ll {
    if(pos == 0) return 1ll; //或者 0ll
	if(lim == 0 && ok == 0 && dp[pos]) return dp[pos]; 
    
    int up = (lim == 1 ? r[pos] : 9);
    ll ret = 0;
    for(int i = 0; i <= up; ++i) {
		if(ok == 1 && i == 0) 
            ret += dfs(dfs, pos - 1, lim && (i == r[pos]), 1);
        else 
            ret += dfs(dfs, pos - 1, lim && (i == r[pos]), 0);
    }
    if(lim == 0 && ok == 0) dp[pos] = ret;
    return ret;
};

auto ask = [&](ll x) {
    int len = 0;
    while(x) {
        len ++:
        r[len] = x%10;
        x /= 10;
	}
    
    pw[0] = 1;
    for(int i = 1; i <= len; ++i) {
		num[i] = num[i - 1] + r[i] * pw[i - 1];
         pw[i] = pw[i - 1] * 10; 
    }
    
    return dfs(dfs, len, 1, 1);
};
```

### 优化

#### 单调队列优化

#### 斜率优化

##### 板子 :  x 单调, k 单调

```c++
// k层dp, 每层n位
int n, k;
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
```

##### 板子 :  x  单调, k 不单调

```c++

int n;
vector<ll> dp(n + 1, 1e18);
dp[0] = 0;

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

// 队列维护凸包
vector<int> q(n + 5);
int head = 1, tail = 0;
q[++tail] = 0;

// 二分最优策略点, 下凸包为例
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
```

##### 板子 : x 不单调, k 不单调

```c++
// CDQ板子，以下凸包为例
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
    cout << f[n];
}
```

