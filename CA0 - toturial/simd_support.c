// InstructionSet.c
// Compile by using: cl /EHsc /W4 InstructionSet.c
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about
// CPU extended instruction set support.

#include <stdio.h>
#include <string.h>
#include <intrin.h>

#define CHECK_BIT(var,pos) (((var) & (1 << (pos))) != 0)

struct InstructionSet_Internal {
    int nIds_;
    int nExIds_;
    int isIntel_;
    int isAMD_;
    unsigned int f_1_ECX_;
    unsigned int f_1_EDX_;
    unsigned int f_7_EBX_;
    unsigned int f_7_ECX_;
    unsigned int f_81_ECX_;
    unsigned int f_81_EDX_;
    char vendor_[0x20];
    char brand_[0x40];
    int data_[100][4];
    int extdata_[100][4];
};

struct InstructionSet_Internal CPU_Rep;

void InstructionSet_Internal_init(struct InstructionSet_Internal* cpu_rep) {
    int cpui[4];
    int i;
    memset(cpu_rep, 0, sizeof(*cpu_rep));

    __cpuid(cpui, 0);
    cpu_rep->nIds_ = cpui[0];

    for (i = 0; i <= cpu_rep->nIds_ && i < 100; ++i) {
        __cpuidex(cpui, i, 0);
        cpu_rep->data_[i][0] = cpui[0];
        cpu_rep->data_[i][1] = cpui[1];
        cpu_rep->data_[i][2] = cpui[2];
        cpu_rep->data_[i][3] = cpui[3];
    }

    // Capture vendor string
    memset(cpu_rep->vendor_, 0, sizeof(cpu_rep->vendor_));
    *((int*)(cpu_rep->vendor_)) = cpu_rep->data_[0][1];
    *((int*)(cpu_rep->vendor_ + 4)) = cpu_rep->data_[0][3];
    *((int*)(cpu_rep->vendor_ + 8)) = cpu_rep->data_[0][2];
    if (strcmp(cpu_rep->vendor_, "GenuineIntel") == 0) {
        cpu_rep->isIntel_ = 1;
    } else if (strcmp(cpu_rep->vendor_, "AuthenticAMD") == 0) {
        cpu_rep->isAMD_ = 1;
    }

    // Load flags for function 0x00000001
    if (cpu_rep->nIds_ >= 1) {
        cpu_rep->f_1_ECX_ = cpu_rep->data_[1][2];
        cpu_rep->f_1_EDX_ = cpu_rep->data_[1][3];
    }

    // Load flags for function 0x00000007
    if (cpu_rep->nIds_ >= 7) {
        cpu_rep->f_7_EBX_ = cpu_rep->data_[7][1];
        cpu_rep->f_7_ECX_ = cpu_rep->data_[7][2];
    }

    __cpuid(cpui, 0x80000000);
    cpu_rep->nExIds_ = cpui[0];

    memset(cpu_rep->brand_, 0, sizeof(cpu_rep->brand_));

    for (i = 0x80000000; i <= cpu_rep->nExIds_ && (i - 0x80000000) < 100; ++i) {
        __cpuidex(cpui, i, 0);
        int idx = i - 0x80000000;
        cpu_rep->extdata_[idx][0] = cpui[0];
        cpu_rep->extdata_[idx][1] = cpui[1];
        cpu_rep->extdata_[idx][2] = cpui[2];
        cpu_rep->extdata_[idx][3] = cpui[3];
    }

    // Load flags for function 0x80000001
    if (cpu_rep->nExIds_ >= 0x80000001) {
        int idx = 1; // idx = 0x80000001 - 0x80000000 = 1
        cpu_rep->f_81_ECX_ = cpu_rep->extdata_[idx][2];
        cpu_rep->f_81_EDX_ = cpu_rep->extdata_[idx][3];
    }

    // Interpret CPU brand string if reported
    if (cpu_rep->nExIds_ >= 0x80000004) {
        int idx2 = 2; // idx = 0x80000002 - 0x80000000 = 2
        int idx3 = 3; // idx = 0x80000003 - 0x80000000 = 3
        int idx4 = 4; // idx = 0x80000004 - 0x80000000 = 4
        memcpy(cpu_rep->brand_, &cpu_rep->extdata_[idx2][0], sizeof(cpui));
        memcpy(cpu_rep->brand_ + 16, &cpu_rep->extdata_[idx3][0], sizeof(cpui));
        memcpy(cpu_rep->brand_ + 32, &cpu_rep->extdata_[idx4][0], sizeof(cpui));
    }
}

