#pragma once

#include <iostream>
#include <unistd.h>
#include <chrono>

constexpr std::string PROC_CPU_STATS_PATH = "/proc/{}/stat";
constexpr std::string PROC_MEM_STATS_PATH = "/proc/{}/statm";
constexpr std::string PROC_IO_STATS_PATH = "/proc/{}/io";
const int HZ = sysconf(_SC_CLK_TCK);

struct cpu_usage {
    long previousUtime;
    long previousStime;
    std::chrono::steady_clock::time_point previousTime;
    double cpuUtilizationRate;
};

struct disk_usage
{
    int read_bytes;
    int write_bytes;
    int prev_read_bytes;
    int prev_wrtie_bytes;
};


class ResourceInfo
{
    public:
        ResourceInfo(std::string processID);
        void fetchInfo();
    private:
        std::string pID;
        double memSize;
        cpu_usage cpuUsage;
        double cpuUtilizationRate;
        disk_usage diskUsage;
        double networkUsage;
        void fetchCPUStats();
        void fetchMemStats();
        void fetchIOStats();
};