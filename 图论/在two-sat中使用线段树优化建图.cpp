    auto either = [ & ] ( ll l , ll r ) {
        g.notClause ( l , 0 , r , 0 ) ;
        s.push ( { l , 1 , r , 1 } ) ;
    };

    auto imply = [ & ] ( ll l , ll r ) {
        g.notClause ( l , 1 , r , 1 ) ;
        s.push ( { l , 0 , r , 0 } ) ;
    };

    function < void ( ll , ll , ll ) > build = [ & ] ( ll l , ll r , ll p ) {
        if ( l == r ) {
            map [ r ] = p ;
            return ;
        } 
        ll mid = ( l + r ) >> 1 ;
        build ( l , mid , p << 1 ) ;
        build ( mid + 1 , r , p << 1 | 1 ) ;
        g.notClause ( p , 0 , p << 1 , 1 ) ;
        g.notClause ( p , 0 , p << 1 | 1 , 1 ) ;
    };
    function < void ( ll , ll , ll , ll , ll , ll ) > modify = [ & ] ( ll l , ll r , ll start , ll end , ll val , ll p ) {
        if ( l > end || r < start ) return ;
        if ( start <= l && r <= end ) {
            imply ( map [ val ] , p ) ;
            return ;
        }
        ll mid = ( l + r ) >> 1 ;
        modify ( l , mid , start , end , val , p << 1 ) ;
        modify ( mid + 1 , r , start , end , val , p << 1 | 1 ) ;
    };
