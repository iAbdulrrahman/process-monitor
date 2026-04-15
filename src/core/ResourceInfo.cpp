#include <iostream>
#include <string>
#include <format>
#include <ranges>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <time.h>
#include "ResourceInfo.h"
#include "utils.h"


ResourceInfo::ResourceInfo(std::string processID)
    : pID {processID}
{
    
};

void ResourceInfo::fetchInfo() {
    // TODO: Parsing /proc/{pid}/ and assigning each stat to its variable.
    this->fetchCPUStats();
    this->fetchMemStats();
    this->fetchIOStats();
}

double ResourceInfo::getCPURate() {
    return this->cpuUsage.cpuUtilizationRate;
}

std::string ResourceInfo::getMemSize() {
    return format_size_from_kb(this->memSize);
}

void ResourceInfo::fetchCPUStats() {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    std::string path = std::format(PROC_CPU_STATS_PATH, this->pID);
    std::string output = open_file(path);

    // /proc/<pid>/stat has format:
    // 1 pid, 2 (comm, may contain spaces), 3 state, ...
    // We must parse around the final ')' of comm before splitting the tail.
    std::size_t leftParen = output.find('(');
    std::size_t rightParen = output.rfind(") ");

    if (rightParen == std::string::npos) {
        rightParen = output.rfind(')');
    }

    if (leftParen == std::string::npos || rightParen == std::string::npos || rightParen <= leftParen) {
        throw std::runtime_error("Failed to parse /proc stat for pid " + this->pID);
    }

    std::size_t tailStart = rightParen + 1;
    if (tailStart < output.size() && output[tailStart] == ' ') {
        tailStart++;
    }

    std::istringstream tailStream(output.substr(tailStart));
    std::vector<std::string> tailStats;
    std::string token;

    while (tailStream >> token) {
        tailStats.push_back(token);
    }

    // tailStats[0] = state (field 3), so:
    // field 14 (utime) -> tailStats[11]
    // field 15 (stime) -> tailStats[12]
    if (tailStats.size() <= 12) {
        throw std::runtime_error("/proc stat has insufficient fields for pid " + this->pID);
    }

    long utime = std::stol(tailStats.at(11));
    long stime = std::stol(tailStats.at(12));

    if(this->cpuUsage.previousUtime == 0 && this->cpuUsage.previousStime == 0) {
        // First sample, no calculation can be done.
        this->cpuUsage.previousUtime = utime;
        this->cpuUsage.previousStime = stime;
        this->cpuUsage.previousTime = currentTime;
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

    this->memSize = std::stod(statm.at(5));

}

void ResourceInfo::fetchIOStats() {
    
}