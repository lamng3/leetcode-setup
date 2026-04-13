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

// 2322. Minimum Score After Removals on a Tree [Hard]
class Solution {
public:
    vii g;
    vi A;
    vi S, E;
    vi euler;
    vi subXor;

    int flatten(int u, int p) {
        S[u] = euler.size();
        euler.pb(A[u]);
        int cxor = A[u];
        for (int v : g[u]) {
            if (v != p) {
                cxor ^= flatten(v, u);
            }
        }
        subXor[u] = cxor;
        E[u] = euler.size();
        return cxor;
    }

    bool is_parent(int u, int v) {
        return S[u] <= S[v] && E[v] <= E[u];
    }

    int minimumScore(vi& nums, vii& edges) {
        A = nums;
        int n = (int)A.size();
        
        S.assign(n, 0);
        E.assign(n, 0);
        subXor.assign(n, 0);

        g.assign(n, vi());
        for (auto& e : edges) {
            int a = e[0], b = e[1];
            g[a].pb(b);
            g[b].pb(a);
        }

        // wlog: rooted at 0
        flatten(0, -1);

        int m = (int)edges.size();
        int ans = INF;
        int tot = subXor[0];

        // choose 2 edges
        for (int i = 0; i < m; i++) {
            for (int j = i+1; j < m; j++) {
                int u1 = edges[i][0], v1 = edges[i][1];
                int u2 = edges[j][0], v2 = edges[j][1];
                
                int c1 = is_parent(u1, v1) ? v1 : u1; 
                int c2 = is_parent(u2, v2) ? v2 : u2;

                int comp1, comp2, comp3;

                if (is_parent(c1, c2)) { // c1 ancestor of c2
                    comp1 = subXor[c2];
                    comp2 = subXor[c1] ^ subXor[c2];
                    comp3 = tot ^ subXor[c1];
                }
                else if (is_parent(c2, c1)) { // c2 ancestor of c1
                    comp1 = subXor[c1];
                    comp2 = subXor[c2] ^ subXor[c1];
                    comp3 = tot ^ subXor[c2];
                }
                else { // disjoint subtrees
                    comp1 = subXor[c1];
                    comp2 = subXor[c2];
                    comp3 = tot ^ subXor[c1] ^ subXor[c2];
                }

                int mx = max({comp1, comp2, comp3});
                int mn = min({comp1, comp2, comp3});
                ans = min(ans, mx - mn);
            }
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