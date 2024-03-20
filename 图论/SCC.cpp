struct SCC {
	using i64 = long long ;
	i64 n , cnt = 0 , tot = -1 ;
	vector < vector < i64 > > mp ;
	vector < i64 > d , id , stack , tag ;
	vector < bool > instack ;
	
	explicit SCC ( ll n , vector < vector < i64 > >& mp ) : 
		n ( n ) , mp ( mp ) , d ( vector < i64 > ( n , -1 ) ) , id ( vector < i64 > ( n ) ) ,
			tag ( vector < i64 > ( n , -1 ) ) , instack ( vector < bool > ( n , 0 ) ) {}
private:
	void __scc ( ll now ) {
//		ps ;
		d [ now ] = id [ now ] = ++ tot ;
		stack.push_back ( now ) ; instack [ now ] = 1 ;
		for ( auto u : mp [ now ] ) {
			if ( !~d [ u ] ) {
				__scc ( u ) ;
				id [ now ] = min ( id [ now ] , id [ u ] ) ;
			}
			else if ( instack [ u ] ) {
				id [ now ] = min ( id [ now ] , id [ u ] ) ;
			}
		}
		if ( d [ now ] == id [ now ] ) {
			++ cnt ;
			do {
				instack [ stack.back () ] = 0 ;
				tag [ stack.back () ] = cnt ;
				stack.pop_back () ;
			} while ( instack [ now ] ) ;
		}
	}
public:
	void scc ( ll now ) {
		-- cnt ;
		__scc ( now ) ;
		++ cnt ;
	}
};
