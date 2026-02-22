#include <iostream>
#include "Process.h"
#include "ResourceInfo.h"

Process::Process(std::string processID)
    : pID {processID}, resourceInfo {processID}
{
    // Fetch name and owner after initialization
    this->fetchName();
    this->fetchOwner();
};

std::string Process::getID() {
    return this->pID;
};

std::string Process::getName() {
    return this->name;
};

std::string Process::getOwner() {
    return this->owner;
};

void Process::fetchName() {
    // TODO: Parsing /proc/{pid} to get the name of the process.
};

void Process::fetchOwner() {
    // TODO: Parsing /proc/{pid} to get the owner of the process.
};
