all:	
	g++ -O3 -o main ./src/main.cpp ./src/precalc.cpp ./src/uint128_t.cpp -lssl -lcrypto -pthread