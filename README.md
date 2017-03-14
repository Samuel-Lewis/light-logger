# HEADER ONLY LOGGER

Light weight logger to be used anywhere in any project.
Version 0.1?

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

If your system supports ANSI colour codes, you can additionally include this, by including
```
LOG_USE_ANSI
```

## USAGE

Where ever you want to log, use the level name and pipe in your stuff:
```
INFO() << "words words " << 452 << 'a' << getSomeValue();;
FATAL() << "could not compute things " << getFailCode();
```

Any data type you want to print must be compatible with `std::ostream`.

If you ever invoke `FATAL()`, the process will exit with status `EXIT_FAILURE`.
