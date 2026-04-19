#include <iostream>
#include <format>
#include "Process.h"
#include "ResourceInfo.h"
#include "utils.h"

Process::Process(std::string processID)
    : pID {processID}, resourceInfo {processID}
{
    // Fetch name and owner after initialization
    this->fetchName();
    this->fetchOwner();
    this->fetchStatus();
    this->resourceInfo.fetchInfo();
};

std::string Process::getID() const {
    return this->pID;
};

std::string Process::getName() const {
    return this->name;
};

std::string Process::getOwner() const {
    return this->owner;
};

std::string Process::getStatus() const {
    return this->status;
};

void Process::fetchName() {
    this->name = open_file(std::format(PROC_NAME_PATH, this->pID));
};

void Process::fetchOwner() {
    // Get Owner from /proc/pid/status
    std::string statusPath = std::format("/proc/{}/status", this->pID);
    std::string statusContent = open_file(statusPath);
    std::string ownerLine;
    std::istringstream iss(statusContent);
    while (std::getline(iss, ownerLine)) {
        if (ownerLine.rfind("Uid:", 0) == 0) {
            // Uid:    1000    1000    1000
            std::istringstream uidStream(ownerLine);
            std::string uidLabel;
            int uid;
            uidStream >> uidLabel >> uid;
            // Get username from uid
            std::string passwdContent = open_file("/etc/passwd");
            std::istringstream passwdStream(passwdContent);
            std::string passwdLine;
            while (std::getline(passwdStream, passwdLine)) {
                if (passwdLine.find(std::to_string(uid) + ":") != std::string::npos) {
                    // username:x:uid:gid:comment:home:shell
                    std::istringstream lineStream(passwdLine);
                    std::string username;
                    std::getline(lineStream, username, ':');
                    this->owner = username;
                    return;
                }
            }
        }
    }
};

void Process::fetchStatus() {
    // Get Status from /proc/pid/status
    std::string statusPath = std::format("/proc/{}/status", this->pID);
    std::string statusContent = open_file(statusPath);
    std::string statusLine;
    std::istringstream iss(statusContent);
    while (std::getline(iss, statusLine)) {
        if (statusLine.rfind("State:", 0) == 0) {
            // State:  S (sleeping)
            std::istringstream stateStream(statusLine);
            std::string stateLabel, stateValue;
            stateStream >> stateLabel >> stateValue;
            this->status = stateValue;
        }
    }
};