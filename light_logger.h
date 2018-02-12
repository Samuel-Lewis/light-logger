/*
# Light Logger
  A light-weight, header only C++ logger to be used anywhere in any project.

  Author    Samuel Lewis
  Updated   12th Feb 2018
  Version   2.0

  github.com/Samuel-Lewis/LightLogger

## Setup
  ### TL;DR
  In any file you want to log within, include
    #include "light_logger.h"
  and you're done! All the below setup is optional.

  To control the logging level, you need to define LOG_LEVEL to some level you want to log at. Levels available are:
    FATAL
    ERROR
    WARNING
    INFO
    DEBUG
  If no level is set, the default is INFO. Extra levels can be added or removed by editing the kLogLevel struct within this file.

  The recommended way to set the level is during compile time, by including as a flag. Eg:
    -DLOG_LEVEL=INFO

  Alternatively, you could define it in the code base before you include light_logger.h for the first time. Eg:
    #define LOG_LEVEL WARNING
    #include "light_logger.h"

  ### Fatal Program Exit
  By default, if you ever log a FATAL, the program will log the message and then exit with return EXIT_FAILURE. You can change the level to exit at by defining LOG_DIE_LEVEL.
    -DLOG_DIE_LEVEL=ERROR

  ### Timestamp Format
  Each log message is prefaced by a timestamp of when the message was called. To change the format, you can specify LOG_TIME_FORMAT to be some format string. The rules are as follows here http://www.cplusplus.com/reference/iomanip/put_time/. By default, the format is "%T", (14:55:02)
    -DLOG_TIME_FORMAT="%D %R"

  ## ANSI Colour Codes
  If your system does not support ANSI escape codes, or you want to log to plain text, make sure to define the flag LOG_DISABLE_ANSI at compile time.

## Usage
  ### TL;DR
  Log something to a logging level:
    LOG(INFO) << "Your message here " << FooBar();
  Quick sanity check some expression:
    CHECK_THAT(!my_string.empty());
  Print out some variable and its value:
    LOOK(my_string);

  ### Logging
  When you want to print out a line to your logs, use the LOG(level) command, where level is one of the listed levels above. This acts as a stream that you can then send things to. Eg:
    LOG(INFO) << "Your message here " << FooBar();

  All logs will be flushed to stderr as soon as the call log is completed. Anything you send to the stream must be compatible with standard C++ streams.

  ### CHECK_THAT
  CHECK_THAT(expression) is much like an ASSERT, and will cause an ERROR message should the expression evaluate to false. If it evaluates to true, no message will be logged.
    CHECK_THAT(a != b)

  ### LOOK
  LOOK(my_var) is a quick utility that pretty much aliases to 
    LOG(INFO) << "my_var" << " = " << my_var;
  and is useful for seeing the state of any variables.

*/

#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <ctime>
#include <iomanip>

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif  // LOG_LEVEL

#ifndef LOG_DIE_LEVEL
#define LOG_DIE_LEVEL FATAL
#endif  // LOG_DIE_LEVEL

#ifndef LOG_TIME_FORMAT
#define LOG_TIME_FORMAT "%T"
#endif  // LOG_TIME_FORMAT

#define LOG(level) if (level <= LOG_LEVEL) LightLogger().Get(level, #level, __FILE__, __FUNCTION__, __LINE__)

#define LOOK(var) LOG(INFO) << "LOOK: " << #var << " = " << var
#define CHECK_THAT(expr) if (!(expr)) LOG(ERROR) << "CHECK_THAT(" << #expr << ")"


enum kLogLevel {
  FATAL = 0,
  ERROR,
  WARNING,
  INFO,
  DEBUG
};

class LightLogger {
 public:
  LightLogger() : do_die_(false) {}
  virtual ~LightLogger() {
    std::cerr << output_stream_.str() << std::endl;

    if (do_die_) {
      std::exit(EXIT_FAILURE);
    }
  }

  std::ostringstream& Get(const kLogLevel& level,
                          const std::string& level_str,
                          const std::string& file,
                          const std::string& function,
                          const int& line) {
    output_stream_ << "- ";
    output_stream_ << TimeNow();
    output_stream_ << ToString(level, level_str);
    output_stream_ << " " << file << ":" << line << ": ";
    do_die_ = level <= LOG_DIE_LEVEL;
    return output_stream_;
  }

 private:
  std::ostringstream output_stream_;
  bool do_die_;
  static std::string TimeNow() {
    auto now = std::time(0);
    auto tm = *std::localtime(&now);
    std::stringstream ss;
    ss << "[" << std::put_time(&tm, LOG_TIME_FORMAT) << "]";
    return ss.str();
  }

  static std::string ToString(const kLogLevel& level,
                              const std::string& level_str) {
    std::stringstream ss;
    ss << "[";
    #ifndef LOG_DISABLE_ANSI
    switch (level) {
      case FATAL: ss << "\033[1;41;37m"; break;
      case ERROR: ss << "\033[1;31m"; break;
      case WARNING: ss << "\033[33m"; break;
      default: break;
    }
    #endif  // LOG_DISABLE_ANSI
    ss << level_str;
    #ifndef LOG_DISABLE_ANSI
    ss << "\033[0m";
    #endif
    ss << "]";
    return ss.str();
  }
};
#endif  // LOGGER_H
