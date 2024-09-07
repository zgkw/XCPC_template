int mul(int a, int b, int P) {
    return 1ll * a * b % P;
}

template<typename T>
T power(T a, i64 b, i64 P) {
    T res = 1;
    for (; b; b >>= 1) {
        if (b & 1) {
            res = 1ll * res * a % P;
        }
        a = 1ll * a * a % P;
    }
    return res;
}

int sum2(int a) {
    return a * (a + 1) * (2 * a + 1) / 6;
}