#include<bits/stdc++.h>
using namespace std;
#define _for(i,a,b) for(int (i)=(a); i<(b); ++i)
#define _rep(i,a,b) for(int (i)=(a); i<=(b); ++i)
#define pii pair<int,int>
#define pll pair<ll,ll>
using ll = long long;
const ll maxn = 1e7+5;
const ll maxe = 1e7+5;
const ll N = 1e7+5;
const ll inf = 0x3f3f3f3f;

/*
ŷ����������Ԫ
f(n) = (TT f(pi^ki) ) 
	 = n*(TT (1-1/pi) )
	 = n*(TT (pi-1)/pi )
*/

ll a,mod;

int eular_phi(int n)// ŷ������ 
{
	int ans = n;
	for(int i=2; i*i <= n; ++i)
	{
		if(n%i == 0) {//������ 
			ans = ans/i*(i-1);
			while(n%i == 0) n/=i;//������ȥ�� 
		}
	}
	if(n > 1) ans = ans/n*(n-1);//���� n Ϊ���� 
	return ans;
}

ll quick_mi(ll a,ll n)
{
	ll res = 1;
	while(n)
	{
		if(n&1) res = (res*a)%mod;
		a = (a*a)%mod;
		n>>=1;
	}
	return res;
}

void solve()
{
	//����
	scanf("%d %d",&a,&mod);
	ll f = eular_phi(mod);
	cout<<f<<endl;
	for(int i=1; i<=a; ++i)
	{
		printf("%d\n",quick_mi(i,f-1));
	}
}

// ָ��ȡģ����:
/*

�� x^(2^n)��modȡģ 
 x <= 1e18 , n <= 1e18
 ��֤ x �� mod ����
 f = eular_phi(mod)
 �� x^f = 1 
 �� 2^n��f ȡģ������Ӱ���� 
 
 ###������Ҳ�����㣬mod��x�ı���,��ô�� x^(2^n)>=mod? 0:x^(2^n) 
*/  

ll quick(ll a,ll n, ll mod)
{
	ll res = 1;
	while(n)
	{
		if(n&1) res = res*a%mod;
		a = a*a%mod;
		n>>=1;
	}
	return res;
}

void phi()
{
	ll x,n,mod;
	cin>>x>>n>>mod;
	
	ll f = eular_phi(mod);
	ll mi = quick(2,n,f);
	ll ans = quick(x,mi,mod);
	cout<<ans;
}

int main()
{
	//ios::sync_with_stdio(false);
	//cin.tie(0);cout.tie(0);
	
	int T = 1;
	while(T--) solve();
	
	return 0;
}

