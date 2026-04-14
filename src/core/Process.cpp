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
    this->name = open_file(std::format(PROC_NAME_PATH, this->pID));
};

void Process::fetchOwner() {
    // TODO: Parsing /proc/{pid} to get the owner of the process.
};
