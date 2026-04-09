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

    inline void add(int idx) {}
    inline void remove(int idx) {}
};