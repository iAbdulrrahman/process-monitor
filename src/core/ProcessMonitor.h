#include <iostream>
#include <vector>
#include "Process.h"

class ProcessMonitor {
    public:
        ProcessMonitor();
        Process getProcess(std::string processID);
    private:
        std::vector<Process> processes = {};
        void fetchProcesses();
        void addProcess(std::string processID);
        void removeProcess(Process process);
};