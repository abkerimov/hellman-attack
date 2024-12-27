// char itoc(int i) {
//     const std::unordered_map<int, char> intToCharHex = {
//         {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'},
//         {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
//         {8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'},
//         {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'},
//     };
//     return intToCharHex.at(i);
// }


// std::string bitsetToHex(const std::bitset<128> & bs) {
//     std::string hex = "";
//     bool lead_digits = false;
//     for (int i = 127; i >= 0; i -= 4) {
//         char nextSym = itoc((int)((bs[i] << 3) ^ (bs[i - 1] << 2) ^ (bs[i - 2]) << 1 ^ bs[i - 3]));
//         if (nextSym != '0') {
//             lead_digits = true;
//         }
//         if (lead_digits) {
//             hex += itoc((int)((bs[i] << 3) ^ (bs[i - 1] << 2) ^ (bs[i - 2]) << 1 ^ bs[i - 3]));
//         }
//     }
//     return hex;
// }



// std::bitset<128> hexToBitset(const std::string& hex) {
//     const std::unordered_map<char, int> hexToIntMap = {
//         {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
//         {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
//         {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
//         {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15},
//         {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13},
//         {'e', 14}, {'f', 15}
//     };
//     std::bitset<128> bs;
//     if (hex.length() * 4 != 128) {
//         throw std::invalid_argument("Invalid hex string");
//     }
//     for (int i = 0, j = 127; i < hex.length(); i += 1) {
//         int c = hexToIntMap.at(hex[i]);
//         bs[j - 3] = (c) & 0x1;
//         bs[j - 2] = (c >> 1) & 0x1;
//         bs[j - 1] = (c >> 2) & 0x1;
//         bs[j ] = (c >> 3) & 0x1;
//         j -= 4;
//     }
//     return bs;

// }


// std::string hexToBin(const std::string& hex) {
//     const std::unordered_map<char, std::string> hexToBinMap = {
//         {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
//         {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
//         {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
//         {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
//         {'a', "1010"}, {'b', "1011"}, {'c', "1100"}, {'d', "1101"},
//         {'e', "1110"}, {'f', "1111"}
//     };

//     std::string binary;
//     for (char ch : hex) {
//         if (hexToBinMap.count(ch) == 0) {
//             throw std::invalid_argument("Invalid hex character: " + std::string(1, ch));
//         }
//         binary += hexToBinMap.at(ch);
//     }
//     return binary;
// }


// void unsignedCharArrayToBinArray(const unsigned char* array, size_t length, std::string& binString) {
//     binString.clear();
//     for (size_t i = 0; i < length; i++) {
//         for (int bit = 7; bit >= 0; bit--) {
//             binString += ((array[i] >> bit) & 1 ) ? '1' : '0';
//         }
//     }
// }

// std::bitset<32> unsignedCharArrayToBitset(const unsigned char* array, size_t length) {
//     if (length > 4) {
//         throw std::invalid_argument("Array length exceeds the maximum size of 4 bytes for a 32-bit bitset.");
//     }

//     std::bitset<32> bs;
//     for (size_t i = 0; i < length; i++) {
//         for (int bit = 0; bit < 8; bit++) {
//             bs[i * 8 + bit] = (array[i] & (1 << bit)) != 0;
//         }
//     }
//     return bs;
// }


// std::bitset<32> bitStrToBitset(const std::string& bin) {
//     std::bitset<32> res;
//     for (int i = 31, j = 0; i >= 0; --i) {
//         res[j] = (bin[i] - '0');
//         ++j;
//     }
//     return res;
// }


// std::bitset<32> hash_and_truncate(const std::bitset<128>& input) {
//     std::string binary_input;
//     binary_input.reserve(32);
//     for (size_t i = 0; i < 4; i += 8) {
//         unsigned char byte = 0;
//         for (size_t j = 0; j < 8; ++j) {
//             byte |= input[i + j] << (7 - j);
//         }
//         binary_input.push_back(byte);
//     }

//     unsigned char hash[SHA384_DIGEST_LENGTH];
//     SHA384(reinterpret_cast<const unsigned char*>(binary_input.data()), binary_input.size(), hash);


//     uint32_t hash_uint32 = 0;
//     // std::bitset<32> hash_bits;
//     // for (size_t i = 0; i < 4; ++i) {
//     //     for (size_t j = 0; j < 8; ++j) {
//     //         hash_bits[i * 8 + j] = (hash[i] >> (7 - j)) & 1;
//     //     }
//     // }

//     for (size_t i = 0; i < 4; ++i) {
//         for (size_t j = 0; j < 8; ++j) {
//             hash_uint32 += (hash[i] >> (7 - j)) & 1;
//         }
//     }


//     return hash_bits;
// }
//void appendCSV(const std::string& file1, const std::string& file2) {
//     std::ofstream outputFile(file1, std::ios::app);
//     std::ifstream inputFile2(file2);

//     if (!outputFile.is_open() || !inputFile2.is_open()) {
//         std::cerr << "Error opening files!" << std::endl;
//         return;
//     }

