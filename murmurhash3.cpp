#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>
#include <cstring>

// MurmurHash3 implementation
uint32_t MurmurHash3_x86_32(const void *key, int len, uint32_t seed)
{
    const uint8_t *data = (const uint8_t *)key;
    const int nblocks = len / 4;
    uint32_t h1 = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    // Body
    const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);
    for (int i = -nblocks; i; i++)
    {
        uint32_t k1 = blocks[i];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = h1 * 5 + 0xe6546b64;
    }

    // Tail
    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);
    uint32_t k1 = 0;
    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        h1 ^= k1;
    };

    // Finalization
    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

std::string generate_random_string(size_t length)
{
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        result += characters[distribution(generator)];
    }
    return result;
}

int main()
{
    const int NUM_TESTS = 10;
    const int STRING_LENGTH = 1000; // Input size in characters (bytes)

    std::string input = generate_random_string(STRING_LENGTH);

    size_t input_size_bits = input.length() * 8;
    std::cout << "Input Size: " << input_size_bits << " bits (" << STRING_LENGTH << " characters)" << std::endl;
    std::cout << "Original String:\n"
              << input << std::endl;

    // Warming up the CPU cache
    uint32_t dummy_hash = MurmurHash3_x86_32(input.c_str(), input.length(), 0);

    std::vector<double> execution_times;
    uint32_t seed = 0;

    for (int i = 0; i < NUM_TESTS; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        uint32_t hash = MurmurHash3_x86_32(input.c_str(), input.length(), seed);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration = end - start;
        execution_times.push_back(duration.count());

        // Output the hash for each test run
        std::cout << "Test " << i + 1 << ":\n"
                  << "MurmurHash3 Hash: 0x" << std::hex << std::setw(8) << std::setfill('0') << hash << std::endl
                  << "Hash Size: 32 bits" << std::endl
                  << "Execution Time: " << duration.count() << " microseconds" << std::endl
                  << std::endl;
    }

    double average_time = std::accumulate(execution_times.begin(), execution_times.end(), 0.0) / NUM_TESTS;
    std::cout << "Average execution time: " << average_time << " microseconds" << std::endl;

    return 0;
}
