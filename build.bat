@echo off

g++ -Wall -std=c++14 ^
 ../strlib/strlib.cpp ./src/*.cpp ^
 -I../includes -I../strlib