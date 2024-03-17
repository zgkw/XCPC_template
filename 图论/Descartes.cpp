template < class T >
struct Descartes {
	int n ;
	vector < T > v ;
	vector < int > ls , rs ;

	Descartes ( int n ) : ls ( n , -1 ) , rs ( n  , -1 ) , v ( n ) {}
	Descartes ( vector < T >& v ) : n ( (ll)v.size () ) , ls ( n , -1 ) , rs ( n , -1 ) , v ( v ) {}

	int build () /* return root */ {
		vector < int > s ( n ) ;
		int top = 0 ; int root = -1 ;
		for ( int i = 0 ; i < n ; ++ i ) {
			int realtop = top ;
			while ( top != 0 && v [ s [ top ] ] > v [ i ] ) { -- top ;}
			if ( top < realtop ) ls [ i ] = s [ top + 1 ] ;
			if ( top != 0 ) rs [ s [ top ] ] = i ;
			s [ ++ top ] = i ;
		}
		root = s [ 1 ] ;
		assert ( !s.empty () ) ;
		return root ;
	}
};
