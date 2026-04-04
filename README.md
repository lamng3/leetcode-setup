# leetcode-setup

Competitive programming workflow for LeetCode in C++.

![demo](assets/demo.gif)

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
Fetch and test your solution against LeetCode's example test cases.
- First run fetches and caches test cases (does not compile/run).
- Subsequent runs compile and test against cached examples.
```
cptest 930              # fetch test cases (first time) or run tests
cptest 930 --refetch    # re-fetch test cases from LeetCode
```
Test cases are cached in `tests/<number>.json`.

## Setup

Clone the repo, then add to `~/.zshrc` (replace `<path>` with where you cloned it):
```zsh
export LEETCODE_SETUP="<path>/leetcode-setup"

cpbuild() {
    g++ -std=c++17 -DLOCAL -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined "$1" -o solution
    echo "Compiled $1 -> solution"
}
cpgen() {
    python3 "$LEETCODE_SETUP/cpgen.py" "$1"
}
cptest() {
    python3 "$LEETCODE_SETUP/cptest.py" "$@"
}
```

## Inspirations
- [Benjamin Qi cp-notebook](https://github.com/bqi343/cp-notebook)
