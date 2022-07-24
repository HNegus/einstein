# Einstein

Programs to solve the challenges of the [Einstein letter block puzzle](https://professorpuzzle.com/products/puzzles/einstein/einsteins-letter-puzzle/)

## Python
The Python program in the `python` directory (unsuccesfully) attempts to solve challenge 6 through brute-force

To run use
```bash
cd python
pip3 install -r requirements.txt
python3 einstein.py
```

## C++
The C++ version in the `src` directory (unsuccesfully) attempts to solve challenge 6 with an evolutionary algorithm (`ea.cpp`). It also includes a brute-force algorithm (`solve()` in `blocks.cpp`).

To run use
```
mkdir build
cmake -B build -S .
cmake --build build
./bin/einstein
```