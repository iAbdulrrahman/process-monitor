#include <iostream>
#include "ResourceInfo.h"

ResourceInfo::ResourceInfo(std::string processID)
    : pID {processID}
{};

void ResourceInfo::fetchInfo() {
    // TODO: Parsing /proc/{pid}/ and assigning each stat to its variable.
}