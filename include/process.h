#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

// Struct to store process details
struct Process {
    int pid;
    std::string name;
    double cpu_usage;  // CPU usage in percentage
    long memory;       // Memory usage in MB
};

// Function prototypes
std::vector<Process> getProcesses(const std::string& browser_name);
void terminateProcess(int pid);

#endif // PROCESS_H

