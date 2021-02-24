@echo off

g++ -Wall -std=c++14 ^
 ./third_party/strlib/strlib.cpp ./src/data.cpp ./src/main.cpp ^
 -I./third_party/strlib -I./third_party/mydefines 