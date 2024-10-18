#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include <stdlib.h>

void getCPUFrequencyInfo(int* baseFrequency, int* maxFrequency) {
    int cpuInfo[4];
    __cpuidex(cpuInfo, 0x16, 0);
    unsigned int base_freq;
    base_freq = cpuInfo[0] & 0xFFFF;
    if (base_freq == 0) {
        printf("Base frequency not available\n");
    } else {
        printf("CPU Base Frequency: %u MHz\n", base_freq);
    }
}

void get_processor_info() {
    int cpu_info[4];
    char processor_name[49];
    memset(processor_name, 0, sizeof(processor_name));
    
    __cpuid(cpu_info, 0x80000002);
    memcpy(processor_name, cpu_info, sizeof(cpu_info));
    __cpuid(cpu_info, 0x80000003);
    memcpy(processor_name + 16, cpu_info, sizeof(cpu_info));
    __cpuid(cpu_info, 0x80000004);
    memcpy(processor_name + 32, cpu_info, sizeof(cpu_info));
    printf("Processor Name: %s\n", processor_name);
    __cpuid(cpu_info, 1);

    int logical_cores = (cpu_info[1] >> 16) & 0xFF;
    printf("Logical Core Count: %d\n", logical_cores);
    __cpuidex(cpu_info, 4, 0);
    
    int physical_cores = ((cpu_info[0] >> 26) & 0x3F) + 1;
    printf("Physical Core Count: %d\n", physical_cores);



    int ht_supported = cpu_info[3] & (1 << 28);
    printf("Hyperthreading Supported: %s\n", ht_supported ? "Yes" : "No");

    __cpuidex(cpu_info, 7, 0);
    int tbm_supported = cpu_info[2] & (1 << 21);
    printf("Turbo Boost Max 3.0 Supported: %s\n", tbm_supported ? "Yes" : "No");

    printf("SIMD Support: ");
    if (cpu_info[3] & (1 << 25)) printf("SSE ");
    if (cpu_info[3] & (1 << 26)) printf("SSE2 ");
    __cpuid(cpu_info, 1);
    if (cpu_info[2] & 0x1) printf("SSE3 ");
    printf("\n");

    int baseFrequency, maxFrequency;
    getCPUFrequencyInfo(&baseFrequency, &maxFrequency);
    // printf("Base Frequency: %d MHz\n", baseFrequency);
    // if (maxFrequency > 0) {
    //     printf("Max Frequency: %d MHz\n", maxFrequency);
    // } else {
    //     printf("Max Frequency: Not available\n");
    // }
}

int main() {
    get_processor_info();
    return 0;
}