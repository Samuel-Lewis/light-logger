
/*
 ===== HEADER ONLY LOGGER =====

	Light weight logger to be used anywhere in any project.

	@Author     Samuel Lewis
	@Updated 	29th April 2017
	@Version	1.1

>> SETUP <<

	In any file you plan on logging, make sure to
		#include "Logger.h"

	To control the logging level, you need to #define what level to log at. Levels available:
		LOG_FATAL
		LOG_ERROR
		LOG_WARN
		LOG_INFO
		LOG_DEBUG
		LOG_VERBOSE

	Any level will also log all levels above it. You can either use
		#define LOG_INFO
	Or include in your compilation
		-DLOG_INFO

	If your system supports ANSI colour codes, you can additionally include this, by including
		LOG_USE_ANSI

>> USAGE <<

	Where ever you want to log, use the level name and pipe in your stuff:
		INFO() << "words words " << 452 << 'a' << getSomeValue();
		FATAL() << "could not compute things ";

	Any data type you want to print must be compatible with std::ostream

	If you ever invoke FATAL(), the process will exit with status EXIT_FAILURE.

*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>

#define _LOG_FORMAT(L,A) Logger().gen(L,A,__FILE__,__LINE__)
#define _LOG_NULLSTREAM _LoggerNullStream.clear(); _LoggerNullStream

#if defined(LOG_FATAL) || defined(LOG_ERROR) || defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define FATAL() _LOG_FORMAT("FATAL","1;41;37m")
#else
	#define FATAL() _LOG_NULLSTREAM
#endif

#if defined(LOG_ERROR) || defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define ERROR() _LOG_FORMAT("ERROR","1;31m")
#else
	#define ERROR() _LOG_NULLSTREAM
#endif

#if defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define WARN() _LOG_FORMAT("WARN ","[1;33m")
#else
	#define WARN() _LOG_NULLSTREAM
#endif

#if defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define INFO() _LOG_FORMAT("INFO ","35m")
#else
	#define INFO() _LOG_NULLSTREAM
#endif

#if defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define DEBUG() _LOG_FORMAT("DEBUG","0m")
#else
	#define DEBUG() _LOG_NULLSTREAM
#endif

#if defined(LOG_VERBOSE)
	#define VERBOSE() _LOG_FORMAT("VERBO","0m")
#else
	#define VERBOSE() _LOG_NULLSTREAM
#endif

std::ostringstream _LoggerNullStream;

class Logger {
public:
	Logger() {};
	~Logger() {
		// Print out, without delay ;)
		std::cerr << oss.str() << std::endl;

		// If FATAL, crash and burn
		if (setLevel == "FATAL") {

			std::exit(EXIT_FAILURE);
		}

		// Clear the null log stream, to hopefully save memory.
		_LoggerNullStream.clear();
	}

	// Generate ostream (with meta), return to inline call position
	std::ostringstream& gen(std::string label, std::string ansi, std::string file, int line) {
		// Format: [20:04:09][FATAL]:main.cpp:6: ...
		setLevel = label;
		oss << "[" << getTime() << "][";

		// Supress usage warnings
		ansi = ansi;

		#ifdef LOG_USE_ANSI
			oss << "\033[" << ansi << label << "\033[0m";
		#else
			oss << label;
		#endif

		oss << "]:" << file << ":" << line << ": ";

		return oss;
	}

private:
	std::ostringstream oss;
	std::string getTime() {
		// C++98 get time as 15:03:12
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[12];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer,sizeof(buffer),"%H:%M:%S",timeinfo);
		std::string str(buffer);
		return str;
	}

	std::string setLevel;

};

#endif // LOGGER_H
