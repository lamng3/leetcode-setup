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

// 2097. Valid Arrangement of Pairs [Hard]
class Solution {
public:
    vector<vector<pii>> g;
    vi path;
    vi seen;

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, idx] = g[u].back();
            g[u].pop_back();
            if (seen[idx]) continue;
            seen[idx] = 1;
            dfs(v);
        }
        path.pb(u);
    }

    vii validArrangement(vii& pairs) {
        int m = (int)pairs.size();

        // coordinate compression
        set<int> uq;
        for (auto& p : pairs) {
            uq.insert(p[0]);
            uq.insert(p[1]);
        }
        vi a(uq.begin(), uq.end());
        map<int,int> rank;
        int n = (int)a.size();
        for (int i = 0; i < n; i++) rank[a[i]] = i;

        vi in(n, 0), out(n, 0);

        g.assign(n, vector<pii>());
        seen.assign(m, 0);
        path.clear();

        for (int i = 0; i < m; i++) {
            int u = rank[pairs[i][0]];
            int v = rank[pairs[i][1]];
            g[u].pb({v, i});
            out[u]++;
            in[v]++;
        }

        int start = 0;
        for (int i = 0; i < n; i++) {
            if (out[i] == in[i]+1) {
                start = i;
                break;
            }
        }

        dfs(start);

        reverse(path.begin(), path.end());

        vii ans;
        for (int i = 1; i < path.size(); i++) {
            ans.pb({a[path[i-1]], a[path[i]]});
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