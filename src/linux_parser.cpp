#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

// requires a higher version of g++
# if __has_include(<filesystem>)
    #include <filesystem>  
    namespace fs = std::filesystem;  
#else
    // works for virtual machine version ==> requires target_link_libraries(... stdc++fs) in 13:CMakeLists.txt
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
# endif


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
/*
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
*/

// get the pids of all the processes using filesystem
vector<int> LinuxParser::Pids(){
  vector<int> pids;
  if (std::filesystem::exists(kProcDirectory)) {
    for (const auto& entry : std::filesystem::directory_iterator(kProcDirectory)) {
      std::string pid = entry.path().filename().string();
      if (std::all_of(pid.begin(), pid.end(), isdigit)) {
        pids.emplace_back(stoi(pid));
      }
    }
  }
   return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float total = 1;
  float free = 1;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream streamline(line);
      streamline >> key;
      if (key == "MemTotal:") {
        streamline >> total;
      } else if (key == "MemFree:") {
        streamline >> free;
        break;
      }
    }
  }
  return (total - free) / total;
};

// Read and return the system uptime
long LinuxParser::UpTime() {
  std::string timestr;
  std::string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream streamline(line);
    streamline >> timestr;
  }
  uptime = std::stol(timestr);
  return uptime;
};

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return (ActiveJiffies() + IdleJiffies()); }

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  long out;
  string line;
  string value;
  vector<string> valuevec;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream streamline(line);
    while (streamline >> value) {
      valuevec.emplace_back(value);
    }
  }
  //reference: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  long utime{0}, stime{0}, cutime{0}, cstime{0};
  if (valuevec.size() >= 22) {
    utime = std::stol(valuevec[13]);
    stime = std::stol(valuevec[14]);
    cutime = std::stol(valuevec[15]);
    cstime = std::stol(valuevec[16]);
  }
  out = utime + stime + cutime + cstime;
  return out / sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> vec = LinuxParser::CpuUtilization();
  long out = std::stol(vec[0]) + std::stol(vec[1]) + std::stol(vec[2]) +
             std::stol(vec[5]) + std::stol(vec[6]) + std::stol(vec[7]);
  return out;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> vec = LinuxParser::CpuUtilization();
  return std::stol(vec[3]);
};

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> out;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // only first line
    std::getline(stream, line);
    std::istringstream streamline(line);
    streamline >> key;
    while (streamline >> value) {
      out.emplace_back(value);
    }
  }
  return out;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> value;
        return value;
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> value;
        return value;
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    string command;
    std::ifstream stream(kProcDirectory +to_string(pid)+ kCmdlineFilename);
    if (stream.is_open()) {
      std::getline(stream, command);
    }

  return command; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid ) { 
    string ram;
    long ramsize;
    string line;
    string key;
    std::ifstream stream(kProcDirectory +to_string(pid)+ kStatusFilename);
    if (stream.is_open())
    {
      
      while (std::getline(stream,line))
      {
        std::istringstream streamline(line);
        streamline>>key;
        if (key =="VmRSS:")
        {
          streamline>>ram;
          ramsize = std::stol(ram)/1000; 
          break;
        }
      }
    }
  return std::to_string(ramsize);
  }

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid ) { 
    string uid;
    string line;
    string key;
    std::ifstream stream(kProcDirectory +to_string(pid)+ kStatusFilename);
    if (stream.is_open())
    {
      while (std::getline(stream,line))
      {
        std::istringstream streamline(line);
        streamline>>key;
        if (key =="Uid:")
        {
          streamline>>uid;
          break;
        }
      }
    }
  return uid; }

// Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid ) { 
  string uid = Uid(pid);
  string id, x, temp, line;
  string name = "DEFAULT";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream streamline(line);

      streamline >> temp >> x >> id;
      if (id == uid) {
        name = temp;
        break;
      }
    }
  }
  return name; }

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string value;
  vector<string> valuevec;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream streamline(line);
    while (streamline >> value) {
      valuevec.emplace_back(value);
    }
  }
  //reference: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  long starttime{0};
  if (valuevec.size() >= 22) {
    starttime = std::stol(valuevec[21]);
  }
  return starttime / sysconf(_SC_CLK_TCK); }
