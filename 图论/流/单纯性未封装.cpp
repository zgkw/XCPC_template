#include <bits/stdc++.h>
using namespace std;

namespace MCMF {
    using i64 = long long;
    using namespace std;
	constexpr int INF  = numeric_limits<int>::max();
	constexpr int MAXN = 5E3 + 3, MAXE = 5E4;
	
	struct Edge {
		int u, v, nxt;
		i64 f, w;
	} E[MAXE * 2 + 4];
	
	int H[MAXN], tot = 1;
	
	inline void addEdge(int u, int v, i64 f, i64 w) {
		E[++ tot] = {u, v, H[u], f, + w}, H[u] = tot;
		E[++ tot] = {v, u, H[v], 0, - w}, H[v] = tot;
	}
	
	i64 pre[MAXN];
	int fa[MAXN], fe[MAXN], cir[MAXN], tag[MAXN];
	int now = 0, S = MAXN - 2, T = S + 1;
	
	inline void Init_ZCT(int x, int e, int nod = 1) {
		fa[x] = E[fe[x] = e].u, tag[x] = nod;
		for (int i = H[x]; i; i = E[i].nxt)
			if(tag[E[i].v] != nod && E[i].f)
				Init_ZCT(E[i].v, i, nod);
	}
	
	inline i64 Sum(int x) {
		if(tag[x] == now) return pre[x];
		return tag[x] = now, pre[x] = Sum(fa[x]) + E[fe[x]].w;
	}
	
	inline i64 Push_Flow(int x) {
		int rt = E[x].u, lca = E[x].v, p = 2, del = 0, cnt = 0;
		++ now;
		while(rt) tag[rt] = now, rt = fa[rt];
		while(tag[lca] != now) tag[lca] = now, lca = fa[lca];
		
		i64 f = E[x].f, cost = 0;
		
		for (int u = E[x].u; u != lca; u = fa[u]) {
			cir[++ cnt] = fe[u];
			if(E[fe[u]].f < f) del = u, p = 0, f = E[fe[u]].f;
		}
		
		for (int u = E[x].v; u != lca; u = fa[u]) {
			cir[++ cnt] = fe[u] ^ 1;
			if(E[fe[u] ^ 1].f < f) del = u, p = 1, f = E[fe[u] ^ 1].f;
		}
		
		cir[++ cnt] = x;
		
		for (int i = 1; i <= cnt; ++ i)
			cost += E[cir[i]].w * f, E[cir[i]].f -= f, E[cir[i] ^ 1].f += f;
		
		if(p == 2) return cost;
		int u = E[x].u, v = E[x].v;
		if(p == 1) std::swap(u, v);
		int le = x ^ p, lu = v, tmp;
		
		while(lu != del) {
			le ^= 1, -- tag[u], std::swap(fe[u], le);
			tmp = fa[u], fa[u] = lu, lu = u, u = tmp;
		}
		
		return cost;
	}
	
	
	inline pair<i64, i64> flow() {
		addEdge(T, S, INF, - INF);
		Init_ZCT(T, 0, ++ now);
		
		tag[T] = ++ now, fa[T] = 0;
		
		bool Run = 1;
	    i64 MinC = 0;
		
		while(Run) {
			Run = 0;
			for (int i = 2; i <= tot; ++ i) 
				if(E[i].f && E[i].w + Sum(E[i].u) - Sum(E[i].v) < 0)
					MinC += Push_Flow(i), Run = 1;
		}
		
		MinC += E[tot].f * INF;
		
		return {E[tot].f, MinC};
	}
}

using namespace MCMF;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    for (int i = 0; i < m; i += 1) {
        int u, v, w, c;
        cin >> u >> v >> w >> c;
        addEdge(u, v, w, c);
    }
    addEdge(S, s, INF, 0);
    addEdge(t, T, INF, 0);
    auto [flow, cost] = MCMF::flow();

    cout << flow << ' ' << cost << '\n';
	
	return 0;
}
