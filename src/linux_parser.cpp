#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

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

    filestream.close();
  }
  return value != "" ? value : "Linux";
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
  return kernel;
}

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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  float mem_total;
  float mem_free;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key) {
        if( key == "MemTotal")
            linestream >> mem_total;
        else if( key == "MemFree")
            linestream >> mem_free;
		else
            break;
      }
    }
    filestream.close();
  }
  return ((mem_total - mem_free)/mem_total);
}

long LinuxParser::UpTime() { 
  string line;
  string key;
  long total_uptime;
  long idle;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> total_uptime >> idle;
    filestream.close();
    return total_uptime;
  }
  return 0;
}

long LinuxParser::Jiffies() {
  string line, value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.push_back(value);
    }
    filestream.close();    
    return stol(tokens[21]);
  }
  return 0;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.push_back(value);
    }
    filestream.close();    
    return stol(tokens[21]);
  }
  return 0;
}

long LinuxParser::ActiveJiffies() {
  string line, value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.push_back(value);
    }
    filestream.close();    
    return stol(tokens[21]);
  }
  return 0;
}

long LinuxParser::IdleJiffies() {
  string line, value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.push_back(value);
    }
    filestream.close();
    return stol(tokens[21]);
  }
  return 0;
}

vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          linestream >> value;
          return value;
        } else {
         break; 
        }
      }
    }
    filestream.close();
  }
  return value;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> value;
          return value;
        } else {
         break; 
        }
      }
    }

    filestream.close();
  }
  return value;
}


string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  filestream.close();
  return line.length() > 50 ? line.substr(0, 50)+"..." : line;
}

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "VmRSS") {
          // Replaced "VmSize" with "VmRSS" because it is a more accurate measure of the amount of RAM used by the process
          linestream >> value;
          return to_string(value/1024);
        } else {
         break; 
        }
      }
    }
    filestream.close();
  }
  return "";
}

string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid") {
          linestream >> value;
          return value;
        } else {
         break; 
        }
      }
    }
    filestream.close();
  }
  return value;
}

string LinuxParser::User(int pid) { 
  string line;
  string value;
  string username;
  string x;
  string uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> uid) {
        if (uid == Uid(pid)) {
          return username;
        } else {
         break; 
        }
      }
    }
    filestream.close();
  }
  return username;
}

float LinuxParser::UniqueCpuUtilization(int pid){
  string line, value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.push_back(value);
    }
    
    float total = (stol(tokens[13])+stol(tokens[14])+stol(tokens[15])+stol(tokens[16]))/sysconf(_SC_CLK_TCK);
    long sec = UpTime(pid);
    float cpu_usage = total / sec;
    filestream.close();
    return cpu_usage;
  }
  return 0;
}

long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    vector<string> tokens{};
    while(linestream >> value) {
      tokens.emplace_back(value);
    }
    filestream.close();
    return UpTime() - stol(tokens[21])/sysconf(_SC_CLK_TCK);
  }
  return 0;
}
