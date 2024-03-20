/**
 * ����ʽ��չ�� 
*/
//114514
namespace ExPoly {
    template<int P = ::P, class T1 , class T2>
    constexpr static Poly<P> Lagrange (T1 x, T2 y) {
        int n = x.size () ;
        vector <Poly<>> M ( 4 * n );
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                M[p] = Poly{(int)-x[l], 1};
            } else {
                int m = (l + r) / 2;
                build(2 * p, l, m);
                build(2 * p + 1, m, r);
                M[p] = M[2 * p] * M[2 * p + 1];
            }
        };
        build ( 1 , 0 , n ) ;
        auto M_ = M[1].deriv().eval( x );
        for ( int i = 0 ; i < n ; ++ i ) {
            M_ [ i ] = y [ i ] * M_ [ i ].inv () ;
        }
        vector<Poly<>> f (4 * n) ;
        std::function<void(int, int, int)> work = [&](int p, int l, int r) -> void {
            if (r - l == 1) {
                if (l < n) {
                    f[p] = Poly{(int)M_[l]};
                }
            } else {
                int m = (l + r) / 2;
                work(2 * p, l, m);
                work(2 * p + 1, m, r);
                f [ p ] = f [ 2 * p ] * M [ 2 * p + 1 ] + f [ 2 * p + 1 ] * M [ 2 * p ] ;
            }
        };
        work ( 1 , 0 , n ) ;
        return f[1];
    }
/**
 *���ã��Զ���ʽ����ƽ�Ʋ���
 *ʱ�临�Ӷ�O(nlog(n))
 */
    template <int P = ::P>
    constexpr static Poly<P> Polynomial_translation ( Poly<P> f , int k ) {
        i64 n = (i64)f.size () - 1 ;
        Poly<P> g ( n + 1 ) ;
        Z res = 1 ;
        for (int i = 0; i <= n; ++ i) {
            g [ n - i ] = res * comb.invfac ( i ) ;
            res *= k ;
            f [ i ] *= comb.fac ( i ) ;
        }
        Poly<P> here = g * f ;
        here = here.shift ( - n ) ;
        for (int i = 0; i <= n ; ++ i) {
            here [ i ] *= comb.invfac ( i ) ;
        }
        return here ;
    }
/**
 *���ã�����ͬ��n��i \in ( 0 , n ) �����n����ͬ��Ԫ�ػ���Ϊi���ǿռ��ķ�����
 *�ڶ���Stirling��
 *ʱ�临�Ӷ�O(nlog(n))
 */
    template <int P = ::P>
    constexpr static Poly<P> Second_Stirling_Same_N ( int n ) {
        Poly<P> f (n + 1), g (n + 1) ;
        for (int i = 0; i <= n; ++ i) {
            g [ i ] = ( i & 1 ? (Z)-1 : Z(1) ) * comb.invfac ( i ) ;
            f [ i ] = power ( (Z)i , n ) * comb.invfac ( i ) ; 
        }
        f *= g ; f.resize ( n + 1 ) ;
        return f ;
    }
/**
 *���ã�����ͬ��k�Բ�ͬn �����n����ͬ��Ԫ�ػ���Ϊk���ǿռ��ķ�����
 *�ڶ���Stirling��
 *ʱ�临�Ӷ�O(nlog(n))
 */
    template <int P = ::P>
    constexpr static Poly<P> Second_Stirling_Same_K ( int Max_n , int k ) {
        comb.init ( Max_n + 1 ) ;
        Poly<P> f ( vector <Z>( comb._invfac.begin () , comb._invfac.begin () + Max_n + 1 ) ) ;
        f [ 0 ] = 0 ;
        f = f.pow ( k , Max_n + 1 ) ;
        for ( int i = 0 ; i <= Max_n ; ++ i ) {
            f [ i ] = f [ i ] * comb.fac ( i ) * comb.invfac ( k ) ;
        }
        return f ;
    }
/**
 *���ã�����ͬ��n��i \in ( 0 , n ) �����n����ͬ��Ԫ�ػ���Ϊi���ǿ��ֻ��ķ�����
 *��һ��Stirling��
 *ʱ�临�Ӷ�O(nlog(n))
 */
    template <int P = ::P>
    constexpr static Poly<P> First_Stirling_Same_N ( int n ) {
        ll len = __lg ( n ) ;
        Poly <P> f = { 1 } ;
        ll cnt = 0 ;
        for ( int i = len ; i >= 0 ; -- i ) {
            f *= Polynomial_translation ( f , cnt ) ;cnt <<= 1 ;
            if ( n >> i & 1 ) f *= Poly { cnt , 1 } , cnt += 1 ;
        }
        return f ;
    }
/**
 *���ã�����ͬ��k�Բ�ͬn �����n����ͬ��Ԫ�ػ���Ϊk�����ֻ��ķ�����
 *��һ��Stirling��
 *ʱ�临�Ӷ�O(nlog(n))
 */
    template <int P = ::P>
    constexpr static Poly<P> First_Stirling_Same_K ( int Max_n , int k ) {
        comb.init ( Max_n + 1 ) ;
        Poly<P> f ( comb._inv.begin () , comb._inv.begin () + Max_n + 1 );
        f = f.pow ( k , Max_n + 1 ) ;
        for (int i = 0; i <= Max_n; ++ i) {
            f [ i ] *= comb.fac ( i ) * comb.invfac ( k ) ; 
        } 
        return f;
    }
};
