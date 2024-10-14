#include <iostream>
#include <string>
#include <intrin.h>
#include <bitset>
#include <vector>

void get_processor_info() {
    int cpu_info[4] = {-1};
    char processor_name[49];
    
    __cpuid(cpu_info, 0x80000002);
    memcpy(processor_name, cpu_info, sizeof(cpu_info));
    __cpuid(cpu_info, 0x80000003);
    memcpy(processor_name + 16, cpu_info, sizeof(cpu_info));
    __cpuid(cpu_info, 0x80000004);
    memcpy(processor_name + 32, cpu_info, sizeof(cpu_info));
    std::cout << "Processor Name: " << processor_name << std::endl;

    __cpuid(cpu_info, 1);
    int logical_cores = (cpu_info[1] >> 16) & 0xFF;
    std::cout << "Logical Core Count: " << logical_cores << std::endl;

    bool ht_supported = cpu_info[3] & (1 << 28);
    std::cout << "Hyperthreading Supported: " << (ht_supported ? "Yes" : "No") << std::endl;

    __cpuidex(cpu_info, 7, 0);
    bool tbm_supported = cpu_info[2] & (1 << 21); 
    std::cout << "Turbo Boost Max 3.0 Supported: " << (tbm_supported ? "Yes" : "No") << std::endl;

    std::vector<std::string> simd_support;
    if (cpu_info[3] & (1 << 25)) simd_support.push_back("SSE");
    if (cpu_info[3] & (1 << 26)) simd_support.push_back("SSE2");

    std::cout << "SIMD Support: ";
    for (const auto& simd : simd_support) {
        std::cout << simd << " ";
    }
    std::cout << std::endl;

}

int main() {
    get_processor_info();
    return 0;
}