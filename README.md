# leetcode-setup

## cpbuild <filename.cpp>
While solving problems, you will inevitably run into "Segmentation Faults" (accessing arrays out of bounds) or weird integer overflows. 

A "debug" compilation command that catches these errors instantly and tells you exactly which line caused them:
```
g++ -std=c++17 -DLOCAL -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined template.cpp -o solution
```

Command is long to type every time. Create an alias by updating `~/.zshrc` file:
```
cpbuild() {
    g++ -std=c++17 -DLOCAL -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined "$1" -o solution
    echo "Compiled $1 -> solution"
}
```

## Inspirations
- [Benjamin Qi cp-notebook](https://github.com/bqi343/cp-notebook)