const char* InstructionSet_Vendor(void) {
    return CPU_Rep.vendor_;
}

const char* InstructionSet_Brand(void) {
    return CPU_Rep.brand_;
}

int InstructionSet_SSE3(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 0); }
int InstructionSet_PCLMULQDQ(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 1); }
int InstructionSet_MONITOR(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 3); }
int InstructionSet_SSSE3(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 9); }
int InstructionSet_FMA(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 12); }
int InstructionSet_CMPXCHG16B(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 13); }
int InstructionSet_SSE41(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 19); }
int InstructionSet_SSE42(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 20); }
int InstructionSet_MOVBE(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 22); }
int InstructionSet_POPCNT(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 23); }
int InstructionSet_AES(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 25); }
int InstructionSet_XSAVE(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 26); }
int InstructionSet_OSXSAVE(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 27); }
int InstructionSet_AVX(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 28); }
int InstructionSet_F16C(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 29); }
int InstructionSet_RDRAND(void) { return CHECK_BIT(CPU_Rep.f_1_ECX_, 30); }

int InstructionSet_MSR(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 5); }
int InstructionSet_CX8(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 8); }
int InstructionSet_SEP(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 11); }
int InstructionSet_CMOV(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 15); }
int InstructionSet_CLFSH(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 19); }
int InstructionSet_MMX(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 23); }
int InstructionSet_FXSR(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 24); }
int InstructionSet_SSE(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 25); }
int InstructionSet_SSE2(void) { return CHECK_BIT(CPU_Rep.f_1_EDX_, 26); }

int InstructionSet_FSGSBASE(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 0); }
int InstructionSet_BMI1(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 3); }
int InstructionSet_HLE(void) { return CPU_Rep.isIntel_ && CHECK_BIT(CPU_Rep.f_7_EBX_, 4); }
int InstructionSet_AVX2(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 5); }
int InstructionSet_BMI2(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 8); }
int InstructionSet_ERMS(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 9); }
int InstructionSet_INVPCID(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 10); }
int InstructionSet_RTM(void) { return CPU_Rep.isIntel_ && CHECK_BIT(CPU_Rep.f_7_EBX_, 11); }
int InstructionSet_AVX512F(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 16); }
int InstructionSet_RDSEED(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 18); }
int InstructionSet_ADX(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 19); }
int InstructionSet_AVX512PF(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 26); }
int InstructionSet_AVX512ER(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 27); }
int InstructionSet_AVX512CD(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 28); }
int InstructionSet_SHA(void) { return CHECK_BIT(CPU_Rep.f_7_EBX_, 29); }

int InstructionSet_PREFETCHWT1(void) { return CHECK_BIT(CPU_Rep.f_7_ECX_, 0); }

int InstructionSet_LAHF(void) { return CHECK_BIT(CPU_Rep.f_81_ECX_, 0); }
int InstructionSet_LZCNT(void) { return CPU_Rep.isIntel_ && CHECK_BIT(CPU_Rep.f_81_ECX_, 5); }
int InstructionSet_ABM(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_ECX_, 5); }
int InstructionSet_SSE4a(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_ECX_, 6); }
int InstructionSet_XOP(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_ECX_, 11); }
int InstructionSet_TBM(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_ECX_, 21); }

