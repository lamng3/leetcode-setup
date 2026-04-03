#pragma once
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MOD = 1e9+7;

ll binpow(ll a, ll b) {
    a %= MOD;
    ll res = 1;
    while (b) {
        if (b % 2) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

ll modInverse(ll a) {
    return binpow(a, MOD-2);
}

struct ModFact {
    int n;
    vector<ll> fac, inv;

    ModFact(int _n) : n(_n) {
        fac.resize(n+1);
        inv.resize(n+1);
        fac[0] = 1;
        for (int i = 1; i <= n; i++) fac[i] = (fac[i-1] * i) % MOD;
        // inv[n] = (1 * 2 * ... * n) ^ (-1)
        // inv[n-1] = inv[n] * n
        inv[n] = modInverse(fac[n]);
        for (int i = n; i >= 1; i--) inv[i-1] = (inv[i] * i) % MOD;
    }

    ll nCk(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fac[n] * inv[k] % MOD * inv[n-k] % MOD;
    }
};