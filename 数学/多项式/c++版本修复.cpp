# include <vector>
# include <tuple>   // for std::tie
# include <utility> // for std::make_pair

using cp = complex<double>;
using _Tp = vector<cp>::iterator;

struct cxx20_It : public _Tp {
    using _Tp::_Tp;
    cxx20_It(const _Tp& it) : _Tp(it) {}
    cp &operator [](int x) {
        return *(*this + x);
    }
};
using It = cxx20_It;