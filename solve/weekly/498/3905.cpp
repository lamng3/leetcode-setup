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

// 3905. Multi Source Flood Fill [Medium]
class Solution {
public:
    vii colorGrid(int n, int m, vii& sources) {
        vii g(n, vi(m, 0));

        map<pii, pii> cmap;
        queue<pii> nodes;
        for (auto& src : sources) {
            int r = src[0], c = src[1], color = src[2];
            g[r][c] = color;
            cmap[{r,c}] = {color, 0};
            nodes.push({r,c});
        }

        int dr[4] = {1, -1, 0, 0};
        int dc[4] = {0, 0, 1, -1};

        auto inrange = [&](int r, int c) {
            return 0 <= r && r < n && 0 <= c && c < m;
        };

        int t = 0;

        while (!nodes.empty()) {
            int sz = (int)nodes.size();
            
            REP(i, sz) {
                auto node = nodes.front();
                nodes.pop();
                
                // current coordinates, color, and time
                int r = node.fi, c = node.se;
                auto& curr = cmap[{r, c}];
                int color = curr.fi, time = curr.se;

                REP(j, 4) {
                    // next coordinates, color, and time
                    int nr = r + dr[j], nc = c + dc[j];
                    int ntime = time + 1;
                    
                    // if next coordinates out of range
                    if (!inrange(nr, nc)) continue;
                    
                    // if color in next coordinates exist
                    if (cmap.count({nr, nc})) {
                        auto& prev = cmap[{nr, nc}];
                        int pcolor = prev.fi, ptime = prev.se;
                        
                        // cannot be updated
                        if (ntime > ptime) continue;

                        // if can be updated earlier
                        if (ntime < ptime) {
                            cmap[{nr, nc}] = {color, ntime};
                            continue;
                        }

                        int ncolor = max(color, pcolor);
                        cmap[{nr, nc}] = {ncolor, ntime};
                    }
                    else {
                        nodes.push({nr, nc});
                        cmap[{nr, nc}] = {color, ntime};
                    }
                }
            }
        }

        REP(i, n) {
            REP(j, m) {
                auto& curr = cmap[{i, j}];
                int color = curr.fi;
                g[i][j] = color;
            }
        }

        return g;
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