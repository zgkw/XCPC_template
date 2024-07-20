template<typename T>
constexpr T power(T a, i64 b) {
    T res = 1;
    for (; b; b /= 2, a *= a) {
        if (b % 2) {
            res *= a;
        }
    }
    return res;
}

template<int P>
struct MInt {
    int x;
    constexpr MInt() : x{} {}
    constexpr MInt(i64 x) : x(norm(x % getMod())) {}

    static int mod;
    constexpr int norm(int x) const {
        if (x < 0) {
            x += getMod();
        }
        else if (x >= getMod()) {
            x -= getMod();
        }
        return x;
    }

    constexpr static void setMod(int x) {
        mod = x;
    }
    constexpr static int getMod() {
        return P > 0 ? P : mod;
    }
    constexpr MInt operator-() {
        return -x;
    }
    constexpr MInt &operator+=(MInt rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr MInt &operator-=(MInt rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr MInt &operator*=(MInt rhs) {
        x = 1ll * x * rhs.x % getMod();
        return *this;
    }

    constexpr MInt inv() {
        assert(x != 0);
        return power(*this, getMod() - 2);
    }
    constexpr MInt &operator/=(MInt rhs) {
        return *this *= rhs.inv();
    }

    constexpr friend MInt operator+(MInt lhs, MInt rhs) {
        return lhs += rhs;
    }
    constexpr friend MInt operator-(MInt lhs, MInt rhs) {
        return lhs -= rhs;
    }
    constexpr friend MInt operator*(MInt lhs, MInt rhs) {
        return lhs *= rhs;
    }
    constexpr friend MInt operator/(MInt lhs, MInt rhs) {
        return lhs /= rhs;
    }
    constexpr friend bool operator==(MInt lhs, MInt rhs) {
        return lhs.x == rhs.x;
    }
    constexpr friend bool operator!=(MInt lhs, MInt rhs) {
        return lhs.x != rhs.x;
    }

    explicit constexpr operator int() const {
        return x;
    }    
    explicit constexpr operator i64() const {
        return x;
    }

    template<class istream>
    constexpr friend istream &operator>>(istream &cin, MInt &rhs) {
        i64 x;
        cin >> x;
        rhs = x;
        return cin;
    }
    template<class ostream>
    constexpr friend ostream &operator<<(ostream &cout, MInt rhs) {
        return cout << rhs.x;
    }
};

template<>
int MInt<0>::mod = 998244353;

constexpr int P = 998244353;
template<int V, int P = P>
constexpr MInt<P> CInv = MInt<P>(V).inv();

using Z = MInt<P>;