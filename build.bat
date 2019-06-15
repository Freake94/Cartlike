@echo off
g++ -static main.cc -std=c++17 -O3 -s -ffast-math -fopenmp -msse -msse2 -march=native -fno-exceptions -flto ^
 -lmingw32 -lglfw3 -lopengl32 -lglu32 -lgdi32
