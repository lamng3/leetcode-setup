# leetcode-setup

Competitive programming workflow for LeetCode in C++.

## Tools

### `cpbuild <file.cpp>`
Debug compilation with sanitizers and warnings.
```
cpbuild solve/930.cpp
./solution
```

### `cpgen <problem_number>`
Fetch a LeetCode problem and generate `solve/<number>.cpp` with the template and method stub.
```
cpgen 930
```

### `cptest <problem_number>`
Compile and test your solution against LeetCode's example test cases.
```
cptest 930
cptest 930 --refetch   # re-fetch test cases from LeetCode
```
Test cases are cached in `tests/<number>.json`.

## Setup

Add to `~/.zshrc`:
```zsh
cpbuild() {
    g++ -std=c++17 -DLOCAL -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined "$1" -o solution
    echo "Compiled $1 -> solution"
}
cpgen() {
    python3 ~/Desktop/source/leetcode-setup/cpgen.py "$1"
}
cptest() {
    python3 ~/Desktop/source/leetcode-setup/cptest.py "$@"
}
```

## Inspirations
- [Benjamin Qi cp-notebook](https://github.com/bqi343/cp-notebook)
