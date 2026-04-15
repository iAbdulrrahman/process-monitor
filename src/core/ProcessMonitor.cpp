#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <filesystem>
#include <algorithm>
#include <format>
#include "ProcessMonitor.h"
#include "Process.h"
#include "utils.h"

ProcessMonitor::ProcessMonitor() {
    this->fetchProcesses();
}

void ProcessMonitor::refresh() {
    this->fetchProcesses();
}

std::vector<Process> ProcessMonitor::getProcesses() {
    return this->processes;
}

void ProcessMonitor::fetchProcesses() {
    std::vector<std::string> listOfDirs = list_dir("/proc/");
    std::unordered_set<std::string> currentPids;

    for (const std::string &entryPath : listOfDirs) {
        std::string pid = std::filesystem::path(entryPath).filename().string();

        try {
            (void)std::stoi(pid);
            currentPids.insert(pid);

            // Track only alive (non-zombie) processes in this list.
            if (!this->isZombie(pid)) {
                this->addProcess(pid);
            } else {
                this->removeProcess(pid);
            }
        } catch (const std::exception&) {
            continue;
        }
    }

    // Remove processes that no longer exist under /proc (terminated and reaped).
    std::vector<std::string> stalePids;
    stalePids.reserve(this->processes.size());

    for (Process &process : this->processes) {
        if (!currentPids.contains(process.getID())) {
            stalePids.push_back(process.getID());
        } else {
            process.resourceInfo.fetchInfo();
        }
    }

    for (const std::string &pid : stalePids) {
        this->removeProcess(pid);
    }
}

bool ProcessMonitor::isZombie(const std::string& processID) const {
    const std::string stat = open_file(std::format("/proc/{}/stat", processID));
    if (stat.empty()) {
        return false;
    }

    // /proc/<pid>/stat format: pid (comm) state ...
    const std::size_t closeParen = stat.rfind(") ");
    if (closeParen == std::string::npos || closeParen + 2 >= stat.size()) {
        return false;
    }

    const char state = stat[closeParen + 2];
    return state == 'Z';
}

void ProcessMonitor::addProcess(std::string pID) {
    bool isDuplicate = false;
    for(Process &process : processes) {
        if(process.getID() == pID) {
            isDuplicate = true;
            break;
        }
    }
    if(!isDuplicate) processes.push_back(Process(pID));
}

void ProcessMonitor::removeProcess(const std::string& processID) {
    std::erase_if(processes, [&](Process& process) {
        return process.getID() == processID;
    });
}