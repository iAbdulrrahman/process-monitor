#pragma once

#include <iostream>
#include <vector>
#include "Process.h"

class ProcessMonitor {
    public:
        ProcessMonitor();
        Process getProcess(std::string processID);
        std::vector<Process> getProcesses();
        void refresh();
    private:
        std::vector<Process> processes = {};
        void fetchProcesses();
        bool isZombie(const std::string& processID) const;
        void addProcess(std::string processID);
        void removeProcess(const std::string& processID);
};