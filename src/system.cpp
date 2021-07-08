#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::for_each;
using std::sort;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  vector<Process> temp {};
  for_each(pids.begin(), pids.end(), [&](int pid){
    Process process(pid);
    temp.push_back(process);
  });
  sort(temp.begin(), temp.end());
  processes_ = temp;
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}
