# include  <bits/stdc++.h>
using  namespace  std ;

using ll = long long ;
namespace predefine {
const long long inf = 0x3f3f3f3f3f3f3f3f ;
using ll = long long ;
using Int = __int128 ;
using pll = pair < ll , ll > ;
using pls = pair < ll , string > ;
using psl = pair < string , ll > ;
using pss = pair < string , string > ;
# define  IOS  iostream::sync_with_stdio ( 0 ) ; cin.tie ( 0 ) ;

# define af( i , a , b ) for ( ll i = a ; i <= b ; ++ i )
# define df( i , a , b ) for ( ll i = a ; i >= b ; -- i )
# define Dg( x ) cout << " ------- " <<  x << " ------- " << '\n'
# define Dgop Dg ( "Debug op" )
# define Dged Dg ( "Debug ed" )
# define pr( x ) cout << #x << " = " << x << '\n'
# define pn putchar ('\n') 
# define ps cout << "yes" 
# define lowbit( x ) ( x & ( - x ) ) 
# define Rd(...) \
	ll __VA_ARGS__ ; \
	rd ( __VA_ARGS__ )

// # define endl '\n'
}
//-----  快读、快输  ---------------------------------------------------------------------------------------------------------
namespace Fastio {
using ll = long long ;
# define MAXSIZE (1 << 20)
# define isdigit(x) (x >= '0' && x <= '9')

char buf[MAXSIZE], *p1 = buf, *p2 = buf;
char _0 = ' ' , _1 = '\n' , _2 = ',' ;

inline char gc() {
	return getchar();
	if (p1 == p2) p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin);
	return p1 == p2 ? ' ' : *p1++;
}

inline bool blank(char ch) {
	return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

template <class T>
T read( T & x ) {
	x = 0 ;
	bool sign = 0;
	char ch = gc();
	for (; !isdigit(ch); ch = gc())
		if (ch == '-') sign = 1;
	for (; isdigit(ch); ch = gc()) x = ( x << 3 ) + ( x << 1 ) + ch - '0';
	if (sign) x = -x;
	return x ;
}

template <>
double read<double>( double & x) {
	x = 0 ;
	double tmp = 1;
	bool sign = 0;
	char ch = gc();
	for (; !isdigit(ch); ch = gc())
		if (ch == '-') sign = 1;
	for (; isdigit(ch); ch = gc()) x = x * 10 + ch - '0';
	if (ch == '.')
	for (ch = gc(); isdigit(ch); ch = gc())
		tmp /= 10.0, x += tmp * (ch - '0');
	if (sign) x = -x;
	return x ;
}

template<>
string read<string>( string & s ) {
	s.clear() ;
	char ch = gc();
	for (; blank(ch); ch = gc());
	for (; !blank(ch); ch = gc()) s += ch;
	return s ;
}

template<>
char read<char>(char & c) { c = ' ' ; for (c = gc(); blank(c); c = gc()); return c ; }

void rd () {}
template<typename Type, typename... Types>
void rd(Type& arg, Types&... args) {
    read<Type>( arg );
    rd(args...);
}

template <typename T>
void sp( T x ) {
    T _ = x < 0;
    if (_) putchar('-'), x *= -1;
    if (x > 9) sp(x / 10);
    putchar(x % 10 | '0');
}

void pf() {}
template <typename Type, typename... Types>
void pf(Type arg, int x, Types... args) {
    sp < Type > ( arg );
    x == 0 ? putchar(' ') : putchar('\n');
    pf(args...);
}
}
//-----  快读、快输  ---------------------------------------------------------------------------------------------------------

using namespace predefine ;
using namespace Fastio ;

//-----  板子  ---------------------------------------------------------------------------------------------------------

//-----  板子  ---------------------------------------------------------------------------------------------------------



//-----  常量定义  ---------------------------------------------------------------------------------------------------------

const ll maxn = 3e5 + 10 ;

const ll mod = 998244353 ;

ll Q_power ( ll a , ll b , ll mod ) { 
	a = a % mod ; ll ans = 1 ; 
	for ( ; b ; b >>= 1 ) { if ( b & 1 ) ans = ( a * ans ) % mod ; a = ( a * a ) % mod ; } 
	return ans ; }

