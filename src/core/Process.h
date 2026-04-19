#pragma once

#include <iostream>
#include "ResourceInfo.h"

constexpr std::string PROC_NAME_PATH = "/proc/{}/comm";

class Process {
    public:
        Process(std::string processID);
        std::string getID() const;
        std::string getName() const;
        std::string getOwner() const;
        std::string getStatus() const;
        ResourceInfo resourceInfo;
        // TODO: Methods to get stats of process, or maybe make ResourceInfo public?
    private:
        std::string pID;
        std::string name;
        std::string owner;
        std::string status = "Unknown";
        void fetchName();
        void fetchOwner();
        void fetchStatus();
};