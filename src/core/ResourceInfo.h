#pragma once

#include <iostream>
#include <unistd.h>
#include <chrono>

constexpr std::string PROC_CPU_STATS_PATH = "/proc/{}/stat";
constexpr std::string PROC_MEM_STATS_PATH = "/proc/{}/statm";
constexpr std::string PROC_IO_STATS_PATH = "/proc/{}/io";
const int HZ = sysconf(_SC_CLK_TCK);

struct cpu_usage {
    long previousUtime = -1;
    long previousStime = -1;
    std::chrono::steady_clock::time_point previousTime;
    double cpuUtilizationRate = 0;
};

struct disk_usage
{
    long prev_read_bytes = -1;
    long prev_write_bytes = -1;
    std::chrono::steady_clock::time_point previousTime;
    long read_speed = 0;
    long write_speed = 0;
};


class ResourceInfo
{
    public:
        ResourceInfo(std::string processID);
        void fetchInfo();
        std::string getCPURate() const;
        std::string getMemSize() const;
        std::string getReadSpeed() const;
        std::string getWriteSpeed() const;
        double getCPURateValue() const;
        double getMemBytes() const;
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