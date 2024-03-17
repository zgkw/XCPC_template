struct DSU {
    std::vector < int > fa , size_ ;
    std::stack < pair < int& , int > > history_size ;
    std::stack < pair < int& , int > > history_fa ;

    DSU () {}
    DSU ( int n ) {
        init ( n ) ;
    }

    void init ( int n ) {
        fa.resize ( n ) ;
        std::iota ( fa.begin () , fa.end () , 0 ) ;
        size_.assign ( n , 1 ) ;  
    }

    int find ( int x ) {
        while ( x != fa [ x ] ) x = fa [ x ] ;
        return x ;
    }

    int size ( int x ) {
        return size_ [ find ( x ) ] ;
    }

    bool same ( int u , int v ) {
        return find ( u ) == find ( v ) ;
    }

    void merge ( int u , int v ) {
        int x = find ( u ) ;
        int y = find ( v ) ;
        if ( x == y ) return ;
        if ( size_ [ x ] < size_ [ y ] ) std::swap ( x , y ) ;
        history_size.emplace ( size_ [ x ] , size_ [ x ] ) ;
        size_ [ x ] = size_ [ x ] + size_ [ y ] ;
        history_fa.emplace ( fa [ y ] , fa [ y ] ) ;
        fa [ y ] = x ;
    }
    int history () {
        return history_fa.size () ;
    }

    void roll ( int h ) {
        while ( history_fa.size () > h ) {
            history_fa.top ().first = history_fa.top ().second ;
            history_fa.pop () ;
            history_size.top ().first = history_size.top ().second ;
            history_size.pop () ;
        }
    }
}dsu;
