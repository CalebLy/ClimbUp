g++ -std=c++20 `
    core/tests/*.cpp `
    core/src/*.cpp `
    -Icore/include `
    -Itests `
    -o tests.exe

./tests.exe