constexpr double pi = 3.141592653589793115997963468544185161590576171875, pi2 = 2 * pi;

using cp = complex<double>;
using It = vector<cp>::iterator;
vector<cp> a;

template<int on, int n>
void fft(It a) {
    if (n == 1)
        return;
    if (n == 2) {
        tie(a[0], a[1]) = make_pair(a[0] + a[1], a[0] - a[1]);
        return;
    }
    constexpr int h = n >> 1, q = n >> 2;
    if (on == -1) {
        fft<on, h>(a);
        fft<on, q>(a + h);
        fft<on, q>(a + h + q);
    }
    cp w(1, 0), w3(1, 0);
    constexpr cp wn(cos(pi2 / n), on * sin(pi2 / n)),
	             wn3(cos(pi2 * 3 / n), on * sin(pi2 * 3 / n));
    for (int i = 0; i < q; i++) {
        if (on == -1) {
            cp tmp1 = w * a[i + h], tmp2 = w3 * a[i + h + q],
               x = a[i], y = tmp1 + tmp2,
               x1 = a[i + q], y1 = tmp1 - tmp2;
            y1 = cp(y1.imag(), -y1.real());
            a[i] += y;
            a[i + q] += y1;
            a[i + h] = x - y;
            a[i + h + q] = x1 - y1;
        } else {
            cp x = a[i] - a[i + h], y = a[i + q] - a[i + h + q];
            y = cp(y.imag(), -y.real());
            a[i] += a[i + h];
            a[i + q] += a[i + h + q];
            a[i + h] = (x - y) * w;
            a[i + h + q] = (x + y) * w3;
        }
        w *= wn;
        w3 *= wn3;
    }
    if (on == 1) {
        fft<on, h>(a);
        fft<on, q>(a + h);
        fft<on, q>(a + h + q);
    }
}
template<>
void fft<1, 0> (It a) {}
template<>
void fft<-1, 0> (It a) {}


template<int on>
void FFT(It a, int n) {
    # define C(x)\
        case 1 << x:\
            fft<on, 1 << x>(a);\
            break
    switch (n) {
        C(1);
        C(2);
        C(3);
        C(4);
        C(5);
        C(6);
        C(7);
        C(8);
        C(9);
        C(10);
        C(11);
        C(12);
        C(13);
        C(14);
        C(15);
        C(16);
        C(17);
        C(18);
        C(19);
        C(20);
        C(21);
    }
    # undef C
}

vector<cp> _x;
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
    friend Poly operator*(Poly &x, Poly &y) {
        int len = x.size() + y.size() + 1;
        int n = 1;
        while (n < len) {
            n *= 2;
        }
        _x.assign(n, {});
        for (int i = 0; i < x.size(); i += 1) {
            _x[i].real(x[i]);
        } 
        for (int i = 0; i < y.size(); i += 1) {
            _x[i].imag(y[i]);
        }
        FFT<1>(_x.begin(), n);
        for (int i = 0; i < n; i += 1) {
            _x[i] *= _x[i];
        }
        FFT<-1>(_x.begin(), n);
        Poly ans(n);
        const double inv = 0.5 / n;
        for (int i = 0; i < n; i += 1) {
            ans[i] = _x[i].imag() * inv;
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
    Poly &operator+=(Poly y) {
        return *this = *this + y;
    }
    Poly &operator-=(Poly y) {
        return *this = *this - y;
    }
    Poly &operator*=(Poly y) {
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