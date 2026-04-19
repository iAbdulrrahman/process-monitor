#include "SystemInfo.h"

#include <limits>
#include <unistd.h>

long SystemInfo::readSysconfValue(int name, long fallbackValue) {
    const long value = sysconf(name);
    return value > 0 ? value : fallbackValue;
}

long SystemInfo::cpuCount() {
    static const long cachedCpuCount = readSysconfValue(_SC_NPROCESSORS_ONLN, 1);
    return cachedCpuCount;
}

long SystemInfo::pageSizeBytes() {
    static const long cachedPageSize = readSysconfValue(_SC_PAGESIZE, 4096);
    return cachedPageSize;
}

long SystemInfo::clockTicksPerSecond() {
    static const long cachedClockTicks = readSysconfValue(_SC_CLK_TCK, 100);
    return cachedClockTicks;
}

std::uint64_t SystemInfo::totalMemoryBytes() {
    static const std::uint64_t cachedTotalMemoryBytes = []() {
        const long pageCount = readSysconfValue(_SC_PHYS_PAGES, 0);
        const long pageSize = pageSizeBytes();

        if (pageCount <= 0 || pageSize <= 0) {
            return static_cast<std::uint64_t>(0);
        }

        const auto max = std::numeric_limits<std::uint64_t>::max();
        const auto pageCountU64 = static_cast<std::uint64_t>(pageCount);
        const auto pageSizeU64 = static_cast<std::uint64_t>(pageSize);

        if (pageCountU64 > max / pageSizeU64) {
            return max;
        }

        return pageCountU64 * pageSizeU64;
    }();

    return cachedTotalMemoryBytes;
}
