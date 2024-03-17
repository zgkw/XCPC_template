using i64 = int64_t ;

template < typename T , bool op >
struct CFS {
    
    T n , m , add_cnt ;
    vector < T > st , to , nt , w ;

    CFS ( i64 n , i64 m ) : n ( n ) , m ( m ) 
        { st.resize ( n ) , to.resize ( 2 * m + 2 ) , nt.resize ( 2 * m + 2 ) , add_cnt = 1 ;
            if ( op ) w.resize ( 2 * m + 2 ) ;}

    void add ( const i64& u , const i64& v ) {
        to [ ++ add_cnt ] = v ; nt [ add_cnt ] = st [ u ] ; st [ u ] = add_cnt ;}

    void add ( const i64& u , const i64& v , const i64& w ) {
        to [ ++ add_cnt ] = v ; nt [ add_cnt ] = st [ u ] ; this->w [ add_cnt ] = w ; st [ u ] = add_cnt ;}
};

using gs = CFS < i64 , true > ;
