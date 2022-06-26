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
    second_ = LinuxParser::UpTime() - uptime_;
    float activejiff = float(LinuxParser::ActiveJiffies(pid));
    float totaluptime = float(LinuxParser::UpTime())*sysconf(_SC_CLK_TCK);
    cpuutilization_ = activejiff/(totaluptime-uptime_*sysconf(_SC_CLK_TCK));

    //cpuutilization_ = float(LinuxParser::ActiveJiffies(pid))/float(second_);
};
// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuutilization_; }

// Return the command that generated this process
string Process::Command() {
    if (command_.size()>40)
    {
        return command_.substr(0,40) +"...";
    }
    else
    {
        return command_;

    }
 }

// Return this process's memory utilization
string Process::Ram() { return std::to_string(ram_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return second_; }

// Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return cpuutilization_<a.CpuUtilization();
     }