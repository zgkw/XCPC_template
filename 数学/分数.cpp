struct fraction {
    ll x , y ;
    fraction () {}
    fraction ( ll x , ll y = 1 ) { *this = norm ( x , y ) ;} 
    fraction norm ( __int128_t x , __int128_t y ) {
        if ( y < 0 ) {
            tie ( x , y ) = make_tuple ( -x , - y ) ;
        }
        __int128_t gcd = __gcd ( x , y ) ;
        fraction res ; res.x = x / gcd , res.y = y / gcd ;
        return res ;
    } 
    fraction operator- () const {
        fraction res = *this ;
        res.x = - res.x;
        return res ;
    }
    fraction &operator+= ( fraction rhs ) & {
        *this = norm ( __int128_t(1) * x * rhs.y + y * rhs.x , __int128_t(1) * y * rhs.y ) ;
        return *this ;
    }
    fraction &operator-= ( fraction rhs ) & {
        *this = norm ( __int128_t(1) * x * rhs.y - y * rhs.x , __int128_t(1) * y * rhs.y ) ;
        return *this ;
    }
    fraction &operator*= ( fraction rhs ) & {
        *this = norm ( __int128_t(1) * x * rhs.x , __int128_t(1) * y * rhs.y ) ;
        return *this ;
    }
    fraction &operator/= ( fraction rhs ) & {
        *this = norm ( __int128_t(1) * x * rhs.y , __int128_t(1) * y * rhs.x ) ;
        return *this ;
    }
    bool operator< ( const fraction& rhs ) const& {
        return __int128_t(1) * x * rhs.y < __int128_t(1) * rhs.x * y ;
    }
    bool operator== ( const fraction& rhs ) const& {
        return __int128_t(1) * x * rhs.y == __int128_t(1) * rhs.x * y ;
    }
    bool operator> ( const fraction& rhs ) const& {
        return __int128_t(1) * x * rhs.y > __int128_t(1) * rhs.x * y ;
    }
    friend fraction operator+ ( fraction lhs , fraction rhs ) {
        fraction res = lhs ;
        return res += rhs ;
    }   
    friend fraction operator- ( fraction lhs , fraction rhs ) {
        fraction res = lhs ;
        return res -= rhs ;
    }   
    friend fraction operator* ( fraction lhs , fraction rhs ) {
        fraction res = lhs ;
        return res *= rhs ;
    }   
    friend fraction operator/ ( fraction lhs , fraction rhs ) {
        fraction res = lhs ;
        return res /= rhs ;
    }
    friend istream& operator>> ( istream& flow , fraction& rhs ) {
        ll x , y ; flow >> x >> y ;
        rhs = { x , y } ;
        return flow ;
    }
    friend ostream& operator<< ( ostream& o , fraction rhs ) {
        return o << complex ( rhs.x , rhs.y ) ;
    }
};