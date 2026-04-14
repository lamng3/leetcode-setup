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

// 3327. Check if DFS Strings Are Palindromes [Hard]
class Solution {
public:
    vii g;
    vi S, E;
    string ss;
    vi nodes;
    vector<char> euler;

    const int BASE = 313;

    void flatten(int u, int p) {
        S[u] = euler.size();
        for (int v : g[u]) {
            if (v != p) flatten(v, u);
        }
        euler.pb(ss[u]);
        nodes.pb(u);
        E[u] = euler.size();
    }

    vector<bool> findAnswer(vi& parent, string s) {
        int n = (int)parent.size();

        g.assign(n, vi());
        for (int i = 0; i < n; i++) {
            if (parent[i] != -1) g[parent[i]].pb(i);
        }

        S.resize(n);
        E.resize(n);
        nodes.clear();
        euler.clear();
        ss = s;
        flatten(0, -1);

        // dbg(S);
        // dbg(E);
        // dbg(nodes);
        // dbg(euler);

        int m = (int)euler.size();

        // pow
        vector<ll> pw(m+1, 1);
        for (int i = 1; i <= m; i++) pw[i] = (pw[i-1] * BASE) % MOD;

        vector<ll> fwd(m+1, 0);
        for (int i = 0; i < m; i++) fwd[i+1] = (fwd[i] * BASE + euler[i]) % MOD;

        vector<ll> bwd(m+1, 0);
        for (int i = 0; i < m; i++) bwd[i+1] = (bwd[i] * BASE + euler[m-1-i]) % MOD;

        auto get_hash = [&](const vector<ll>& h, int l, int r) {
            ll hsh = (h[r+1] - h[l] * pw[r-l+1]) % MOD;
            if (hsh < 0) hsh += MOD;
            return hsh;
        };

        vector<bool> ans(n, false);
        for (int i = 0; i < n; i++) {
            // build string in [S[u], E[u])
            // dbg(u, S[u], E[u]-1);
            int L = S[i], R = E[i] - 1;
            if (L > R) continue;
            ll hfwd = get_hash(fwd, L, R);
            int bL = m-1-R, bR = m-1-L;
            ll hbwd = get_hash(bwd, bL, bR);
            ans[i] = hfwd == hbwd;
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