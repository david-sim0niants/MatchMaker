# MatchMaker

## Build
```sh
mkdir build
cd build
cmake ../
make -j$(nproc)
```

## Run
```
./bin/match-maker
```

## Test
In the build directory, run CMake with the `-DBUILD_TESTS=ON` option, rebuild and run:
```
./test/test_all
```

## Games Included
```sh
./bin/rock-paper-scissors
./bin/tic-tac-toe-3x3
./bin/tic-tac-toe-4x4
./bin/tic-tac-toe-5x5
```
