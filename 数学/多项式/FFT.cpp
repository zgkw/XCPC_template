const double PI = acos(-1.0);

struct Complex {
    double x, y;
    Complex(double _x = 0.0, double _y = 0.0) {
        x = _x;
        y = _y;
    }
    Complex operator-(const Complex &b) const {
        return Complex(x - b.x, y - b.y);
    }
    Complex operator+(const Complex &b) const {
        return Complex(x + b.x, y + b.y);
    }
    Complex operator*(const Complex &b) const {
        return Complex(x * b.x - y * b.y, x * b.y + y * b.x);
    }
    friend ostream &operator<<(ostream &cout, Complex u) {
        return cout << "(" << u.x << ", " << u.y << ")";
    }
};

void change(vector<Complex> &y) {
    int len = y.size();
    for (int i = 1, j = len / 2; i < len - 1; i++) {
        if (i < j) std::swap(y[i], y[j]);
        int k = len / 2;
        while (j >= k) {
            j = j - k;
            k = k / 2;
        }
        if (j < k) j += k;
    }
}

void fft(vector<Complex> &y, int on) {
    int len = y.size();
    change(y);
    for (int h = 2; h <= len; h <<= 1) {
        Complex wn(cos(2 * PI / h), sin(on * 2 * PI / h));
        for (int j = 0; j < len; j += h) {
            Complex w(1, 0);
            for (int k = j; k < j + h / 2; k++) {
                Complex u = y[k];
                Complex t = w * y[k + h / 2];
                y[k] = u + t;
                y[k + h / 2] = u - t;
                w = w * wn;
            }
        }
    }
    if (on == -1) {
        for (int i = 0; i < len; i++) {
            y[i].x /= len;
        }
    }
}

struct Poly : public vector<double> {
    using std::vector<double>::vector;
    friend Poly operator+(Poly x, Poly y) {
        int n = std::max(x.size(), y.size());
        Poly z(n);
        for (int i = 0; i < x.size(); i += 1) {
            z[i] += x[i];
        }
        for (int i = 0; i < y.size(); i += 1) {
            z[i] += y[i];
        }
        return z;
    }
    friend Poly operator*(Poly x, Poly y) {
        int len = x.size() + y.size();
        int n = 1;
        while (n < len) {
            n *= 2;
        }
        vector<Complex> _x(n), _y(n);
        for (int i = 0; i < x.size(); i += 1) {
            _x[i].x = x[i];
        } 
        for (int i = 0; i < y.size(); i += 1) {
            _y[i].x = y[i];
        }
        fft(_x, 1), fft(_y, 1);
        for (int i = 0; i < n; i += 1) {
            _x[i] = _x[i] * _y[i];
        }
        fft(_x, -1);
        Poly ans(n);
        for (int i = 0; i < n; i += 1) {
            ans[i] = _x[i].x;
        }
        return ans;
    }
    Poly operator-() {
        Poly a = *this;
        for (auto &i : a) {
            i = -i;
        }
        return a;
    }
    friend Poly operator-(Poly x, Poly y) {
        return x + -y;
    }

    friend Poly operator*(Poly x, int _mul) {
        for(int i = 0; i < x.size(); ++i) {
            x[i] *= _mul;
        }
        return x;
    };
    friend Poly operator*(int _mul, Poly x) {
        return x * _mul;
    };

    Poly &operator+=(Poly y) {
        return *this = *this + y;
    }
    Poly &operator-=(Poly y) {
        return *this = *this - y;
    }
    Poly &operator*=(Poly y) {
        return *this = *this * y;
    }

    Poly &operator*=(int y) {
        return *this = *this * y;
    }
    template<typename T>
    operator vector<T>() {
        vector<T> a(size());
        for (int i = 0; i < size(); i += 1) {
            a[i] = T(this->operator[](i) + 0.5);
        }
        return a;
    }
};
