# include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i64 = long long;

int  main () {
# ifndef cin 
    std::ios::sync_with_stdio (false);
    std::cin.tie (nullptr) ;
# endif
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto rnd = [&] (i64 l, i64 r) {
        uniform_int_distribution<long long> _rnd(l, r);
    	return _rnd(rng);
    };
    vector<int> a;
    shuffle(a.begin(), a.end(), rng);

	return 0 ;
}