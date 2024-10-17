template<class T>
T power(T a, i64 n) {
	T res = 1;
	while(n) {
		if(n & 1) res *= a;
		a *= a;
		n >>= 1;
	}
	return res;
}

vector<int> rev;
vector<Z> roots{Z(0), Z(1)};

Z findPrimitiveRoot() {
    Z i(2);
    int k = __builtin_ctz(P - 1);
    while (true) {
        if (power(i, (P - 1) / 2).x != 1) {
            break;
        }
        i += 1;
    }
    return power(i, (P - 1) >> k);
}

Z primitiveRoot = findPrimitiveRoot();

void dft(vector<Z> &a) {
    int n = a.size();

    if (int(rev.size())!= n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }

    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            swap(a[i], a[rev[i]]);
        }
    }

    if (roots.size() < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            Z e = power(primitiveRoot, 1 << (__builtin_ctz(P - 1) - k - 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e;
            }
            k++;
        }
    }

    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                Z u = a[i + j];
                Z v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
            }
        }
    }
}

void idft(vector<Z> &a) {
    int n = a.size();
    reverse(a.begin() + 1, a.end());
    dft(a);
    // Z inv = (Z(1 - P) / n);
	Z inv = Z(1 - P) * power(Z(n), P - 2);
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}

struct Poly : public vector<Z> {
    using vector<Z>::vector;

    template<class F>
    Poly(int n, F f) : vector<Z>(n) {
        for (int i = 0; i < n; i++) {
            (*this)[i] = f(i);
        }
    }

    // 多项式加，可不写
    friend Poly operator+(const Poly &a, const Poly &b) {
        Poly res(max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] += a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] += b[i];
        }
        return res;
    }

    // 多项式减，可不写
    friend Poly operator-(const Poly &a, const Poly &b) {
        Poly res(max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] += a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] -= b[i];
        }
        return res;
    }

    friend Poly operator*(Poly a, Poly b) {
        if (a.size() == 0 || b.size() == 0) {
            return Poly();
        }
        if (a.size() < b.size()) {
            std::swap(a, b);
        }
        int n = 1, tot = a.size() + b.size() - 1;
        while (n < tot) {
            n *= 2;
        }
        a.resize(n);
        b.resize(n);
        dft(a);
        dft(b);
        for (int i = 0; i < n; ++i) {
            a[i] *= b[i];
        }
        idft(a);
        a.resize(tot);
        return a;
    }

    // 启发式合并NTT
    static Poly merge(vector<Poly> &a) {
        assert(!a.empty());
        priority_queue<pair<int, int>> H; // {-siz, index}
        for (int i = 0; i < a.size(); ++i) {
            H.emplace(-a[i].size(), i);
        }

        while (H.size() >= 2) {
            int idx1 = H.top().second;
            H.pop();
            int idx2 = H.top().second;
            H.pop();

            Poly res = a[idx1] * a[idx2];
            a[idx1].clear();
            a[idx2].clear();
            a[idx1] = res;
            H.emplace(-a[idx1].size(), idx1);
        }
        return a[H.top().second];
    };
};
