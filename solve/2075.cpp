// g++ -std=c++17 -DLOCAL template.cpp -o solution
#include <bits/stdc++.h>
using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (int i = 0; i < v.size(); i++) {
        os << v[i] << (i == v.size() - 1 ? "" : ", ");
    }
    return os << "]";
}

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
    string decodeCiphertext(string encodedText, int rows) {
        int n = (int)encodedText.size();
        int cols = n/rows;

        string ans = "";
        for (int i = 0; i < cols; i++) {
            int r = 0, c = i;
            string run = "";
            while (r < rows && c < cols) {
                run += encodedText[r*cols+c];
                r++;
                c++;
            }
            if (run != "") ans += run;
        }

        while (!ans.empty() && ans.back() == ' ') ans.pop_back();
        return ans;
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    string encodedText; 
    getline(cin, encodedText);
    int rows; 
    cin >> rows;
    cout << Solution().decodeCiphertext(encodedText, rows) << '\n';
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