int InstructionSet_SYSCALL(void) { return CPU_Rep.isIntel_ && CHECK_BIT(CPU_Rep.f_81_EDX_, 11); }
int InstructionSet_MMXEXT(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_EDX_, 22); }
int InstructionSet_RDTSCP(void) { return CPU_Rep.isIntel_ && CHECK_BIT(CPU_Rep.f_81_EDX_, 27); }
int InstructionSet_3DNOWEXT(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_EDX_, 30); }
int InstructionSet_3DNOW(void) { return CPU_Rep.isAMD_ && CHECK_BIT(CPU_Rep.f_81_EDX_, 31); }

void support_message(const char* isa_feature, int is_supported) {
    printf("%s %s\n", isa_feature, is_supported ? "supported" : "not supported");
}

int main(void) {
    InstructionSet_Internal_init(&CPU_Rep);

    printf("%s\n", InstructionSet_Vendor());
    printf("%s\n", InstructionSet_Brand());

    support_message("3DNOW",       InstructionSet_3DNOW());
    support_message("3DNOWEXT",    InstructionSet_3DNOWEXT());
    support_message("ABM",         InstructionSet_ABM());
    support_message("ADX",         InstructionSet_ADX());
    support_message("AES",         InstructionSet_AES());
    support_message("AVX",         InstructionSet_AVX());
    support_message("AVX2",        InstructionSet_AVX2());
    support_message("AVX512CD",    InstructionSet_AVX512CD());
    support_message("AVX512ER",    InstructionSet_AVX512ER());
    support_message("AVX512F",     InstructionSet_AVX512F());
    support_message("AVX512PF",    InstructionSet_AVX512PF());
    support_message("BMI1",        InstructionSet_BMI1());
    support_message("BMI2",        InstructionSet_BMI2());
    support_message("CLFSH",       InstructionSet_CLFSH());
    support_message("CMPXCHG16B",  InstructionSet_CMPXCHG16B());
    support_message("CX8",         InstructionSet_CX8());
    support_message("ERMS",        InstructionSet_ERMS());
    support_message("F16C",        InstructionSet_F16C());
    support_message("FMA",         InstructionSet_FMA());
    support_message("FSGSBASE",    InstructionSet_FSGSBASE());
    support_message("FXSR",        InstructionSet_FXSR());
    support_message("HLE",         InstructionSet_HLE());
    support_message("INVPCID",     InstructionSet_INVPCID());
    support_message("LAHF",        InstructionSet_LAHF());
    support_message("LZCNT",       InstructionSet_LZCNT());
    support_message("MMX",         InstructionSet_MMX());
    support_message("MMXEXT",      InstructionSet_MMXEXT());
    support_message("MONITOR",     InstructionSet_MONITOR());
    support_message("MOVBE",       InstructionSet_MOVBE());
    support_message("MSR",         InstructionSet_MSR());
    support_message("OSXSAVE",     InstructionSet_OSXSAVE());
    support_message("PCLMULQDQ",   InstructionSet_PCLMULQDQ());
    support_message("POPCNT",      InstructionSet_POPCNT());
    support_message("PREFETCHWT1", InstructionSet_PREFETCHWT1());
    support_message("RDRAND",      InstructionSet_RDRAND());
    support_message("RDSEED",      InstructionSet_RDSEED());
    support_message("RDTSCP",      InstructionSet_RDTSCP());
    support_message("RTM",         InstructionSet_RTM());
    support_message("SEP",         InstructionSet_SEP());
    support_message("SHA",         InstructionSet_SHA());
    support_message("SSE",         InstructionSet_SSE());
    support_message("SSE2",        InstructionSet_SSE2());
    support_message("SSE3",        InstructionSet_SSE3());
    support_message("SSE4.1",      InstructionSet_SSE41());
    support_message("SSE4.2",      InstructionSet_SSE42());
    support_message("SSE4a",       InstructionSet_SSE4a());
    support_message("SSSE3",       InstructionSet_SSSE3());
    support_message("SYSCALL",     InstructionSet_SYSCALL());
    support_message("TBM",         InstructionSet_TBM());
    support_message("XOP",         InstructionSet_XOP());
    support_message("XSAVE",       InstructionSet_XSAVE());

    return 0;
}
