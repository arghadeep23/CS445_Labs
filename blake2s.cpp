#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>
#include <openssl/evp.h>

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

std::string blake2s_hash(const std::string &input)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_blake2s256(), nullptr);
    EVP_DigestUpdate(ctx, input.c_str(), input.size());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int main()
{
    const int NUM_TESTS = 10;
    const int STRING_LENGTH = 1000; // Input size in characters (bytes)

    // Generate random input string
    std::string input = generate_random_string(STRING_LENGTH);

    size_t input_size_bits = input.length() * 8;
    std::cout << "Input Size: " << input_size_bits << " bits (" << STRING_LENGTH << " characters)\n"
              << std::endl;

    std::cout << "Original String:\n"
              << input << "\n"
              << std::endl;

    // Hashing the string the first time and printing it
    std::string hash = blake2s_hash(input);
    std::cout << "Hashed String:\n"
              << hash << "\n"
              << std::endl;
    std::cout << "Output Size: " << hash.length() * 4 << " bits\n"
              << std::endl;

    // Warming up the CPU cache
    blake2s_hash(input);

    std::vector<double> execution_times;

    for (int i = 0; i < NUM_TESTS; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        blake2s_hash(input);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        execution_times.push_back(duration.count());

        std::cout << "Test " << i + 1 << ": " << duration.count() << " ms" << std::endl;
    }

    double average_time = std::accumulate(execution_times.begin(), execution_times.end(), 0.0) / NUM_TESTS;
    std::cout << "\nAverage execution time: " << average_time << " ms" << std::endl;

    return 0;
}
