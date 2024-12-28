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
#include <iomanip>  // For std::hex
#include <openssl/evp.h>
#include <unordered_set>
#include <memory.h>

#define s_variant 16
#define h_variant 32


typedef struct {
    uint64_t lo, hi;
    bool operator[](size_t index) const {
        if (index >= 128) {
            throw std::out_of_range("Index must be between 0 and 127");
        }

        if (index < 64) {
            return (lo >> index) & 1;
        } else {
            return (hi >> (index - 64)) & 1;
        }
    }
    void to_hex(char* hex_str) const {
        const char hex_digits[] = "0123456789abcdef";

        for (int i = 0; i < 16; ++i) {
            hex_str[i] = hex_digits[(hi >> (4 * (15 - i))) & 0xF];  
            hex_str[i + 16] = hex_digits[(lo >> (4 * (15 - i))) & 0xF]; 
        }
        hex_str[32] = '\0';  
    }
} uint128_t;

uint128_t operator+(const uint128_t& a, const uint128_t& b) {
    uint128_t result;
    result.lo = a.lo + b.lo;
    result.hi = a.hi + b.hi + (result.lo < a.lo);
    return result;
}
uint128_t operator>>(const uint128_t& value, size_t shift) {
    uint128_t result = {0, 0};

    if (shift >= 128) {
        result.hi = 0;
        result.lo = 0;
    } else if (shift >= 64) {
        result.lo = value.hi >> (shift - 64);
        result.hi = 0;
    } else {
        result.lo = (value.lo >> shift) | (value.hi << (64 - shift));
        result.hi = value.hi >> shift;
    }

    return result;
}
uint128_t operator<<(const uint128_t& value, size_t shift) {
    uint128_t result = {0, 0};

    if (shift >= 128) {
        result.hi = 0;
        result.lo = 0;
    } else if (shift >= 64) {
        result.hi = value.lo << (shift - 64);
        result.lo = 0;
    } else {
        result.hi = (value.hi << shift) | (value.lo >> (64 - shift));
        result.lo = value.lo << shift;
    }

    return result;
}

void print_uint128(const uint128_t& a) {
    for (int i = 127; i >= 0; --i) {
        std::cout << a[i];
    }
    std::cout << std::endl;
}


uint16_t hash_and_truncate(const uint128_t& input) {
    constexpr size_t n = s_variant; 

    char hex_input[33];
    input.to_hex(hex_input);

    std::string hex_input_without_zeroes = "";
    bool leading_zeros = false;
    for (int i = 0; i < 32; i++) {
        if (hex_input[i] != '0') {
            leading_zeros = true;
        }
        if (leading_zeros) {
            hex_input_without_zeroes += hex_input[i];
        }
    }
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating EVP context" << std::endl;
        return 0;
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha384(), nullptr) != 1) {
        std::cerr << "Error initializing digest" << std::endl;
        EVP_MD_CTX_free(ctx);
        return 0;
    }

    if (EVP_DigestUpdate(ctx, hex_input_without_zeroes.c_str(), hex_input_without_zeroes.length()) != 1) {
        std::cerr << "Error updating digest" << std::endl;
        EVP_MD_CTX_free(ctx);
        return 0;
    }

    unsigned char hash[SHA384_DIGEST_LENGTH];
    unsigned int hash_len = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        std::cerr << "Error finalizing digest" << std::endl;
        EVP_MD_CTX_free(ctx);
        return 0;
    }

    EVP_MD_CTX_free(ctx);


    uint16_t trunc = 0;
    trunc |= hash[0];             
    trunc |= hash[1] << 8;       
    return trunc & 0xFFFF;  
}


uint128_t generateRandomVector(int N) {
    int r_length = 128 - N;
    if (r_length < 0) {
        throw std::invalid_argument("n must be less than or equal to 128");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

    uint128_t random;
    random.hi = dis(gen);
    random.lo = dis(gen);

    return random << N; 
}


uint128_t R(const uint128_t& r, const uint16_t& x) {
    constexpr size_t n = s_variant;  
    constexpr size_t r_size = 128 - n;  

    if (n > 16) {
        throw std::invalid_argument("n must not exceed the size of x (16 bits).");
    }

    uint128_t result;

    result = r << n;

    result.lo |= x;

    return result;
}

std::pair<uint16_t*, uint16_t*> build_precalc_table_in_memory(const size_t& k, const size_t& l, const uint128_t& r) {
    std::minstd_rand gen(std::random_device{}());
    std::uniform_int_distribution<uint16_t> dist(0, UINT16_MAX);

    auto * arrayX_0 = new uint16_t[k];
    auto * arrayX_l = new uint16_t[k];
    
    for (size_t i = 0; i < k; ++i) {
        uint16_t x_0(dist(gen));
        uint16_t x_i = x_0;
        for (size_t j = 0; j < l; ++j) {
            x_i = hash_and_truncate(R(r, x_i));
        }
        arrayX_0[i] = x_0;
        arrayX_l[i] = x_i;
    }

    return {arrayX_0, arrayX_l}; 
}


void swap(uint16_t* a, uint16_t* b) {
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}


int partition(uint16_t arr1[], uint16_t arr2[], int low, int high) {

    uint16_t pivot = arr2[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr2[j] <= pivot) {
            i++;
            swap(&arr2[i], &arr2[j]); 
            swap(&arr1[i], &arr1[j]); 
        }
    }

    swap(&arr2[i + 1], &arr2[high]); 
    swap(&arr1[i + 1], &arr1[high]);
    return i + 1;
}


