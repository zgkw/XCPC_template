struct CPT {

	ll n , tot = -1 , root = -1 ;
	vector < vector < ll > > mp ;
	vector < ll > d , id ;
	vector < bool > iscp ;

	explicit CPT ( ll n , vector < vector < ll > >& mp ) :
		 n ( n ) , mp ( mp ) , d ( vector < ll > ( n , -1 ) ) ,
		  id ( vector < ll > ( n , -1 ) ) , iscp ( vector < bool > ( n , 0 ) ) {} ;
	
private:
	void __cutpoint ( ll now ) {
		d [ now ] = id [ now ] = ++ tot ;
		ll child = 0 ;
		for ( auto u : mp [ now ] ) {
			if ( !~d [ u ] ) {
				__cutpoint ( u ) ;
				id [ now ] = min ( id [ now ] , id [ u ] ) ;
				if ( id [ u ] >= d [ now ] ) {
					++ child ;
					if ( now != root || child >= 2 ) {
						iscp [ now ] = 1 ;
					}
				}
			}
			else id [ now ] = min ( d [ u ] , id [ now ] ) ;
		}
	}

public:
	void cutpoint ( ll now , ll root ) {
		this ->root = root ;
		__cutpoint ( now ) ;
		this ->root = -1 ;
	}

};

/*µãË«°æ 
struct CPT {

	ll n , tot = -1 , root = -1 ;
	vector < vector < ll > > mp ;
	vector < ll > d , id , stack ;
	vector < bool > iscp , instack ;
	vector < vector < ll > > ans ;

	explicit CPT ( ll n , vector < vector < ll > >& mp ) :
		 n ( n ) , mp ( mp ) , d ( vector < ll > ( n , -1 ) ) ,
		  id ( vector < ll > ( n , -1 ) ) , iscp ( vector < bool > ( n , 0 ) ) 
		  	 ,instack ( vector < bool > ( n , 0 ) ) {} ;
	
private:
	void __cutpoint ( ll now ) {
		d [ now ] = id [ now ] = ++ tot ;
		stack.push_back ( now ) ; instack [ now ] = 1 ;
		ll child = 0 ;
		for ( auto u : mp [ now ] ) {
			if ( !~d [ u ] ) {
				__cutpoint ( u ) ;
				id [ now ] = min ( id [ now ] , id [ u ] ) ;
				if ( id [ u ] >= d [ now ] ) {
					++ child ;
					if ( now != root || child >= 2 ) {
						iscp [ now ] = 1 ;
						ans.push_back ( vector < ll > ( 0 ) ) ;
						auto& bk = ans.back () ;
						while ( instack [ u ] ) {
							bk.push_back ( stack.back () ) ;
							instack [ stack.back () ] = 0 ;
							stack.pop_back () ;
						}
						bk.push_back ( now ) ;
					}
				}
			}
			else id [ now ] = min ( d [ u ] , id [ now ] ) ;
			if ( now == root && child ) {
				ans.push_back ( vector < ll > ( 0 ) ) ;
				auto& bk = ans.back () ;
				while ( instack [ now ] ) {
					bk.push_back ( stack.back () ) ;
					instack [ stack.back () ] = 0 ;
					stack.pop_back () ;
				}
			}
		}
	}

public:
	void cutpoint ( ll now , ll root ) {
		this ->root = root ;
		__cutpoint ( now ) ;
		this ->root = -1 ;
	}

};
*/ 
