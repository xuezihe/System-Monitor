#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hour,minute;
    hour = seconds /3600;
    seconds = seconds %3600;
    minute = seconds/60;
    seconds = seconds %60;


     return to_string(hour) +":"+to_string(minute)+":"+to_string(seconds); }