#!/bin/bash
g++ -O3 -o main ./src/main.cpp -lssl -lcrypto
./main
