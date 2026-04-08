/*
Square Root Decomposition
*/
#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SRD {
    int BLOCK_SIZE;
    vector<T> A;
    vector<T> blocks;

    SRD(int sqrtn, vector<T>& a) : BLOCK_SIZE(sqrtn), blocks(sqrtn, 0) {
        A = a;
        for (int i = 0; i < A.size(); i++) {
            blocks[i/BLOCK_SIZE] += A[i];
        }
    }

    /* O(1) update to set A[x] = v */
    void update(int x, T v) {
        blocks[x/BLOCK_SIZE] -= A[x];
        A[x] = v;
        blocks[x/BLOCK_SIZE] += A[x];
    }

    /* O(sqrt(n)) query for sum of [0,R) */
    T query(int R) {
        T res = 0;
        for (int i = 0; i < R/BLOCK_SIZE; i++) r += blocks[i];
        for (int i = (R/BLOCK_SIZE) * BLOCK_SIZE; i < R; i++) r += A[i];
        return res;
    }

    /* O(sqrt(n)) query for sum of [L,R) */
    T query(int L, int R) {
        return query(R) - query(L-1);
    }
};