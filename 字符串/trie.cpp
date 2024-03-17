struct Trie {
    struct Node {
        ll sum = 0 ;
        array <Node*, 27> ch ;
        Node () {ch.fill ( nullptr );} ; 
    };
    Node* root ;

    Trie ( bool have_default ) {
        root = new Node () ;
    }

    void add ( const vector < int >& v ) {
        auto p = root ;
        for ( auto u : v ) {
            if ( not p->ch [ u ] ) { p -> ch [ u ] = new Node () ;}
            p = p ->ch [ u ] ;
            ++ p -> sum ;
        }
    }

    void add ( string s , char off = 'a' ) {
        vector < int > v ;
        for ( auto u : s ) {
            v.push_back ( u - off ) ;
        }
        v.push_back ( 27 ) ;
    }
};