void quicksort(uint16_t arr1[], uint16_t arr2[], int low, int high) {
    if (low < high) {
        int pi = partition(arr1, arr2, low, high);
        quicksort(arr1, arr2, low, pi - 1);
        quicksort(arr1, arr2, pi + 1, high);
    }
}


int binary_search(uint16_t* mega_X_l, uint16_t target, int a, int b) {
    if (a >= b) { 
        return -1; 
    }
    
    int pivot = a + (b - a) / 2;
    
    if (mega_X_l[pivot] == target) {
        return pivot; 
    }
    
    if (mega_X_l[pivot] < target) {
        return binary_search(mega_X_l, target, pivot + 1, b); 
    }
    
    return binary_search(mega_X_l, target, a, pivot); 
}


uint128_t find_preimage(
    size_t K,
    size_t L,
    const uint16_t* mega_X_0,
    const uint16_t* mega_X_l,
    uint16_t h,
    const uint128_t& r) {
    
    uint16_t y = h;

    for (size_t j = 0; j < L; ++j) {
        int left = 0, right = K - 1, found_index = -1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (mega_X_l[mid] == y) {
                found_index = mid;
                break;
            } else if (mega_X_l[mid] < y) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        if (found_index != -1) {
            uint16_t x = mega_X_0[found_index];
            for (size_t m = 0; m < L - j - 1; ++m) {
                x = hash_and_truncate(R(r, x));
            }

            uint128_t maybe_hash = R(r, x);
            if (hash_and_truncate(maybe_hash) == h) {
                return maybe_hash;
            }
        }

        y = hash_and_truncate(R(r, y));
    }

    return {uint64_t(0), uint64_t(0)};
}


void build_precalc_table_in_memory_multithreaded(size_t number_of_tables, size_t k, size_t l, int threads = 8) {
    std::vector<uint128_t> r_s;
    for (size_t i = 0; i < number_of_tables; ++i) {
        r_s.push_back(generateRandomVector(0));
    }

    size_t range = k / threads;

    std::vector<std::future<std::pair<uint16_t*, uint16_t*>>> futures;
    for (size_t i = 0; i < number_of_tables; ++i) {
        uint128_t r = r_s[i];
        for (size_t t = 0; t < threads; ++t) {
            futures.push_back(std::async(std::launch::async, build_precalc_table_in_memory, range, l, r));
        }
    }

    std::vector<std::vector<uint16_t*>> arraysX_0(number_of_tables);
    std::vector<std::vector<uint16_t*>> arraysX_l(number_of_tables);

    int j = 0;
    for (auto& future : futures) {
        try {
            auto [arrayX_0, arrayX_l] = future.get();
            size_t table_index = j / threads;
            arraysX_0[table_index].push_back(arrayX_0);
            arraysX_l[table_index].push_back(arrayX_l);
            ++j;
        } catch (const std::exception& e) {
            std::cerr << "Future error: " << e.what() << std::endl;
            throw;
        }
    }

    std::vector<uint16_t*> array_mega_X_0(number_of_tables);
    std::vector<uint16_t*> array_mega_X_l(number_of_tables);

    for (size_t i = 0; i < number_of_tables; ++i) {
        uint16_t* arrX0 = new uint16_t[k];
        uint16_t* arrXl = new uint16_t[k];
        for (size_t t = 0; t < arraysX_0[i].size(); ++t) {
            memcpy(arrX0 + t * range, arraysX_0[i][t], range * sizeof(uint16_t));
            memcpy(arrXl + t * range, arraysX_l[i][t], range * sizeof(uint16_t));
            delete[] arraysX_0[i][t]; 
            delete[] arraysX_l[i][t];
        }
        array_mega_X_0[i] = arrX0;
        array_mega_X_l[i] = arrXl;
    }

    for (size_t i = 0; i < number_of_tables; ++i) {
        quicksort(array_mega_X_0[i], array_mega_X_l[i], 0, k - 1);
    }
    
    std::vector<uint128_t> errors;
    constexpr int N = 10000;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < number_of_tables; ++j) {
            uint16_t randomV = hash_and_truncate(generateRandomVector(0));
            uint128_t pre = find_preimage(k, l, array_mega_X_0[j], array_mega_X_l[j], randomV, r_s[j]);
            if (pre.lo != uint64_t{0} || pre.hi != uint64_t{0}) {
                errors.push_back(pre);
                break;
            }
            if (j == number_of_tables - 1) {
                errors.push_back(pre);
            }
        }
    }

    double count_errors = 0;
    for (const auto& error : errors) {
        if (error.lo == uint64_t{0} && error.hi == uint64_t{0}) {
            count_errors += 1;
        }
    }

    std::cout << "Total Errors: " << count_errors << " / " << N << std::endl;
    double percentage = (1.0 - static_cast<double>(count_errors) / N)  * 100.0;
    std::cout << "Success Probability: " << percentage << "%" << std::endl;

    for (size_t i = 0; i < number_of_tables; ++i) {
        delete[] array_mega_X_0[i];
        delete[] array_mega_X_l[i];
    }
}