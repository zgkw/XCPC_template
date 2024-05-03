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
    static int mod;
    constexpr mod_int() : x{} {}
    constexpr mod_int(long long x) : x(norm(x % getMod())) {}

    constexpr int norm(int x) {
        if (x >= getMod()) x -= getMod();
        if (x < 0) x += getMod();
        return x;
    }

    constexpr static void setMod(int x) {
        mod = x;
    }
    constexpr static int getMod() {
        return (P > 0 ? P : mod);
    }
    constexpr mod_int operator-() {
        return -x;
    }

    constexpr mod_int &operator+=(mod_int rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr mod_int &operator-=(mod_int rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr mod_int &operator*=(mod_int rhs) {
        x = 1ll * x * rhs.x % getMod();
        return *this;
    }

    constexpr mod_int inv() {
        return power(*this, P - 2);
    }
    constexpr mod_int &operator/=(mod_int rhs) {
        x = 1ll * x * rhs.inv().x % getMod();
        return *this;
    }

    constexpr friend mod_int operator+(mod_int lhs, mod_int rhs) {
        return lhs += rhs;
    }
    constexpr friend mod_int operator-(mod_int lhs, mod_int rhs) {
        return lhs -= rhs;
    }
    constexpr friend mod_int operator*(mod_int lhs, mod_int rhs) {
        return lhs *= rhs;
    }
    constexpr friend mod_int operator/(mod_int lhs, mod_int rhs) {
        return lhs /= rhs;
    }
    constexpr friend bool operator==(mod_int lhs, mod_int rhs) {
        return lhs.x == rhs.x;
    }
    constexpr friend bool operator!=(mod_int lhs, mod_int rhs) {
        return lhs.x != rhs.x;
    }

    template<class istream>
    constexpr friend istream &operator>>(istream &flow, mod_int &rhs) {
        long long x;
        flow >> x;
        rhs = x;
        return flow;
    }
    template<class ostream>
    constexpr friend ostream &operator<<(ostream &flow, mod_int rhs) {
        return flow << rhs.x;
    }
};

template<>
int mod_int<0>::mod = 998244353;

template<int P, int x>
constexpr mod_int<P> invx = mod_int<P>(x).inv();

constexpr int P = 0;
using Z = mod_int<P>;