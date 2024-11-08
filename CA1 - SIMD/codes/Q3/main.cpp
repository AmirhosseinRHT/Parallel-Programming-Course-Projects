#include <immintrin.h>
#include <iostream>
#include <vector>
#include <algorithm>

// Function to count max consecutive zeros in each byte using AVX2
std::vector<int> count_consecutive_zeros_simd(const std::vector<uint8_t>& bytes) {
    size_t len = bytes.size();
    std::vector<int> max_zero_counts(len, 0);
    
    const __m256i ones = _mm256_set1_epi8(1);
    const __m256i zeros = _mm256_setzero_si256();

    for (size_t i = 0; i < len; i += 32) {
        // Load 32 bytes into an AVX2 register
        __m256i data = _mm256_loadu_si256((__m256i*)&bytes[i]);
        
        // Initialize zero count vector for current chunk
        __m256i max_counts = zeros;
        __m256i current_counts = zeros;

        // Loop to detect consecutive zero bits across 8 shifts
        for (int shift = 0; shift < 8; ++shift) {
            // Shift data right by one bit and mask with 1 to isolate current bit position
            __m256i shifted = _mm256_srli_epi16(data, shift);
            __m256i is_zero = _mm256_cmpeq_epi8(_mm256_and_si256(shifted, ones), zeros);

            // Increment current_counts where is_zero, otherwise reset to zero
            current_counts = _mm256_add_epi8(current_counts, is_zero);
            current_counts = _mm256_and_si256(current_counts, is_zero);

            // Update max_counts with the maximum found so far
            max_counts = _mm256_max_epu8(max_counts, current_counts);
        }

        // Store results back to array
        _mm256_storeu_si256((__m256i*)&max_zero_counts[i], max_counts);
    }

    // Convert packed results (max zero counts per byte) from bytes to integers
    return std::vector<int>(max_zero_counts.begin(), max_zero_counts.begin() + len);
}

int main() {
    // Example input bytes
    std::vector<uint8_t> bitstring = { 0b00100001, 0b00000011, 0b11111111, 0b00010000 };
    
    std::vector<int> zero_sequences = count_consecutive_zeros_simd(bitstring);
    
    std::cout << "Max consecutive zero counts per byte: ";
    for (int count : zero_sequences) {
        std::cout << count << " ";
    }
    std::cout << std::endl;

    return 0;
}
