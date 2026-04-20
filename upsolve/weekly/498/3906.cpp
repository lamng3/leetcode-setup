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

// dp[pos][tight][last_path_digit]
ll dp[20][2][10];

int ispath[16];

// 3906. Count Good Integers on a Grid Path [Hard]
class Solution {
public:
    ll solveDp(const string& num, int pos, int tight, int lpd) {
        if (pos == 16) return 1;

        if (dp[pos][tight][lpd] != -1) return dp[pos][tight][lpd];

        ll res = 0;
        int ub = tight ? (num[pos] - '0') : 9;

        for (int d = 0; d <= ub; d++) {
            if (ispath[pos]) {
                if (d < lpd) continue;
                res += solveDp(num, pos+1, tight & (d == ub), d);
            }
            else {
                res += solveDp(num, pos+1, tight & (d == ub), lpd);
            }
        }

        return dp[pos][tight][lpd] = res;
    }

    ll countUpTo(const string& x) {
        string num = string(16 - (int)x.size(), '0') + x;
        memset(dp, -1, sizeof dp);
        ll res = solveDp(num, 0, 1, 0);
        return res;
    }

    ll countGoodIntegersOnPath(ll L, ll R, string directions) {
        memset(ispath, 0, sizeof ispath);
        int curr = 0;
        ispath[curr] = 1;
        for (auto& c : directions) {
            if (c == 'D') curr += 4;
            else if (c == 'R') curr += 1;
            ispath[curr] = 1;
        }

        ll countL = countUpTo(to_string(L-1));
        ll countR = countUpTo(to_string(R));
        return countR - countL;
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