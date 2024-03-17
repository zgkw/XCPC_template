template < typename T >
struct Splay {
	struct node {
		array < T , 2 > son = { 0 , 0 } ; array < T , 2 >& s = son ;
		T fa , val , cnt , size ;
		T& p = fa ,& v = val ;
		void init ( i64 father , i64 value ) {
			this->p = father ; this->v = value ; this->cnt = 1 ; }
	} ;
	
	i64 n , root ;
	vector < node > tree ; vector < node > &tr = tree ;
    vector < i64 > res ;

	Splay ( i64 n ): n ( n ) , root ( 0 ) , tree ( n + 3 ) , res ( n + 5 ) 
         { iota ( res.begin () , res.end () , 0 ) ; insert ( -inf ) ; insert ( inf ) ; }
    /*作用：维护树形*/
	void maintain ( i64 x ) {
		tree [ x ].size = tree [ tree [ x ].son [ 0 ] ].size 
			+ tree [ tree [ x ].son [ 1 ] ].size + tree [ x ].cnt ;
	}
    /*作用：将x向上旋转，
    **复杂度：常数*/
	void rotate ( i64 x ) {
		i64 y = tree [ x ].fa ; i64 z = tree [ y ].fa ;
		i64 k = tree [ y ].son [ 1 ] == x ;
		tree [ z ].s [ tree [ z ].s [ 1 ] == y ] = x ;
		tree [ x ].fa = z ; 
		tree [ y ].son [ k ] = tree [ x ].son [ k ^ 1 ] ;
		tree [ tree [ x ].son [ k ^ 1 ] ].fa = y ;
		tree [ x ].son [ k ^ 1 ] = y ;
		tree [ y ].fa = x ;
		maintain ( x ) ; maintain ( y ) ;
	}
    /*作用：将x旋转到根节点
    复杂度：O(logn)*/
	void splay ( i64 x , i64 k ) {
		while ( tree [ x ].fa != k ) {
			i64 y = tree [ x ].fa ; i64 z = tree [ y ].fa ;
			if ( z != k ) 
				( tree [ z ].s [ 1 ] == y ) ^ ( tree [ y ].s [ 1 ] == x ) 
					? rotate ( x ) : rotate ( y ) ;
			rotate ( x ) ;
		}
		if ( k == 0 ) root = x ;
	}
    /*作用：查找x,并将与x值最接近的点旋转到根节点
    复杂度：O(logn)*/
	bool find ( i64 v ) {
		i64 x = root ;
		while ( tree [ x ].s [ v > tree [ x ].v ] && v != tree [ x ].v ) 
			x = tree [ x ].s [ v > tree [ x ].val ] ;
		splay ( x , 0 ) ;
        return tree [ x ].val == v ;
	}
    /*作用：将x的前驱到根节点，并返回
    复杂度：O(logn)*/
	i64 Precursor ( i64 v ) {
		find ( v ) ;
		i64 x = root ;
		if ( tree [ x ].v < v ) return x ;
		x = tree [ x ].s [ 0 ] ;
		while ( tree [ x ].s [ 1 ] ) x = tree [ x ].s [ 1 ] ;
		splay ( x , 0 ) ;
		return x ;
	}
    /*作用：将x的后继到根节点，并返回
    复杂度：O(logn)*/
	i64 Subsequent ( i64 v ) {
		find ( v ) ;
		i64 x = root ;
		if ( tree [ x ].v > v ) return x ;
		x = tree [ x ].s [ 1 ] ;
		while ( tree [ x ].s [ 0 ] ) x = tree [ x ].s [ 0 ] ;
		splay ( x , 0 ) ;
		return x ;
	}
    /*作用：删除x
    复杂度：O(logn)*/
	void erase ( i64 v ) {
		i64 suc = Subsequent ( v ) ;
		i64 pre = Precursor ( v ) ;
		splay ( suc , pre ) ;
		i64 now = tree [ suc ].son [ 0 ] ;
		if ( tree [ now ].cnt > 1 ) 
			-- tree [ now ].cnt , splay ( now , 0 ) ;
		else 
			tree [ suc ].s [ 0 ] = 0 , splay ( suc , 0 ) , res.push_back ( now ) ;
	}
    /*作用：返回x的排名
    复杂度：O(logn)*/
	i64 get_rank ( i64 v ) {
		insert ( v ) ;
		i64 res = tree [ tree [ root ].s [ 0 ] ].size ;
		erase ( v ) ;
		return res ;
	}
    /*作用：返回第k大
    复杂度：O(logn)*/
	i64 get_val ( i64 k ) {
		i64 x = root ; ++ k ;
		while ( true ) {
			i64 ls = tree [ x ].son [ 0 ] ;
			if( k <= tree [ ls ].size ) x = ls ;
			else if ( k <= tree [ ls ].size + tree [ x ].cnt ) break;
			else k -= tree [ ls ].size + tree [ x ].cnt , x = tree [ x ].son [ 1 ];
		}
		splay ( x , 0 ) ;
		return tree [ x ].v ;
	}

	void insert ( i64 v ) {
		i64 x = root , p = 0 ;
		while ( x && tree [ x ].v != v )
			p = x , x = tree [ x ].s [ v > tree [ x ].v ] ;
		if ( x ) ++ tree [ x ].cnt ;
		else {
            assert ( !res.empty () ) ;
			x = res.back () ; res.pop_back () ;
			tree [ p ].s [ v > tree [ p ].v ] = x ;
			tree [ x ].init ( p , v ) ;
		}
		splay ( x , 0 ) ;
	}

    i64 size () { return tree [ root ].size - 2 ;}
    bool empty () { return tree [ root ].size == 2 ;}
    ~Splay () {}
};
