struct MCFGraph {
	struct Edge {
		int u, v, nxt;
		i64 f, w;
	};
	static constexpr int inf  = numeric_limits<int>::max();
    vector<Edge> E;
    vector<int> fa, fe, cir, tag, H;
    vector<i64> pre;

    MCFGraph(int n, int m = 0): fa(n), fe(n), cir(n), tag(n), H(n), pre(n), E(2) {
        E.reserve(2 * m + 4);
    }
	
	int tot = 1;
	
	void addEdge(int u, int v, i64 f, i64 w) {
        E.push_back({u, v, H[u], f, + w}), H[u] = ++ tot;
        E.push_back({v, u, H[v], 0, - w}), H[v] = ++ tot;
	}
	
	int now = 0;
	
	void InitZCT(int x, int e, int nod = 1) {
		fa[x] = E[fe[x] = e].u, tag[x] = nod;
		for (int i = H[x]; i; i = E[i].nxt)
			if(tag[E[i].v] != nod && E[i].f)
				InitZCT(E[i].v, i, nod);
	}
	
	i64 sum(int x) {
		if(tag[x] == now) return pre[x];
		return tag[x] = now, pre[x] = sum(fa[x]) + E[fe[x]].w;
	}
	
	i64 PushFlow(int x) {
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
	
	
	pair<i64, i64> flow(int S, int T) {
		addEdge(T, S, inf, - inf);
		InitZCT(T, 0, ++ now);
		
		tag[T] = ++ now, fa[T] = 0;
		
		bool Run = 1;
	    i64 MinC = 0;
		
		while(Run) {
			Run = 0;
			for (int i = 2; i <= tot; ++ i) 
				if(E[i].f && E[i].w + sum(E[i].u) - sum(E[i].v) < 0)
					MinC += PushFlow(i), Run = 1;
		}
		
		MinC += E[tot].f * inf;

		return {E[tot].f, MinC};
	}
};