template<typename A, typename B> 
ostream &operator<<(ostream &cout, const pair<A, B> &p) { 
    return cout << '(' << p.first << ", " << p.second << ')'; 
}
template<typename Tp, typename T = typename 
    enable_if<!is_same<Tp, string>::value, typename Tp::value_type>::type> 
ostream &operator<<(ostream &cout, const Tp &v) { 
    cout << '{'; 
    string sep; 
    for (const T &x : v) 
        cout << sep << x, sep = ", "; 
    return cout << '}'; 
}

void Output() { cerr << endl; }
template<typename Head, typename... Tail> 
void Output(Head H, Tail... T) {
    cerr << ' ' << H; Output(T...); 
}

# define ps cerr << "YES" << endl 
# define debug(...) \
         cerr << "(" << #__VA_ARGS__ << "):" << endl,\
         Output(__VA_ARGS__)