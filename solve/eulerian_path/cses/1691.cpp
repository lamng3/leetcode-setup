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

class Solution {
// LeetCode method function
// void solve() {}
public:

};

#if !defined(CPTEST) && (defined(LOCAL) || defined(ONLINE_JUDGE))
void preprocess() {
    
}

int n, m; 
vector<vector<pii>> g;
vi path;
vi seen;

void dfs(int u) {
    while (!g[u].empty()) {
        auto [v, sid] = g[u].back();
        g[u].pop_back();
        if (seen[sid]) continue;
        seen[sid] = 1;
        dfs(v);
    }
    path.pb(u);
}

// cout << Solution().solve() << '\n';
void solve() {
    cin >> n >> m;

    vi deg(n, 0);
    g.resize(n);
    seen.assign(m, 0);

    REP(i, m) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        g[a].pb({b, i});
        g[b].pb({a, i});
        deg[a]++;
        deg[b]++;
    }

    // eulerian circuit
    for (int i = 0; i < n; i++) {
        if (deg[i] % 2) {
            cout << "IMPOSSIBLE" << '\n';
            return;
        }
    }

    dfs(0);

    if (path.size() != m+1) {
        cout << "IMPOSSIBLE" << '\n';
        return;
    }

    for (int i = 0; i < path.size(); i++) {
        cout << (path[i]+1) << (i == path.size()-1 ? '\n' : ' ');
    }
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