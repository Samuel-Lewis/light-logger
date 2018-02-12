# Light Logger
A light-weight, header only C++ logger to be used anywhere in any project.

Author: Samuel Lewis  
Updated: 12th Feb 2018  
Version: 2.0  

github.com/Samuel-Lewis/LightLogger

## Setup
### TL;DR
In any file you want to log within, include
```
#include "light_logger.h"
```
and you're done! All the below setup is optional.

### Logging Levels
To control the logging level, you need to define `LOG_LEVEL` to some level you want to log at. Levels available are:
```
FATAL
ERROR
WARNING
INFO
DEBUG
```
If no level is set, the default is `INFO`. Extra levels can be added or removed by editing the `kLogLevel` struct within this file.

The recommended way to set the level is during compile time, by including as a flag. Eg:
```
-DLOG_LEVEL=INFO
```
Alternatively, you could define it in the code base before you include `light_logger.h` for the first time. Eg:
```
#define LOG_LEVEL WARNING
#include "light_logger.h"
```

### Fatal Program Exit
By default, if you ever log a `FATAL`, the program will log the message and then exit with return `EXIT_FAILURE`. You can change the level to exit at by defining `LOG_DIE_LEVEL`.
```
-DLOG_DIE_LEVEL=ERROR
```

### Timestamp Format
Each log message is prefaced by a timestamp of when the message was called. To change the format, you can specify `LOG_TIME_FORMAT` to be some format string. The rules are as follows here http://www.cplusplus.com/reference/iomanip/put_time/. By default, the format is `"%T"`, (14:55:02)
```
-DLOG_TIME_FORMAT="%D %R"
```

## ANSI Colour Codes
If your system does not support ANSI escape codes, or you want to log to plain text, make sure to define the flag `LOG_DISABLE_ANSI` at compile time.

## Usage
### TL;DR
Log something to a logging level:
```
LOG(INFO) << "Your message here " << FooBar();
```
Quick sanity check some expression:
```
CHECK_THAT(!my_string.empty());
```
Print out some variable and its value:
```
LOOK(my_string);
```

### Logging
When you want to print out a line to your logs, use the `LOG(level)` command, where `level` is one of the listed levels above. This acts as a stream that you can then send things to. Eg:
```
LOG(INFO) << "Your message here " << FooBar();
```

All logs will be flushed to `stderr` as soon as the call log is completed. Anything you send to the stream must be compatible with standard C++ streams.

### CHECK_THAT
`CHECK_THAT(expression)` is much like an `ASSERT`, and will cause an `ERROR` message should the expression evaluate to false. If it evaluates to true, no message will be logged.
```
CHECK_THAT(a != b);
```

### LOOK
`LOOK(my_var)` is a quick utility that pretty much aliases printing out the variable name and then the variable value. This will only accept variables, no literals. Use as such:
```
LOOK(my_name);
```