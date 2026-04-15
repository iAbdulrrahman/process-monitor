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

std::string ResourceInfo::getCPURate() const {
    // format to percentage with 2 decimal places
    return std::format("{:.2f}%", this->cpuUsage.cpuUtilizationRate);
}

std::string ResourceInfo::getMemSize() const {
    return format_size_from_bytes(this->memSize);
}

std::string ResourceInfo::getReadSpeed() const {
    return format_size_from_bytes(this->diskUsage.read_speed) + "/s";
}

std::string ResourceInfo::getWriteSpeed() const {
    return format_size_from_bytes(this->diskUsage.write_speed) + "/s";
}

double ResourceInfo::getCPURateValue() const {
    return this->cpuUsage.cpuUtilizationRate;
}

double ResourceInfo::getMemBytes() const {
    return this->memSize;
}

void ResourceInfo::fetchCPUStats() {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    std::string path = std::format(PROC_CPU_STATS_PATH, this->pID);
    std::string output = open_file(path);

    if(output.empty()) {
        return;
    }

    // /proc/<pid>/stat has format:
    // 1 pid, 2 (comm, may contain spaces), 3 state, ...
    // We must parse around the final ')' of comm before splitting the tail.
    std::size_t leftParen = output.find('(');
    std::size_t rightParen = output.rfind(") ");

    if (rightParen == std::string::npos) {
        rightParen = output.rfind(')');
    }

    if (leftParen == std::string::npos || rightParen == std::string::npos || rightParen <= leftParen) {
        return;
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
        return;
    }

    long utime = 0;
    long stime = 0;
    try {
        utime = std::stol(tailStats.at(11));
        stime = std::stol(tailStats.at(12));
    } catch (const std::exception&) {
        return;
    }

    if(this->cpuUsage.previousUtime == -1 && this->cpuUsage.previousStime == -1) {
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

    if(output.empty()) {
        return;
    }

    std::vector<std::string> statm = split_string(output, " ");
    if (statm.size() <= 2) {
        return;
    }

    int pageSize = sysconf(_SC_PAGESIZE);

    long residentPages = 0;
    long sharedPages = 0;
    try {
        residentPages = std::stol(statm.at(1));
        sharedPages = std::stol(statm.at(2));
    } catch (const std::exception&) {
        return;
    }
    long residentSize = residentPages * pageSize;
    long sharedSize = sharedPages * pageSize;

    this->memSize = residentSize - sharedSize;
}

void ResourceInfo::fetchIOStats() {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    std::string path = std::format(PROC_IO_STATS_PATH, this->pID);
    std::string output = open_file(path);
    if(output.empty()) {
        return;
    }

    std::vector<std::string> iostat = split_string(output, "\n");
    if (iostat.size() <= 5) {
        return;
    }

    const std::vector<std::string> readParts = split_string(iostat.at(4), ": ");
    const std::vector<std::string> writeParts = split_string(iostat.at(5), ": ");
    if (readParts.size() <= 1 || writeParts.size() <= 1) {
        return;
    }

    long read_bytes = 0;
    long write_bytes = 0;
    try {
        read_bytes = std::stol(readParts.at(1));
        write_bytes = std::stol(writeParts.at(1));
    } catch (const std::exception&) {
        return;
    }

    if(this->diskUsage.prev_read_bytes == -1 && this->diskUsage.prev_write_bytes == -1) {
        this->diskUsage.prev_read_bytes = read_bytes;
        this->diskUsage.prev_write_bytes = write_bytes;
        this->diskUsage.previousTime = currentTime;
    } else {
        long delta_read_bytes = read_bytes - this->diskUsage.prev_read_bytes;
        long delta_write_bytes = write_bytes - this->diskUsage.prev_write_bytes;
        double elapsedSeconds = std::chrono::duration<double>(currentTime - this->diskUsage.previousTime).count();

        this->diskUsage.read_speed = (elapsedSeconds > 0.0) ? static_cast<long>(delta_read_bytes / elapsedSeconds) : 0;
        this->diskUsage.write_speed = (elapsedSeconds > 0.0) ? static_cast<long>(delta_write_bytes / elapsedSeconds) : 0;
        this->diskUsage.prev_read_bytes = read_bytes;
        this->diskUsage.prev_write_bytes = write_bytes;
        this->diskUsage.previousTime = currentTime;
    }
}