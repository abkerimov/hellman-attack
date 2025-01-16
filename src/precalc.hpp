#pragma once
#include <iostream>
#include <random>
#include <fstream>
#include <openssl/sha.h>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <future>
#include <iomanip>  
#include <openssl/evp.h>
#include <unordered_set>
#include <memory.h>
#include "uint128_t.hpp"


uint16_t hash_and_truncate(const uint128_t& input);
uint128_t generateRandomVector(int N);
uint128_t R(const uint128_t& r, const uint16_t& x);
std::pair<uint16_t*, uint16_t*> build_precalc_table_in_memory(const size_t& k, const size_t& l, const uint128_t& r);
void swap(uint16_t* a, uint16_t* b);
int partition(uint16_t arr1[], uint16_t arr2[], int low, int high);
void quicksort(uint16_t arr1[], uint16_t arr2[], int low, int high);
int binary_search(uint16_t* mega_X_l, uint16_t target, int a, int b);
uint128_t find_preimage(size_t K, size_t L, const uint16_t* mega_X_0, const uint16_t* mega_X_l, uint16_t h, const uint128_t& r);
size_t count_unique_items(const std::vector<uint128_t>& errors);
size_t count_unique_items(const uint16_t* arr, size_t size);
void build_precalc_table_in_memory_multithreaded(size_t number_of_tables, size_t k, size_t l, int threads);
