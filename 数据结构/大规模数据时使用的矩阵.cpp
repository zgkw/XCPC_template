struct Matrix {
    Z a[4];
    Matrix() :a{} {}
    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
        Matrix res;
        res.a[0] = (lhs.a[0] * rhs.a[0] + lhs.a[1] * rhs.a[2]) ;
        res.a[1] = (lhs.a[0] * rhs.a[1] + lhs.a[1] * rhs.a[3]) ;
        res.a[2] = (lhs.a[2] * rhs.a[0] + lhs.a[3] * rhs.a[2]) ;
        res.a[3] = (lhs.a[2] * rhs.a[1] + lhs.a[3] * rhs.a[3]) ;
        return res;
    }
};
struct Matrix {
    Z a[9];

    Matrix() : a{} {}

    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
        Matrix res;
        res.a[0] = lhs.a[0] * rhs.a[0] + lhs.a[1] * rhs.a[3] + lhs.a[2] * rhs.a[6];
        res.a[1] = lhs.a[0] * rhs.a[1] + lhs.a[1] * rhs.a[4] + lhs.a[2] * rhs.a[7];
        res.a[2] = lhs.a[0] * rhs.a[2] + lhs.a[1] * rhs.a[5] + lhs.a[2] * rhs.a[8];

        res.a[3] = lhs.a[3] * rhs.a[0] + lhs.a[4] * rhs.a[3] + lhs.a[5] * rhs.a[6];
        res.a[4] = lhs.a[3] * rhs.a[1] + lhs.a[4] * rhs.a[4] + lhs.a[5] * rhs.a[7];
        res.a[5] = lhs.a[3] * rhs.a[2] + lhs.a[4] * rhs.a[5] + lhs.a[5] * rhs.a[8];

        res.a[6] = lhs.a[6] * rhs.a[0] + lhs.a[7] * rhs.a[3] + lhs.a[8] * rhs.a[6];
        res.a[7] = lhs.a[6] * rhs.a[1] + lhs.a[7] * rhs.a[4] + lhs.a[8] * rhs.a[7];
        res.a[8] = lhs.a[6] * rhs.a[2] + lhs.a[7] * rhs.a[5] + lhs.a[8] * rhs.a[8];

        return res;
    }
};
