#include <iostream>
#include <vector>
#include <string>
#include "ProcessMonitor.h"
#include "Process.h"
#include "utils.h"

ProcessMonitor::ProcessMonitor() {
    this->fetchProcesses();
}

void ProcessMonitor::fetchProcesses() {
    // TODO: Parsing /proc/ and get list of PIDs and add it using this->addProcess(pid)
    std::vector<std::string> listOfDirs = list_dir("/proc/");
    for (const std::string &str : listOfDirs) {
        try {
            std::stoi(str);
            this->addProcess(str);
        } catch (std::invalid_argument const& ex) {
            continue;
        }
    }
}

void ProcessMonitor::addProcess(std::string pID) {
    bool isDuplicate = false;
    for(Process &process : processes) {
        if(process.getID() == pID) {
            isDuplicate = true;
        }
    }
    if(!isDuplicate) processes.push_back(Process(pID));
}

void ProcessMonitor::removeProcess(Process process) {
    for(int i = 0; i < processes.size(); i++) {
        if(processes[i].getID() == process.getID()) {
            processes.erase(processes.begin() + i);
        }
    }
}