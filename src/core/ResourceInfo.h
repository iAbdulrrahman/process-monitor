#include <iostream>

class ResourceInfo
{
    public:
        ResourceInfo(std::string processID);
        void fetchInfo();
    private:
        std::string pID;
        double memoryUsage;
        double cpuUsage;
        double diskUsage;
        double networkUsage;
};