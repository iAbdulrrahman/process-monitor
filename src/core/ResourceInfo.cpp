#include <iostream>
#include <string>
#include <format>
#include <ranges>
#include <chrono>
#include <time.h>
#include "ResourceInfo.h"
#include "utils.h"


ResourceInfo::ResourceInfo(std::string processID)
    : pID {processID}
{
    this->cpuUsage.previousTime = std::chrono::steady_clock::now();
};

void ResourceInfo::fetchInfo() {
    // TODO: Parsing /proc/{pid}/ and assigning each stat to its variable.
    this->fetchCPUStats();
    this->fetchMemStats();
    this->fetchIOStats();
}

void ResourceInfo::fetchCPUStats() {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    std::string path = std::format(PROC_CPU_STATS_PATH, this->pID);
    std::string output = open_file(path);
    std::vector<std::string> stats = split_string(output, " ");

    long utime = std::stoi(stats.at(13));
    long stime = std::stoi(stats.at(14));

    if(this->cpuUsage.previousUtime == 0 && this->cpuUsage.previousStime == 0) {
        // First sample, no calculation can be done.
        this->cpuUsage.previousUtime = utime;
        this->cpuUsage.previousStime = stime;
        this->cpuUsage.cpuUtilizationRate = 0.0;
    } else {
        long deltaCPUTicks = (utime + stime) - (this->cpuUsage.previousUtime + this->cpuUsage.previousStime);
        double elapsedSeconds = std::chrono::duration<double>(currentTime - this->cpuUsage.previousTime).count();

        long numberOfCPUs = sysconf(_SC_NPROCESSORS_ONLN);

        double totalTicks = elapsedSeconds * HZ * numberOfCPUs;

        this->cpuUsage.cpuUtilizationRate = (totalTicks > 0.0)
        ? (deltaCPUTicks / totalTicks) * 100.0
        : 0.0;

        this->cpuUsage.previousUtime = utime;
        this->cpuUsage.previousStime = stime;
        this->cpuUsage.previousTime = currentTime;
    }
}

void ResourceInfo::fetchMemStats() {
    std::string path = std::format(PROC_MEM_STATS_PATH, this->pID);
    std::string output = open_file(path);
    std::vector<std::string> statm = split_string(output, " ");

    this->memSize = std::stod(statm.at(0));

}

void ResourceInfo::fetchIOStats() {
    
}