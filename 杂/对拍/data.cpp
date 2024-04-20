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
    uniform_int_distribution<long long> rnd(1, 1000000000000000000);
    auto rnd = [&] {
    	return rnd(rng);
    };

	return 0 ;
}