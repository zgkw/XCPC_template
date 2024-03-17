namespace matrix {
    using i64 = long long ;
    template<typename T>
    struct Matrix:public std::vector<std::vector<T>> {
        using std::vector<std::vector<T>>::vector ;
        Matrix ( int x ):std::vector<std::vector<T>> ( x , std::vector<T> ( x ) ) {} ;
        Matrix ( int x , int y ): std::vector<std::vector<T>> ( x , std::vector<T> ( y ) ) {};
        Matrix ( int x , int y , T c ): std::vector<std::vector<T>> ( x , std::vector<T> ( y , c ) ) {};

        constexpr Matrix operator+ ( Matrix a ) ;
        constexpr Matrix operator- ( Matrix a ) ;
        constexpr Matrix operator* ( Matrix a ) ;
        
        template <typename T1 ,typename T2>
        friend constexpr Matrix<T1> operator* ( Matrix<T1> x , T2 a ) ;

        constexpr Matrix& operator+= ( Matrix a ) ;
        constexpr Matrix& operator-= ( Matrix a ) ;
        constexpr Matrix& operator*= ( Matrix a ) ;

        template <typename T1, typename T2>
        friend constexpr Matrix<T1>& operator*= ( Matrix<T1>& x , T2 a ) ;

/**
 * 一些矩阵操作
 * 矩阵快速幂、转置、逆元
*/
        constexpr Matrix pow ( i64 b ) ;
        constexpr Matrix Transpose ();
        constexpr Matrix inv () ;
    };
    template <typename T>
    constexpr Matrix<T> Matrix<T>::operator+ ( Matrix<T> a ) {
        auto it = *this ;
        int n = (int)a.size () ; int m = (int)a.back().size () ;
        for (int i = 0; i < n; ++ i)
        for (int j = 0; j < m; ++ j)
        it [ i ] [ j ] += a [ i ] [ j ] ;
        return it ;
    }
    template <typename T>
    constexpr Matrix<T> Matrix<T>::operator- ( Matrix<T> a ) {
        auto it = *this ;
        int n = (int)a.size () ; int m = (int)a.back().size () ;
        for (int i = 0; i < n; ++ i)
        for (int j = 0; j < m; ++ j)
        it [ i ] [ j ] -= a [ i ] [ j ] ;
        return it ;
    }
    template <typename T>
    constexpr Matrix<T> Matrix<T>::operator* ( Matrix<T> a ) {
        int n = (int)this->size () ; 
        int mid = (int)a.size () ;
        int m = (int)a.back().size () ;
        Matrix<T> it ( n , m ) ;
        for (int i = 0; i < n; ++ i)
        for (int j = 0; j < m; ++ j)
        for (int k = 0; k < mid; ++ k)
        it [ i ] [ j ] += (*this) [ i ] [ k ] * a [ k ] [ j ] ;
        return it ;
    }
    template <typename T1, typename T2>
    constexpr Matrix<T1> operator* ( Matrix<T1> x , T2 a ) {
        int n = (int)x.size () ;
        int m = (int)x.back().size () ;
        for (int i = 0; i < n; ++ i) 
        for (int j = 0; j < m; ++ j) 
            x [ i ] [ j ] *= a ;
        return x ;
    }

    template <typename T>
    constexpr Matrix<T>& Matrix<T>:: operator+= ( Matrix<T> a ) {
        return *this = *this + a ;
    }
    template <typename T>
    constexpr Matrix<T>& Matrix<T>:: operator-= ( Matrix<T> a ) {
        return *this = *this - a ;
    }
    template <typename T>
    constexpr Matrix<T>& Matrix<T>:: operator*= ( Matrix<T> a ) {
        return *this = *this * a ;
    }
    template <typename T1, typename T2>
    constexpr Matrix<T1>& operator*= ( Matrix<T1>& x , T2 a ) {
        return x = x * a ;
    }

    template <typename T>
    constexpr Matrix<T> Matrix<T>:: pow( i64 b ) {
        auto res = Matrix ( this->size () , this->size () ) ;
        for (int i = 0; i < (int)this->size (); ++ i) 
            res [ i ] [ i ] = 1 ;
        auto a = *this ;
        for (; b; b /= 2, a *= a) 
            if (b % 2) res *= a;
        return res;
    }
    template <typename T>
    constexpr Matrix<T> Matrix<T>:: Transpose() {
        int n = this->back ().size () , m = this->size () ;
        auto it = Matrix ( n , m ) ;
        for (int i = 0; i < n ; ++ i) 
        for (int j = 0; j < m ; ++ j)
            it [ i ] [ j ] = (*this) [ j ] [ i ] ;
        return it;
    }
    template <typename T>
    constexpr Matrix<T> Matrix<T>:: inv() {
        int n = this->size () ;
        Matrix<T> it ( n , 2 * n ) ;
        for ( int i = 0 ; i < n ; ++ i )
        for ( int j = 0 ; j < n ; ++ j )
            it [ i ] [ j ] = (*this) [ i ] [ j ] ;
        for ( int i = 0 ; i < n ; ++ i ) 
            it [ i ] [ i + n ] = 1 ;
        for (int i = 0; i < n; ++ i) {
            int r = i ;
            for (int k = i; k < n; ++ k) 
                if ( (i64)it [ k ] [ i ] ) { r = k ; break ;}
            if ( r != i ) swap ( it [ r ] , it [ i ] ) ;
            if ( !(i64)it [ i ] [ i ] ) return Matrix<T> () ;

            T x = (T) 1 / it [ i ] [ i ] ;
            for (int k = 0; k < n; ++ k) {
                if ( k == i ) continue ;
                T t = it [ k ] [ i ] * x ;
                for (int j = i; j < 2 * n; ++ j) 
                    it [ k ] [ j ] -= t * it [ i ] [ j ] ;
            }
            for (int j = 0; j < 2 * n; ++ j)
                it [ i ] [ j ] *= x ;
        }
        Matrix<T> ans ( n , n ) ;
        for (int i = 0; i < n; ++ i)
        for (int j = 0; j < n; ++ j)
            ans [ i ] [ j ] = it [ i ] [ j + n ] ;
        return ans;
    }
    // namespace Matrix
};
