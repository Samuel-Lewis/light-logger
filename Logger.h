
/*
 ===== HEADER ONLY LOGGER =====

	Light weight logger to be used anywhere in any project.

	@Author     Samuel Lewis
	@Updated	9th March 2017
	@Version	1.0 ?

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

	Where ever you want to log, use the level name:
		INFO() << "words words " << 452 << 'a' << myVar;
		FATAL() << "could not compute things " << getFailCode();

	Any data type you want to print must be compatible with std::ostream

	If you ever invoke FATAL(), the process will exit with status EXIT_FAILURE.

*/








#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <cstdio>
#include <ctime>

#define _LOG_FORMAT(L) Logger().gen(L,__FILE__,__LINE__)
#define FATAL() _LOG_FORMAT(logFATAL)
#define ERROR() _LOG_FORMAT(logERROR)
#define WARN() _LOG_FORMAT(logWARN)
#define INFO() _LOG_FORMAT(logINFO)
#define DEBUG() _LOG_FORMAT(logDEBUG)
#define VERBOSE() _LOG_FORMAT(logVERBOSE)

// Levels, FATAL will exit program
enum kLogLevel {
	logNONE = -1,
	logFATAL,
	logERROR,
	logWARN,
	logINFO,
	logDEBUG,
	logVERBOSE
};

// Conversion from enum to string
static const std::string _LoggerLevelStrings[] = {"FATAL","ERROR","WARN ","INFO ","DEBUG","VERBOS"};
static const std::string _LoggerLevelStringsANSI[] = {"\033[1;41;37mFATAL","\033[1;31mERROR","\033[1;33mWARN ","\033[35mINFO ","DEBUG","VERBOS"};

class Logger {
public:
	Logger() {};
	~Logger() {
		kLogLevel levelThresh = logNONE;

		// Check debug level (specified at compile, or where ever)
		#ifdef LOG_FATAL
			levelThresh = logFATAL;
		#elif LOG_ERROR
			levelThresh = logERROR;
		#elif LOG_WARN
			levelThresh = logWARN;
		#elif LOG_INFO
			levelThresh = logINFO;
		#elif LOG_DEBUG
			levelThresh = logDEBUG;
		#elif LOG_VERBOSE
			levelThresh = logVERBOSE;
		#endif

		if (setLevel <= levelThresh) {
			// Print out, without delay ;)
			oss << std::endl;
			fprintf(stderr, "%s", oss.str().c_str());
			fflush(stderr);

			// If FATAL, crash and burn
			if (setLevel == logFATAL) {
				std::exit(EXIT_FAILURE);
			}
		}
	}

	// Generate ostream (with meta), return to inline call position
	std::ostringstream& gen(kLogLevel level, std::string file, int line) {
		// Format: [20:04:09][FATAL]:main.cpp:6: ...
		setLevel = level;
		oss << "[" << getTime() << "][";

		#ifdef LOG_USE_ANSI
			oss << _LoggerLevelStringsANSI[level];
			oss << "\033[0m";
		#else
			oss << _LoggerLevelStrings[level];
		#endif

		oss << "]:" << file << ":" << line << ": ";

		return oss;
	}
private:
	std::ostringstream oss;
	std::string getTime() {
		// http://stackoverflow.com/questions/997512/string-representation-of-time-t
		std::time_t now = std::time(NULL);
		std::tm * ptm = std::localtime(&now);
		char buffer[12];
		// Format: 20:20:00
		std::strftime(buffer, 12, "%H:%M:%S", ptm);

		return buffer;
	}
	kLogLevel setLevel;
};

#endif // LOGGER_H