//     std::string line;

//     bool isFirstLine = false;
//     while (std::getline(inputFile2, line)) {
//         if (isFirstLine) {
//             isFirstLine = false;
//             continue;
//         }
//         outputFile << line << '\n';
//     }

//     inputFile2.close();
//     outputFile.close();

//     std::cout << "Content from " << file2 << " appended to " << file1 << " successfully!" << std::endl;
// }


// void build_precalc_table_in_csv(size_t a, size_t b, size_t l, int table_number, int thread_number) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<uint64_t> dis(0, UINT32_MAX);

//     std::ofstream table_file("tables/table" + std::to_string(table_number) + std::to_string(thread_number) + ".csv");
//     if (!table_file.is_open()) {
//         throw std::runtime_error("Failed to open output file.");
//     }

//     const size_t buffer_size = 4096 * 32; 
//     std::ostringstream buffer;

//     for (size_t i = a; i < b; ++i) {
//         std::bitset<32> x_0(dis(gen));
//         std::bitset<32> x_i = x_0;
//         for (size_t j = 0; j < l; ++j) {
//             x_i = hash_and_truncate(R(x_i));
//         }
//         buffer << i << "," << x_0.to_string() << "," << x_i.to_string() << "\n";

//         if (buffer.tellp() >= static_cast<std::streampos>(buffer_size)) {
//             table_file << buffer.str();
//             buffer.str(""); 
//             buffer.clear();
//         }
//     }

//     if (!buffer.str().empty()) {
//         table_file << buffer.str();
//     }
//     table_file.close();
// }
// std::bitset<128> R(const std::bitset<32>& x) {
//     size_t n = x.size();
//     size_t r_size = 128 - n;
//     std::bitset<128> r;
//     std::mt19937 rng(std::random_device{}());
//     for (size_t i = 0; i < r_size; ++i) {
//         r[i] = rng() % 2;
//     }
//     std::bitset<128> result = r << n;
//     for (size_t i = 0; i < n; ++i) {
//         if (x[i]) {
//             result.set(i);
//         }
//     }

//     return result;
// }

// void build_precalc_table_multithread(size_t k, size_t l, int table_number) {

//     size_t a1, b1, a2, b2, a3, b3;
//     size_t a4, b4, a5, b5, a6, b6;
//     size_t a7, b7, a8, b8;
//     int range = k / 8;
//     a1 = 0;
//     b1 = a1 + range;
//     a2 = b1 + 1;
//     b2 = a2 + range;
//     a3 = b2 + 1;
//     b3 = a3 + range;
//     a4 = b3 + 1;
//     b4 = a4 + range;
//     a5 = b4 + 1;
//     b5 = a5 + range;
//     a6 = b5 + 1;
//     b6 = a6 + range;
//     a7 = b6 + 1;
//     b7 = a7 + range;
//     a8 = b7 + 1;
//     b8 = a8 + range - 1;

//     std::thread thread1(build_precalc_table_in_csv, a1, b1, l, table_number, 1);
//     std::thread thread2(build_precalc_table_in_csv, a2, b2, l, table_number, 2);
//     std::thread thread3(build_precalc_table_in_csv, a3, b3, l, table_number, 3);
//     std::thread thread4(build_precalc_table_in_csv, a4, b4, l, table_number, 4);
//     std::thread thread5(build_precalc_table_in_csv, a5, b5, l, table_number, 5);
//     std::thread thread6(build_precalc_table_in_csv, a6, b6, l, table_number, 6);
//     std::thread thread7(build_precalc_table_in_csv, a7, b7, l, table_number, 7);
//     std::thread thread8(build_precalc_table_in_csv, a8, b8, l, table_number, 8);
    
//     thread1.join();
//     thread2.join();
//     thread3.join();
//     thread4.join();
//     thread5.join();
//     thread6.join();
//     thread7.join();
//     thread8.join();

//     std::string filename1 = std::string("tables/") + "table" + std::to_string(table_number) + "1.csv";
//     std::string filename2 = std::string("tables/") + "table" + std::to_string(table_number) + "2.csv";
//     std::string filename3 = std::string("tables/") + "table" + std::to_string(table_number) + "3.csv";
//     std::string filename4 = std::string("tables/") + "table" + std::to_string(table_number) + "4.csv";
//     std::string filename5 = std::string("tables/") + "table" + std::to_string(table_number) + "5.csv";
//     std::string filename6 = std::string("tables/") + "table" + std::to_string(table_number) + "6.csv";
//     std::string filename7 = std::string("tables/") + "table" + std::to_string(table_number) + "7.csv";
//     std::string filename8 = std::string("tables/") + "table" + std::to_string(table_number) + "8.csv";
//     appendCSV(filename1, filename2);
//     appendCSV(filename1, filename3);
//     appendCSV(filename1, filename4);
//     appendCSV(filename1, filename5);
//     appendCSV(filename1, filename6);
//     appendCSV(filename1, filename7);
//     appendCSV(filename1, filename8);
// }