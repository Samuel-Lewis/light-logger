# HEADER ONLY LOGGER

Light weight logger to be used anywhere in any C++ project.

## SETUP

In any file you plan on logging, make sure to
```
#include "Logger.h"
```

To control the logging level, you need to `#define` what level to log at. Levels available:
```
LOG_FATAL
LOG_ERROR
LOG_WARN
LOG_INFO
LOG_DEBUG
LOG_VERBOSE
```

Any level will also log all levels above it. You can either use
```
#define LOG_INFO
```
Or include in your compilation  
```
-DLOG_INFO
```

If your system does not support ANSI colour codes, you can disable these, by also defining
```
LOG_NO_ANSI
```

## USAGE

Where ever you want to log, use the level name and pipe in your stuff:
```
INFO() << "words words " << 452 << 'a' << getSomeValue();
FATAL() << "could not compute things";
```

Any data type you want to print must be compatible with `std::ostream`.

If you ever invoke `FATAL()`, the process will exit with status `EXIT_FAILURE`.

If you want to track scope of a function, first line of the function, call
```
METHOD();
```
This will indent the logs, and then finish when the function goes out of scope. Warning, this can get expensive.

You are still able to give some input:
```
METHOD() << nameOfDog();
```
