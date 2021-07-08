#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;
using std::ostringstream;
using std::setw;
using std::setfill;

string Format::ElapsedTime(long seconds) { 
  long hours = seconds/3600;
  long min = (seconds%3600)/60;
  long sec = (seconds%3600)%60;
  
  ostringstream os;
  os << setfill('0');
  os << setw(2) << hours << ":";
  os << setw(2) << min << ":";
  os << setw(2) << sec;
  
  return os.str(); 
}