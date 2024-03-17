template < typename T >
struct Min_Cost_Flow {
    using i64 = int64_t ;
    struct info { T v , f , c ; info ( T v , T f , T c ): v ( v ) , f ( f ) , c ( c ) {}};

    i64 n ;
    vector < info > e ;
    vector < vector < T > > g ;
    std::vector < i64 > dis ,h ;
    std::vector < T > pre ;

    Min_Cost_Flow ( i64 n ): n ( n ) , g ( n ) {}
    void add ( T u , T v , T f , T c ) {
        if ( c < 0 ) {
            g [ u ].push_back ( e.size () ) ;
            e.emplace_back ( v , 0 , c ) ;
            g [ v ].push_back ( e.size () ) ;
            e.emplace_back ( u , f , -c ) ;            
        } else {
            g [ u ].push_back ( e.size () ) ;
            e.emplace_back ( v , f , c ) ;
            g [ v ].push_back ( e.size () ) ;
            e.emplace_back ( u , 0 , -c ) ;             
        }
    }
    bool dijkstra ( i64 s , i64 t ) {
        dis.assign ( n , std::numeric_limits < i64 >::max () ) ;
        pre.assign ( n , -1 ) ;
        priority_queue < pair < i64 , i64 > , std::vector < pair < i64 , i64 > > 
                , std::greater < pair < i64 , i64 > > > que ;
        dis [ s ] = 0 ;
        que.emplace ( 0 , s ) ;
        while ( !que.empty () ) {
            auto [ d , u ] = que.top () ;
            que.pop () ;
            if ( dis [ u ] < d ) continue ;
            for ( i64 i : g [ u ] ) {
                auto [ v , f , c ] = e [ i ] ;
                if ( f > 0 && dis [ v ] > d + h [ u ] - h [ v ] + c ) {
                    dis [ v ] = d + h [ u ] - h [ v ] + c ;
                    pre [ v ] = i ;
                    que.emplace ( dis [ v ] , v ) ;
                }
            }
        }
        return dis [ t ] != std::numeric_limits < i64 >::max () ;
    }
    
    std::pair < i64 , i64 > flow ( i64 s , i64 t ) {
        int flow = 0 ;
        i64 cost = 0 ;
        h.assign ( n , 0 ) ;
        while ( dijkstra ( s , t ) ) {
            for ( int i = 0 ; i < n ; ++ i ) h [ i ] += dis [ i ] ;
            i64 aug = std::numeric_limits < i64 >::max () ;
            for ( int i = t ; i != s ; i = e [ pre [ i ] ^ 1 ].v )
                aug = std::min ( aug , (i64)e [ pre [ i ] ].f ) ;
            for ( int i = t ; i != s ; i = e [ pre [ i ] ^ 1 ].v ) {
                e [ pre [ i ] ].f -= aug ;
                e [ pre [ i ] ^ 1 ].f += aug ;
            }
            flow += aug ;
            cost += h [ t ] * aug ;
        }
        return std::make_pair ( flow , cost ) ;
    }
    
    struct Edge {
        int from;
        int to;
        T cap;
        T flow;
    };

    std::vector<Edge> edges() {
        std::vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].v;
            x.to = e[i].v;
            x.cap = e[i].f + e[i + 1].f;
            x.flow = e[i + 1].f;
            a.push_back(x);
        }
        return a;
    }
};
