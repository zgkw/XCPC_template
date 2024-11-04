# include <bits/stdc++.h>
using namespace std;

template<typename U, typename V> 
ostream &operator<<(ostream &cout, const pair<U, V> &p) {
    return cout << "(" << p.first << ", " << p.second << ")";
}

template<typename T, typename Tp = typename enable_if<!is_same<T, string>::value, typename T::value_type>::type>
ostream &operator<<(ostream &cout, const T &u) {
    string sep;
    cout << "{";
    for (const Tp &v : u) {
        cout << sep << v;
        sep = ", ";
    }
    return cout << "}";
}

void Output() {
    cerr << endl;
}

template<typename H, typename ...T>
void Output(H h, T ...t) {
    cerr << h << ' ';
    Output(t...);
}

# define debug(...) cerr << "(" << #__VA_ARGS__ << "):\n"; Output(__VA_ARGS__)
# define ps cerr << "YES" << endl