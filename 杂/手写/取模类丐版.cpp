struct Z {
    static constexpr int P = 998244353;
    int x = 0;
    Z() {}
    Z(i64 x) : x(norm(x % P)) {}
    int norm(int x) {
        if (x >= P) {
            x -= P;
        }
        if (x < 0) {
            x += P;
        }
        return x;
    }
    Z operator-() {
        return -x;
    }
    Z &operator+=(Z rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    Z &operator-=(Z rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    Z &operator*=(Z rhs) {
        x = 1ll * x * rhs.x % P;
        return *this;
    }
    friend Z operator+(Z lhs, Z rhs) {
        return lhs += rhs;
    }
    friend Z operator-(Z lhs, Z rhs) {
        return lhs -= rhs;
    }
    friend Z operator*(Z lhs, Z rhs) {
        return lhs *= rhs;
    }
    friend istream &operator>>(istream &cin, Z &rhs) {
        i64 x;
        cin >> x;
        rhs = x;
        return cin;
    }
    friend ostream &operator<<(ostream &cout, Z rhs) {
        return cout << rhs.x;
    }
};  