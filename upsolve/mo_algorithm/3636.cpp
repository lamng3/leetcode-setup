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

template<class T> struct CoordinateCompressor {
    vector<T> A; 
    
    CoordinateCompressor(vector<T>& arr) {
        A = arr;
        sort(A.begin(), A.end());
        A.erase(unique(A.begin(), A.end()), A.end());
        
        for (auto& x : arr) {
            x = lower_bound(A.begin(), A.end(), x) - A.begin();
        }
    }
    
    T value_at(int rank) const { 
        return A[rank]; 
    }
    
    int rank_of(T x) const { 
        return lower_bound(A.begin(), A.end(), x) - A.begin(); 
    }
    
    int size() const { 
        return A.size(); 
    }
};

struct Query {
    int L, R, threshold, id;
};

// 3636. Threshold Majority Queries [Hard]
class Solution {
public:
    vi subarrayMajority(vi& nums, vii& queries) {
        int n = (int)nums.size(), m = (int)queries.size();
        int block_size = ceil(sqrt(n));

        vector<Query> Q(m);
        for (int i = 0; i < m; i++) {
            int L = queries[i][0], R = queries[i][1];
            int threshold = queries[i][2];
            Q[i] = {L, R, threshold, i};
        }

        // coordinate compression
        vi A = nums;
        CoordinateCompressor<int> cc(A);

        auto mo_cmp = [&](const Query& x, const Query& y) {
            int block_x = x.L / block_size;
            int block_y = y.L / block_size;
            if (block_x != block_y) return block_x < block_y;
            // even/odd optimization: snake-like
            return block_x % 2 ? x.R < y.R : x.R > y.R;
        };
        sort(Q.begin(), Q.end(), mo_cmp);

        int f[10005] = {0};
        set<pii> st;
        REP(i, n) st.insert({0, A[i]});

        auto remove = [&](int i) {
            int x = A[i];
            st.erase({-f[x], x});
            f[x] -= 1;
            st.insert({-f[x], x});
        };

        auto add = [&](int i) {
            int x = A[i];
            st.erase({-f[x], x});
            f[x] += 1;
            st.insert({-f[x], x});
        };

        vi ans(m);
        int mo_L = 0, mo_R = -1;

        for (int i = 0; i < m; i++) {
            int L = Q[i].L, R = Q[i].R;
            int threshold = Q[i].threshold;

            // expand
            while (mo_L > L) add(--mo_L);
            while (mo_R < R) add(++mo_R);

            // shrink
            while (mo_L < L) remove(mo_L++);
            while (mo_R > R) remove(mo_R--);

            // process answer
            auto& best = *st.begin();
            ans[Q[i].id] = -best.fi >= threshold ? cc.value_at(best.se) : -1;
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