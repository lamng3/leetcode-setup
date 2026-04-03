#pragma once
using namespace std;

using ll = long long;
const int MOD = 1e9+7;

struct ModInv {
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
        /*
            Fermat's little theorem: 
                a^(p-1) mod p = 1 if (a, p) = 1
            Multiply LHS and RHS by a^(-1)
                => a^(p-2) mod p = a^(-1)
        */
        return binpow(a, MOD-2);
    }
};