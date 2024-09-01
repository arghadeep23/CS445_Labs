#include <iostream>
#include <string>
#include <chrono>
#include <openssl/evp.h>
#include <iomanip>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>

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

// Function to compute MD5 hash
std::string md5(const std::string &input)
{
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr)
    {
        throw std::runtime_error("Failed to create MD context");
    }

    const EVP_MD *md = EVP_md5();
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_length = 0;

    if (EVP_DigestInit_ex(context, md, nullptr) != 1 ||
        EVP_DigestUpdate(context, input.c_str(), input.length()) != 1 ||
        EVP_DigestFinal_ex(context, digest, &digest_length) != 1)
    {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to compute MD5 hash");
    }

    EVP_MD_CTX_free(context);

    std::stringstream ss;
    for (unsigned int i = 0; i < digest_length; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    return ss.str();
}

int main()
{
    const int NUM_TESTS = 10;
    const int STRING_LENGTH = 1000; // Input size in characters (bytes)

    std::string input = generate_random_string(STRING_LENGTH);

    size_t input_size_bits = input.length() * 8;
    std::cout << "Input Size: " << input_size_bits << " bits (" << STRING_LENGTH << " characters)\n"
              << std::endl;

    std::cout << "Original String:\n"
              << input << "\n"
              << std::endl;

    // Hashing the string the first time and printing it
    std::string hash = md5(input);
    std::cout << "Hashed String:\n"
              << hash << "\n"
              << std::endl;

    // Warming up the CPU cache
    md5(input);

    std::vector<double> execution_times;

    for (int i = 0; i < NUM_TESTS; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        md5(input);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        execution_times.push_back(duration.count());

        std::cout << "Test " << i + 1 << ": " << duration.count() << " ms" << std::endl;
    }

    double average_time = std::accumulate(execution_times.begin(), execution_times.end(), 0.0) / NUM_TESTS;
    std::cout << "\nAverage execution time: " << average_time << " ms" << std::endl;

    return 0;
}
