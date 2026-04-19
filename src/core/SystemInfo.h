#pragma once

#include <cstdint>

class SystemInfo {
public:
    static long cpuCount();
    static long pageSizeBytes();
    static long clockTicksPerSecond();
    static std::uint64_t totalMemoryBytes();

private:
    static long readSysconfValue(int name, long fallbackValue);
};
