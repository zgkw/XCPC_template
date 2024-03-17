struct CEG {

	ll n , tot = -1 ; 
	vector < pll > edge ;
	vector < vector < ll > > mp ;
	vector < ll > d , id ;
	vector < ll > ans ;

	explicit CEG ( ll n ) : 
		 n ( n ) , d ( vector < ll > ( n , -1 ) ) , id ( vector < ll > ( n , - 1 ) ) ,
		 	mp ( vector < vector < ll > > ( n ) ) {} ;

private :
	void __cutedge ( ll now , ll __edge ) {
		d [ now ] = id [ now ] = ++ tot ;
		for ( auto tag : mp [ now ] ) {
			auto& here = edge [ tag ].second ;
			if ( !~d [ here ] ) {
				__cutedge ( here , tag ) ;
				id [ now ] = min ( id [ now ] , id [ here ] ) ;
				if ( id [ here ] > d [ now ] ) {
					ans.push_back ( tag ) ;
				}
			}
			else if ( tag != ( __edge ^ 1 ) ) {
				id [ now ] = min ( id [ here ] , id [ now ] ) ;
			}
		}
	}
 
public:
	void add ( ll u , ll v ) {
		edge.emplace_back ( u , v ) ;
		mp [ u ].push_back ( (ll)edge.size () - 1 ) ;
	}
	void cutedge ( ll u , ll __edge ) {
		__cutedge ( u , __edge ) ;
	}
};
