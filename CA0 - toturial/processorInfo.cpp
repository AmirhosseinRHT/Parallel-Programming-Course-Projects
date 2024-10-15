#include <iostream>
#include <string>
#include <intrin.h>
#include <bitset>
#include <vector>

void getCPUFrequencyInfo(int& baseFrequency, int& maxFrequency) {
    int cpuInfo[4] = {-1};

    __cpuid(cpuInfo, 0x16);
    baseFrequency = cpuInfo[0];

    maxFrequency = 0;
    __cpuid(cpuInfo, 0x80000007);
    std::bitset<32> edxBits(cpuInfo[3]);
    if (edxBits[8]) { 
        __cpuid(cpuInfo, 0x80000004);
        unsigned int eax = cpuInfo[0];

        std::string freqString;
        for (int i = 0; i < 16; ++i) {
            if (((char*)&eax)[i] >= '0' && ((char*)&eax)[i] <= '9') {
                while (i < 16 && ((char*)&eax)[i] != ' ') {
                    freqString += ((char*)&eax)[i++];
                }
                break;
            }
        }

        if (!freqString.empty()) {
            maxFrequency = std::stof(freqString) * 1000;
        }
    }
}

void get_processor_info() {
    int cpu_info[4] = {-1};
    char processor_name[49] = {0};
    
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

    // Add frequency information
    int baseFrequency, maxFrequency;
    getCPUFrequencyInfo(baseFrequency, maxFrequency);
    std::cout << "Base Frequency: " << baseFrequency << " MHz" << std::endl;
    if (maxFrequency > 0) {
        std::cout << "Max Frequency: " << maxFrequency << " MHz" << std::endl;
    } else {
        std::cout << "Max Frequency: Not available" << std::endl;
    }
}

int main() {
    get_processor_info();
    return 0;
}