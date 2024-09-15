# include<bits/stdc++.h>
using namespace std;

using f80 = long double;

void solve() {
    for (int i = 1; ; i += 1) {
        system("data.exe > in.txt");
        system("std.exe < in.txt > std.txt");
        f80 begin = clock();
        system("baoli.exe < in.txt > baoli.txt");
        f80 end = clock();
        f80 t = end - begin;
        if (system("fc std.txt baoli.txt")) {
            cout << "case " << i << " Wrong Answer" << endl;
            system("pause");
        } else {
            cout << "case " << i << " Accepted Answer" << endl;
        }
    }
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    signed t = 1;
//    cin >> t;
    while (t --) {
        solve();
    }
    return 0;
}
