#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "../include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
//construct function
Process::Process(int pid){
    pid_ = pid;
    ram_ =std::stol(LinuxParser::Ram(pid));
    uptime_ = LinuxParser::UpTime(pid);

    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);

    cpuutilization_ = LinuxParser::ActiveJiffies(pid)/LinuxParser::Jiffies();


};
// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuutilization_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return std::to_string(ram_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    if (cpuutilization_ < a.CpuUtilization())
    {
        return true;
    }
    else
    {
        return false;
    }
     }