#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;

using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using ci = const int;

constexpr int inSZ = 1 << 17, outSZ = 1 << 21;
char ibuf[inSZ], *in1 = ibuf, *in2 = ibuf;
char obuf[outSZ], *out1 = obuf, *out2 = obuf + outSZ;

inline char gc() {
	if (__builtin_expect(in1 == in2 && (in2 = (in1 = ibuf) + 
		fread(ibuf, 1, inSZ, stdin), in1 == in2), 0)) return EOF;
	return *in1++;
}

inline void flush() {
	fwrite(obuf, 1, out1 - obuf, stdout);
	out1 = obuf;
}


#define pc(c) (*out1++ = c)

inline void read(int &x) {
	x = 0; static char c;
	while (!isdigit((c = gc())));
	while (x = 10 * x + (c ^ 48), isdigit(c = gc()));
}

inline void write(int x) {
	if (__builtin_expect(out1 + 20 > out2, 0)) flush();
	int tot = 0;
	do { pc(x % 10 + 48); } while (++tot, x /= 10);
	reverse(out1 - tot, out1);
}

constexpr int N = 1 << 21;
constexpr int mod = 998244353, g = 3;
inline int dil(int x) { return x >> 31 ? x + mod : x; }
inline int mu(int x, int y) { return u64(x) * y % mod; }

inline int qpow(int x, int y) {
	int z = 1;
	do { if (y & 1) z = mu(z, x); x = mu(x, x); } while (y >>= 1);
	return z;
}

inline int bceil(int x) { return 1 << __lg(x - 1) + 1; }

int w[N >> 1], iw[N >> 1];
void preNTT(int n) {
	int l = bceil(n) >> 1;
	w[0] = iw[0] = 1;
	for (int i = 1; i < l; i <<= 1) {
		w[i] = qpow(g, (mod - 1 >> 2) / i);
		iw[i] = qpow(g, mod - 1 - (mod - 1 >> 2) / i);
	}
	for (int i = 1; i < l; ++i) {
		w[i] = mu(w[i & (i - 1)], w[i & -i]);
		iw[i] = mu(iw[i & (i - 1)], iw[i & -i]);
	}
}

struct poly : vector<int> {
	friend void dif(poly &f, int lim) {
		f.resize(lim);
		for (int l = lim >> 1, r = lim; l; l >>= 1, r >>= 1)
			for (int i = 0, *o = w; i != lim; i += r, ++o)
				for (int j = i, x, y; j != i + l; ++j)
					x = dil(f[j] - mod), y = mu(f[j + l], *o), f[j] = x + y, f[j + l] = x - y + mod;
		for (int i = 0; i < lim; ++i) f[i] = dil(f[i] - mod);
	}
	friend void dit(poly &f, int lim) {
		f.resize(lim);
		for (int l = 1, r = 2; l < lim; l <<= 1, r <<= 1)
			for (int i = 0, *o = iw; i != lim; i += r, ++o)
				for (int j = i, x, y; j != i + l; ++j)
					x = f[j], y = mod - f[j + l], f[j] = dil(x - y), f[j + l] = mu(x + y, *o);
		ci iv = mod - (mod - 1) / lim;
		for (int i = 0; i < lim; ++i) f[i] = mu(f[i], iv);
	}
	friend poly operator*(poly f, poly g) {
		int len = f.size() + g.size() - 1;
		int lim = bceil(len);
		f.resize(lim), g.resize(lim);
		preNTT(lim); dif(f, lim); dif(g, lim);
		for (int i = 0; i < lim; ++i) f[i] = mu(f[i], g[i]);
		dit(f, lim); f.resize(len);
		return forward<poly>(f);
	}
};
poly F, G;

int main() {
	int n, m, lim;
	read(n), read(m);
	F.resize(n + 1), G.resize(m + 1);
	lim = bceil(n + m + 1);
	for (int i = 0; i <= n; ++i) read(F[i]);
	for (int i = 0; i <= m; ++i) read(G[i]);
	F = F * G;
	for (int i = 0; i <= n + m; ++i) write(F[i]), pc(' ');

	return flush(), 0;
}