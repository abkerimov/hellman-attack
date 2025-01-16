#include "uint128_t.hpp"

bool operator==(const uint128_t& lhs, const uint128_t& rhs) {
    return lhs.lo == rhs.lo && lhs.hi == rhs.hi;
}

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

// namespace std {
//     template <>
//     size_t hash<uint128_t>::operator()(const uint128_t& value) const {
//         return hash<uint64_t>()(value.lo) ^ (hash<uint64_t>()(value.hi) << 1);
//     }
// }