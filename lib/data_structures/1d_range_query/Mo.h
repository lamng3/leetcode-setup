/*
Mo's Algorithm
*/
#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Mo {
    struct Query {
        int L, R, id;
    };

    int BLOCK_SIZE;
    vector<T> A;
    vector<Query> Q;

    Mo(const vector<T>& a, int n_queries) {
        A = a;
        int n = A.size();
        BLOCK_SIZE = ceil(sqrt(n));
        Q.reserve(n_queries);
    }

    void add_query(int L, int R, int id) {
        Q.push_back({L, R, id});
    }

    // expand then shrink
    // add(++mo_R)/add(--mo_L) and remove(mo_R--)/remove(mo_L--)
    inline void process_queries() {
        // window size = mo_R - mo_L + 1 = 0 (empty)
        int mo_L = 0, mo_R = -1;
        for (int i = 0; i < Q.size(); i++) {
            int L = Q[i].L, R = Q[i].R;

            // expand
            while (mo_L > L) add(--mo_L);
            while (mo_R < R) add(++mo_R);

            // shrink
            while (mo_L < L) remove(mo_L--);
            while (mo_R > R) remove(mo_R--);

            // process answer here
            // ans[Q[i].id] = ...
        }
    }
    
    inline void add(int idx) {}
    inline void remove(int idx) {}
};