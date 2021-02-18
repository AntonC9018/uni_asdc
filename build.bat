@echo off

g++ -Wall -std=c++14 ^
 ../strlib/strlib.cpp ./src/data.cpp ./src/main.cpp ^
 -I../includes -I../strlib