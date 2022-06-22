/*
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>

#include "../include/linux_parser.h"
using std::stof;
using std::string;
using std::to_string;
using std::vector;

int LinuxParser::RunningProcesses() {
    string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream>>key;
      if (key =="procs_running")
      {
        linestream>>value;
        return value;
      }
    }
  }
   return value; }

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> out;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream streamline(line);
    streamline>>key;
    while (streamline>>value)
    {
      out.push_back(value);
    }
    
  }
  
  
  
  
return out; }



int main(){
  //LinuxParser::Kernel();
  //LinuxParser::MemoryUtilization();
  //std::cout<<LinuxParser::UpTime()<<"\n";
  //std::cout<<LinuxParser::TotalProcesses()<<"\n";
  //std::cout<<LinuxParser::RunningProcesses()<<"\n";
  vector<string> vec = LinuxParser::CpuUtilization();
  //std::cout<<vec[3]<<"\n";
  for (string i:vec)
  {
   std::cout<<i<<"\n";
  }

  vector<int> pids{5,7,9,10};
  for (int i:pids)
  {
    std::cout<<i<<"\n";
  };
};



#include "processor.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float utilization;
    float totaldelta,idledelta;
    //get current jiffies
    long totaljiff = LinuxParser::Jiffies();
    long idlejiff = LinuxParser::IdleJiffies();
    //update last jiffies
    Update(totaljiff,idlejiff);

    long last_totaljiff = Last_TotalJiffies();
    long last_idlejiff = Last_IdleJiffies();

    totaldelta = totaljiff - last_totaljiff;
    idledelta = idlejiff - last_idlejiff;
    utilization = (totaldelta-idledelta)/totaldelta;
    return utilization; }


long Processor::Last_IdleJiffies() const {
    return last_idlejiff;
};
long Processor::Last_TotalJiffies() const {
    return last_totaljiff;
};
void Processor::Update(long totaljiff, long idlejiff) {
    last_totaljiff = totaljiff;
    last_idlejiff = idlejiff;
};
*/
