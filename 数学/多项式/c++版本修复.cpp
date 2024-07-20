# include <vector>
# include <tuple>   // for std::tie
# include <utility> // for std::make_pair

using cp = complex<double>;
using __It = vector<cp>::iterator;

struct cxx20_It : public __It {
    using __It::__It;
    cxx20_It(const __It& it) : __It(it) {}
    cp &operator [](int x) {
        return *(*this + x);
    }
};
using It = cxx20_It;