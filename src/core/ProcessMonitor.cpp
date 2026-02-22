#include <iostream>
#include <vector>
#include "ProcessMonitor.h"
#include "Process.h"

ProcessMonitor::ProcessMonitor() {
    this->fetchProcesses();
}

void ProcessMonitor::fetchProcesses() {
    // TODO: Parsing /proc/ and get list of PIDs and add it using this->addProcess(pid)
}

void ProcessMonitor::addProcess(std::string pID) {
    processes.push_back(Process(pID));
}

void ProcessMonitor::removeProcess(Process process) {
    for(int i = 0; i < processes.size(); i++) {
        if(processes[i].getID() == process.getID()) {
            processes.erase(processes.begin() + i);
        }
    }
}