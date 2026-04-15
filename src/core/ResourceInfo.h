#pragma once

#include <iostream>
#include <unistd.h>
#include <chrono>

constexpr std::string PROC_CPU_STATS_PATH = "/proc/{}/stat";
constexpr std::string PROC_MEM_STATS_PATH = "/proc/{}/statm";
constexpr std::string PROC_IO_STATS_PATH = "/proc/{}/io";
const int HZ = sysconf(_SC_CLK_TCK);

struct cpu_usage {
    long previousUtime = 0;
    long previousStime = 0;
    std::chrono::steady_clock::time_point previousTime;
    double cpuUtilizationRate = 0;
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
        double getCPURate();
        std::string getMemSize();
    private:
        std::string pID;
        double memSize = 0.0;
        cpu_usage cpuUsage;
        disk_usage diskUsage;
        double networkUsage = 0.0;
        void fetchCPUStats();
        void fetchMemStats();
        void fetchIOStats();
};