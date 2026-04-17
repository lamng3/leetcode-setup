// g++ -std=c++17 -DLOCAL template.cpp -o solution
#include <bits/stdc++.h>
using namespace std;

#pragma region Debug
#ifdef LOCAL
template<typename T, typename U>
ostream& operator<<(ostream& os, const pair<T,U>& p) {
    return os << "(" << p.first << ", " << p.second << ")";
}
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (int i = 0; i < (int)v.size(); i++) os << (i ? ", " : "") << v[i];
    return os << "]";
}
template<typename T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    int i = 0;
    for (auto& x : s) os << (i++ ? ", " : "") << x;
    return os << "}";
}
template<typename K, typename V>
ostream& operator<<(ostream& os, const map<K,V>& m) {
    os << "{";
    int i = 0;
    for (auto& [k, v] : m) os << (i++ ? ", " : "") << k << ": " << v;
    return os << "}";
}
void _dbg() { cerr << endl; }
template<typename T, typename... A>
void _dbg(T t, A... a) { cerr << " " << t; if constexpr(sizeof...(a)) cerr << ","; _dbg(a...); }
#define dbg(...) cerr << "\033[35m[" << #__VA_ARGS__ << "]\033[0m:", _dbg(__VA_ARGS__)
#else
#define dbg(...)
#endif
#pragma endregion

using ll = long long;
using vi = vector<int>;
using vii = vector<vector<int>>;
using pii = pair<int, int>;

#define REP(i, n) for (int i = 0; i < (n); i++)
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define FORD(i, a, b) for (int i = (a); i >= (b); i--)
#define RFOR(i, n) for (int i = (n) - 1; i >= 0; i--)

#define fi first
#define se second
#define pb push_back

const int INF = 1e9;
const ll LLINF = 2e18;

const int MOD = 1e9+7;
const int MOD_NTT = 998244353; // number theoretic transform (NTT)

// 2003. Smallest Missing Genetic Value in Each Subtree [Hard]
class Solution {
public:
    vii g;
    vi S, E;
    vi euler;
    vi nodes;

    vi A;
    vi pos;

    const int MAXV = 1e5+5;
    vi stmax, stmin;

    void build(int v, int tl, int tr) {
        if (tl == tr) {
            stmax[v] = stmin[v] = pos[tl];
            return;
        }
        int tm = tl + (tr - tl) / 2;
        build(v * 2 + 1, tl, tm);
        build(v * 2 + 2, tm+1, tr);
        stmax[v] = max(stmax[v * 2 + 1], stmax[v * 2 + 2]);
        stmin[v] = min(stmin[v * 2 + 1], stmin[v * 2 + 2]);
    }

    int query(int v, int tl, int tr, int ql, int qr) {
        if (tl == tr) return tl;
        int tm = tl + (tr - tl) / 2;
        if (stmin[v * 2 + 1] < ql || stmax[v * 2 + 1] > qr) {
            return query(v * 2 + 1, tl, tm, ql, qr);
        }
        else {
            return query(v * 2 + 2, tm+1, tr, ql, qr);
        }
    }

    void flatten(int u, int p) {
        S[u] = euler.size();
        euler.pb(A[u]);
        nodes.pb(u);
        for (int v : g[u]) {
            if (v != p) flatten(v, u);
        }
        E[u] = euler.size();
    }

    vi smallestMissingValueSubtree(vi& parents, vi& nums) {
        A = nums;
        int n = (int)parents.size();

        g.assign(n, vi());
        for (int i = 0; i < n; i++) {
            if (parents[i] != -1) g[parents[i]].pb(i);
        }

        S.assign(n, 0);
        E.assign(n, 0);
        euler.clear();

        flatten(0, -1);

        pos.assign(MAXV + 1, -1);
        for (int i = 0; i < euler.size(); i++) {
            pos[euler[i]] = i;
        }

        stmin.assign(4 * MAXV, 0);
        stmax.assign(4 * MAXV, 0);
        build(0, 1, MAXV);

        vi ans(n);
        for (int u = 0; u < n; u++) {
            int L = S[u], R = E[u]-1;
            ans[u] = query(0, 1, MAXV, L, R);
        }
        return ans;
    }
};

#if !defined(CPTEST) && (defined(LOCAL) || defined(ONLINE_JUDGE))
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    
}

int main() {
    // freopen("name.in", "r", stdin);
    // freopen("name.out", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    preprocess();
    int tt = 1;
    // cin >> tt;
    while (tt--) solve();
    return 0;
}
#endif