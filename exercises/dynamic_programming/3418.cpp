// g++ -std=c++17 -DLOCAL template.cpp -o solution
#include <bits/stdc++.h>
using namespace std;

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
    int maximumAmount(vii& coins) {
        int m = (int)coins.size(), n = coins[0].size();

        // dp[i][j][u] = max profit of robot using u neutralize
        int dp[m][n][3];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int u = 0; u < 3; u++) {
                    dp[i][j][u] = -INF;
                }
            }
        }

        /*
            dp[i][j][u] = max(dp[i-1][j][u], dp[i][j-1][u]) + coins[i][j];
            if coins[i][j] < 0
                dp[i][j][u] = max(dp[i-1][j][u-1], dp[i][j-1][u-1])
        */
        dp[0][0][0] = coins[0][0];
        dp[0][0][1] = dp[0][0][2] = max(coins[0][0], 0);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int u = 0; u < 3; u++) {
                    if (i > 0 && dp[i-1][j][u] != -INF) {
                        dp[i][j][u] = max(dp[i][j][u], dp[i-1][j][u] + coins[i][j]);
                    }
                    if (j > 0 && dp[i][j-1][u] != -INF) {
                        dp[i][j][u] = max(dp[i][j][u], dp[i][j-1][u] + coins[i][j]);
                    }
                    if (coins[i][j] < 0 && u > 0) {
                        if (i > 0) {
                            dp[i][j][u] = max(dp[i][j][u], dp[i-1][j][u-1]);
                        }
                        if (j > 0) {
                            dp[i][j][u] = max(dp[i][j][u], dp[i][j-1][u-1]);
                        }
                    }
                }
            }
        }

        int ans = -INF;
        for (int u = 0; u < 3; u++) ans = max(ans, dp[m-1][n-1][u]);
        return ans;
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    int m, n; 
    cin >> m >> n;
    vii coins(m, vi(n));
    REP(i, m) {
        REP(j, n) {
            cin >> coins[i][j];
        }
    }
    cout << Solution().maximumAmount(coins) << '\n';
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