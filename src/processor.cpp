#include <string>
#include <sstream>
#include <fstream>

using std::istringstream;
using std::ifstream;
using std::string;

#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
  string line;
  string key;
  float iowait, idle, user, nice, system, irq, softirq, steal;
  std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          linestream >> user >> nice >> system >> idle >> iowait;
          linestream >> irq >> softirq >> steal;
        } else {
         break; 
        }
      }
    }
  }
  
  float Idle = idle + iowait;
  float NonIdle = user + nice + system + irq + softirq + steal;
  float Total = Idle + NonIdle;

  return (Total - Idle)/Total;
}