template<typename T>
T power(T x, long long b) {
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
    mod_int() : x{} {}
    mod_int(long long x) : x(norm(x % getMod())) {}

    int norm(int x) {
        if (x >= P) x -= P;
        if (x < 0) x += P;
        return x;
    }

    static void setMod(int x) {
        mod = x;
    }
    static int getMod() {
        return (P > 0 ? P : mod);
    }
    mod_int operator-() {
        return -x;
    }

    mod_int &operator+=(mod_int rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    mod_int &operator-=(mod_int rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    mod_int &operator*=(mod_int rhs) {
        x = 1ll * x * rhs.x % getMod();
        return *this;
    }

    mod_int inv() {
        return power(*this, P - 2);
    }
    mod_int &operator/=(mod_int rhs) {
        x = 1ll * x * rhs.inv().x % getMod();
        return *this;
    }

    friend mod_int operator+(mod_int lhs, mod_int rhs) {
        return lhs += rhs;
    }
    friend mod_int operator-(mod_int lhs, mod_int rhs) {
        return lhs -= rhs;
    }
    friend mod_int operator*(mod_int lhs, mod_int rhs) {
        return lhs *= rhs;
    }
    friend mod_int operator/(mod_int lhs, mod_int rhs) {
        return lhs /= rhs;
    }
    friend bool operator==(mod_int lhs, mod_int rhs) {
        return lhs.x == rhs.x;
    }
    friend bool operator!=(mod_int lhs, mod_int rhs) {
        return lhs.x != rhs.x;
    }

    template<class istream>
    friend istream &operator>>(istream &input, mod_int &rhs) {
        long long x;
        input >> x;
        rhs = x;
        return input;
    }
    template<class ostream>
    friend ostream &operator<<(ostream &output, mod_int rhs) {
        return output << rhs.x;
    }
};

template<>
int mod_int<0>::mod = 998244353;

constexpr int P = 1e9 + 7;
using Z = mod_int<P>;