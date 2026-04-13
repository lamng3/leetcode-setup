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

// 3900. Longest Balanced Substring After One Swap [Medium]
class Solution {
public:
    int find(const string& s) {
        int n = (int)s.size();

        int res = 0;

        map<int,vi> mp;
        mp[1].pb(-1);

        vi pref(n);
        pref[0] = s[0] - '0';
        for (int i = 1; i < n; i++) pref[i] = pref[i-1] + (s[i] - '0');

        int ones = pref[n-1];
        int zeros = n - pref[n-1];

        for (int R = 0; R < n; R++) {
            int x = 2 * pref[R] - R;

            // d = 2: more zeros than ones
            // d = -2: more ones than zeros
            for (int d : {0, -2, 2}) {
                if (mp.count(x+d)) {
                    for (int L : mp[x+d]) {
                        int in_ones = pref[R] - (L == -1 ? 0 : pref[L]);
                        int in_zeros = R - L - in_ones;

                        // verify that a swap is possible
                        int rones = ones - in_ones;
                        int rzeros = zeros - in_zeros;

                        // check if there are zeros and ones outside [L..R] to swap
                        bool ok = (d == 0) || (d == 2 && rones > 0) || (d == -2 && rzeros > 0);
                        if (ok) {
                            res = max(res, R - L);
                            break;
                        }
                    }
                }
            }

            mp[x].pb(R);
        }

        return res;
    }

    int longestBalanced(string s) {
        // perform 1 swap: best to swap 1 to 0 or 0 to 1
        // if (i,j) belongs to a longest substring, it wont affect the balancedness
        // 2 * pref[R] - R = 2 * pref[L-1] - (L-1)

        // [L..R] we can find substrings differ by at most 1
        // pick [L..R] such that 2 * (pref[R] - pref[L-1]) = (R - L + 1) - 2
        // 2 * pref[R] - R + 2 = 2 * pref[L-1] - (L-1)
        return find(s);
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