inline ll sum1 ( ll _a ) { return ( _a * ( _a + 1 ) ) >> 1 ; }
inline ll sum2 ( ll _b ) { return _b * ( _b + 1 ) * ( 2 * _b + 1 ) / 6  ; }

ll gcd( ll a , ll b ){ 
	if ( a < b ) swap ( a , b ) ; 
	while ( a % b != 0 ){ ll t_a = a ; a = b ; b = t_a % b ; } 
	return b ; }

ll hr [ maxn ] , hr1 [ maxn ]  /*,  hr2 [ maxn ] *//* , hr3 [ maxn ] *//* , hr4 [ maxn ] */;

bitset < maxn > vis ;

// ll sum [ maxn ] ;

// priority_queue < ll > q ;
//  priority_queue < pll , vector < pll > , greater < pll > > q1 ;

// unordered_map < char , ll > hp;

// ll dx [ 4 ] = { -1 , 0 , 1 , 0 } ;
// ll dy [ 4 ] = { 0 , 1 , 0 , -1 } ;

// char mp [ maxn ] [ maxn ] ;

// ll dp1 [ maxn ] , dp2 [ maxn ] , dp3 [ maxn ] ;

ll line_cnt = 0 ; 
struct Line {
	ll l , r , y , val ;
	bool operator< ( Line& a ) const {
		return y < a.y ;
	}
} line [ 2 * maxn ] ;

ll X [ 2 * maxn ] ;

struct Tree {
	ll l , r ;
	ll cnt , len ;
} tree [ maxn << 3 ];

void build ( ll tag , ll l , ll r ) {
	tree [ tag ] = { l , r , 0 , 0 } ;
	if ( l == r )  return ;
	ll mid = ( l + r ) >> 1 ;
	build ( tag << 1 , l , mid ) ;
	build ( tag << 1 | 1 , mid + 1 , r ) ;
}

void pushup ( int u ) {
	ll l = X [ tree [ u ].l ] ;
	ll r = X [ tree [ u ].r + 1 ] ;
	if ( tree [ u ].cnt ) tree [ u ].len = r - l ;
	else tree [ u ].len = tree [ u << 1 ].len + tree [ u << 1 | 1 ].len ;
}

void modify ( ll u , ll l , ll r , ll tag ) {
	if ( l > tree [ u ].r || r < tree [ u ].l ) return ;
	if ( l <= tree [ u ].l && tree [ u ].r <= r ) {
		tree [ u ].cnt += tag ;
		pushup ( u ) ;
		return ;
	}
	modify ( u << 1 , l , r , tag ) ; modify ( u << 1 | 1 , l , r , tag ) ;
	pushup ( u ) ;
}

void solve () {
	Rd ( n ) ;
	af ( i , 1 , n ) {
		Rd ( x1 , y1 , x2 , y2 ) ;
		line [ ++ line_cnt ] = { x1 , x2 , y1 , 1 } ;
		X [ line_cnt ] = x1 ;
		line [ ++ line_cnt ] = { x1 , x2 , y2 , -1 } ;
		X [ line_cnt ] = x2 ;
	}
	n *= 2 ;
	sort ( line + 1 , line + n + 1 ) ;
	sort ( X + 1 , X + n + 1 ) ;
	ll s = unique ( X + 1 , X + n + 1 ) - X - 1 ;
	build ( 1 , 1 , s - 1 ) ;
	ll ans = 0 ;
	af ( i , 1 , line_cnt  ) {
		ll l = lower_bound ( X + 1 , X + s + 1 , line [ i ].l ) - X ;
		ll r = lower_bound ( X + 1 , X + s + 1 , line [ i ].r ) - X ;
		modify ( 1 , l , r - 1 , line [ i ].val ) ;
		ans += tree [ 1 ].len * ( line [ i + 1 ].y - line [ i ].y ) ;
	}
	pf ( ans , 1 ) ;
}

int  main () {
	// IOS

	ll _ ;

	   _ = 1 ;

	// rd ( _ ) ;

	// cin >> _ ;

	while ( _ -- )
	{
		solve () ;
	}

	return 0 ;
}

/*
//-----  数据  ---------------------------------------------------------------------------------------------------------
3260745630
266012571
2
0 0 100 1
0 0 1 100
----------
*/
