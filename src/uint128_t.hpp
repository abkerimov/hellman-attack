#pragma once

#include <cstdint>
#include <iostream>
#include <functional>

struct uint128_t {
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
};

bool operator==(const uint128_t& lhs, const uint128_t& rhs);
uint128_t operator+(const uint128_t& lhs, const uint128_t& rhs);
uint128_t operator>>(const uint128_t& lhs, unsigned long shift);
uint128_t operator<<(const uint128_t& lhs, unsigned long shift);
void print_uint128(const uint128_t& value);

