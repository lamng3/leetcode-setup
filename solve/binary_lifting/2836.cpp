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

const int MAXNODES = 1e5+5;
const int MAXSTEPS = 40;

int recv[MAXNODES][MAXSTEPS];
ll pathsum[MAXNODES][MAXSTEPS];

// 2836. Maximize Value of Function in a Ball Passing Game [Hard]
class Solution {
public:
    ll getMaxFunctionValue(vi& receiver, ll k) {
        int n = (int)receiver.size();

        memset(recv, -1, sizeof recv);
        memset(pathsum, 0, sizeof pathsum);

        REP(i, n) {
            recv[i][0] = receiver[i];
            pathsum[i][0] = i;
        }

        FOR(j, 1, MAXSTEPS-1) {
            REP(i, n) {
                recv[i][j] = recv[recv[i][j-1]][j-1];
                pathsum[i][j] = pathsum[i][j-1] + pathsum[recv[i][j-1]][j-1];
            }
        }

        auto getPassK = [&](int node) {
            ll res = 0;
            REP(j, MAXSTEPS) {
                if (k & (1LL << j)) {
                    res += pathsum[node][j];
                    node = recv[node][j];
                }
            }
            res += node;
            return res;
        };

        ll ans = 0;
        REP(i, n) ans = max(ans, getPassK(i));
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