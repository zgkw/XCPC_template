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
