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

Processor& System::Cpu() { 
  return cpu_; 
}

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

std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}
