#include <iostream>
#include "ResourceInfo.h"

class Process {
    public:
        Process(std::string processID);
        std::string getID();
        std::string getName();
        std::string getOwner();
        // TODO: Methods to get stats of process, or maybe make ResourceInfo public?
    private:
        std::string pID;
        std::string name;
        std::string owner;
        void fetchName();
        void fetchOwner();
        ResourceInfo resourceInfo;
};