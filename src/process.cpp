#include "process.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <iostream> // For std::cout and std::cerr

std::vector<Process> getProcesses(const std::string& browser_name) {
    std::vector<Process> processes;
    DIR* proc_dir = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(proc_dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            std::string cmdline_path = "/proc/" + std::to_string(pid) + "/cmdline";
            std::string status_path = "/proc/" + std::to_string(pid) + "/status";
            std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";

            std::ifstream cmdline_file(cmdline_path);
            std::ifstream status_file(status_path);
            std::ifstream stat_file(stat_path);

            if (cmdline_file && status_file && stat_file) {
                std::string cmdline;
                std::getline(cmdline_file, cmdline, '\0');

                if (cmdline.find(browser_name) != std::string::npos) {
                    std::string line, name;
                    long memory = 0;
                    double cpu = 0.0;

                    // Parse /proc/[pid]/status for process name and memory usage
                    while (std::getline(status_file, line)) {
                        if (line.find("Name:") == 0) {
                            name = line.substr(6);
                        } else if (line.find("VmRSS:") == 0) {
                            std::istringstream iss(line);
                            std::string key;
                            iss >> key >> memory; // Memory in KB
                            memory /= 1024;      // Convert to MB
                            break;
                        }
                    }

                    // Parse /proc/[pid]/stat for CPU usage
                    std::string stat_line;
                    if (std::getline(stat_file, stat_line)) {
                        std::istringstream iss(stat_line);
                        std::string temp;
                        long utime, stime;
                        for (int i = 1; i <= 13; ++i) iss >> temp; // Skip first 13 fields
                        iss >> utime >> stime;
                        cpu = (utime + stime) / sysconf(_SC_CLK_TCK);
                    }

                    processes.push_back({pid, name, cpu, memory});
                }
            }
        }
    }

    closedir(proc_dir);
    return processes;
}

void terminateProcess(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        std::cout << "Successfully terminated process " << pid << "." << std::endl;
    } else {
        std::cerr << "Failed to terminate process " << pid << "." << std::endl;
    }
}

