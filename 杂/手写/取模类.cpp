template<typename T>
constexpr T power(T x, long long b) {
    T res = 1;
    while (b) {
        if (b & 1) res *= x;
        x *= x;
        b >>= 1;
    }
    return res;
}

template<int P>
struct mod_int {
    int x;
    constexpr static int mod{0};
    constexpr mod_int ():x{}{}
    constexpr mod_int (long long x):x(norm(x % getMod())){}
    
    constexpr int norm(int x) {
        if (x >= P) x -= P;
        if (x < 0) x += P;
        return x;
    }
    constexpr void setMod(int x) {
        mod = x;
    }
    constexpr int getMod() {
        return (P > 0 ? P : mod);
    }
    constexpr mod_int operator-() {
        return -x;
    }
    constexpr mod_int &operator+= (mod_int rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr mod_int &operator-= (mod_int rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr mod_int &operator*= (mod_int rhs) {
        x = 1ll * x * rhs.x % getMod();
        return *this;
    }
    constexpr mod_int inv () {
        return power(*this, P - 2);
    }
    constexpr mod_int &operator/= (mod_int rhs) {
        x = 1ll * x * rhs.inv().x % getMod();
        return *this;
    }
    constexpr friend mod_int operator+ (mod_int lhs, mod_int rhs) {
        return lhs += rhs;
    }
    constexpr friend mod_int operator- (mod_int lhs, mod_int rhs) {
        return lhs -= rhs;
    }
    constexpr friend mod_int operator* (mod_int lhs, mod_int rhs) {
        return lhs *= rhs;
    }
    constexpr friend mod_int operator/ (mod_int lhs, mod_int rhs) {
        return lhs /= rhs;
    }
    constexpr friend bool operator== (mod_int lhs, mod_int rhs) {
        return lhs.x == rhs.x;
    }
    constexpr friend bool operator!= (mod_int lhs, mod_int rhs) {
        return lhs.x != rhs.x;
    }
    constexpr friend istream &operator>> (istream& flow, mod_int& rhs) {
        long long x;
        flow >> x;
        rhs = x;
        return flow;
    }
    constexpr friend ostream &operator<< (ostream& flow, mod_int rhs) {
        return flow << rhs.x;
    }
};

template<int P, int x>
constexpr mod_int<P> invx = mod_int<P>(x).inv();

constexpr int P = 998244353;
using Z = mod_int<P>;