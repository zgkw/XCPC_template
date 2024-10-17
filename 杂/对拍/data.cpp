# include <bits/stdc++.h>
using namespace std;

using i64 = long long;

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

int gen(int min, int max) {
    std::uniform_int_distribution<long long> dis(min, max);
    return dis(rng);
}

int  main () {
    std::ios::sync_with_stdio (false);
    std::cin.tie (nullptr) ;
    vector<int> a;
    shuffle(a.begin(), a.end(), rng);

	return 0 ;
}