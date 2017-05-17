
/*
 ===== HEADER ONLY LOGGER =====

	Light weight logger to be used anywhere in any project.

	@Author     Samuel Lewis
	@Updated 	29th April 2017
	@Version	1.2

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

	If your system does not support ANSI colour codes, you can additionally include this, to disable
		LOG_NO_ANSI

>> USAGE <<

	Where ever you want to log, use the level name and pipe in your stuff:
		INFO() << "words words " << 452 << 'a' << getSomeValue();
		FATAL() << "could not compute things ";

	Any data type you want to print must be compatible with std::ostream

	If you ever invoke FATAL(), the process will exit with status EXIT_FAILURE.

	If you want to track scope of a function, first line of the function, call
		METHOD()
	This will indent the logs, and then finish when the function goes out of scope. Warning, this can get expensive.

*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

#define _LOG_FILENAME_MAX 12

#define LOGGER_LOG(label, ansi) Logger().gen(label,ansi,__FILE__,__LINE__)

// FATAL
#if defined(LOG_FATAL) || defined(LOG_ERROR) || defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define FATAL() LOGGER_LOG("FATAL","1;41;37m")
#else
	#define FATAL() if (false) std::cerr
#endif

// ERROR
#if defined(LOG_ERROR) || defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define ERROR() LOGGER_LOG("ERROR","1;31m")
#else
	#define ERROR() if (false) std::cerr
#endif

// WARN
#if defined(LOG_WARN) || defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define WARN() LOGGER_LOG("WARN ","1;33m")
#else
	#define WARN() if (false) std::cerr
#endif

// INFO
#if defined(LOG_INFO) || defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define INFO() LOGGER_LOG("INFO ","35m")
#else
	#define INFO() if (false) std::cerr
#endif

// DEBUG and METHOD
#if defined(LOG_DEBUG) || defined(LOG_VERBOSE)
	#define DEBUG() LOGGER_LOG("DEBUG","0m")

	#define _LOGGER_METHOD_FORMAT(f) LoggerScope _LoggerScope_##f##_scope(f);
	#define METHOD() _LOGGER_METHOD_FORMAT(__FUNCTION__) LOGGER_LOG("METHD","36m") << "> START: " << __FUNCTION__ << " | "
#else
	#define DEBUG() if (false) std::cerr
	#define METHOD() if (false) std::cerr
#endif

// VERBOSE
#if defined(LOG_VERBOSE)
	#define VERBOSE() LOGGER_LOG("VERBO","0m")
#else
	#define VERBOSE() if (false) std::cerr
#endif

class Logger {
public:
	Logger() {}
	~Logger() {
		// Print out, without delay ;)
		std::cerr << _oss.str() << std::endl;

		// If FATAL, crash and burn
		if (currentLabel == "FATAL") {

			std::exit(EXIT_FAILURE);
		}
	}

	// Generate the stream to feed into
	std::ostringstream& gen(std::string label, std::string ansi, std::string file, int line) {
		// Format: [20:04:09][FATAL]:main.cpp:6: ...
		currentLabel = label;
		_oss << "[" << _getTime() << "][";

		// Supress usage warnings
		ansi = ansi;

		#ifdef LOG_NO_ANSI
			_oss << label;
		#else
			_oss << "\033[" << ansi << label << "\033[0m";
		#endif

		// Remove leading path name
		file = file.erase(0, file.find_last_of("\\/")+1);

		// Trim/expand to consistent size
		if (file.size() > _LOG_FILENAME_MAX) {
			file = file.substr(0,_LOG_FILENAME_MAX-3);
			file += "...";
		} else if (file.size() < _LOG_FILENAME_MAX) {
			file += std::string(_LOG_FILENAME_MAX - file.size(), ' ');
		}

		_oss << "]:" << file << ":";
		_oss << std::setw(3) << std::setfill(' ') << line << ": ";
		_oss << std::string(indentLevel(0)*2, ' ');

		return _oss;
	}

	// Sudo-static indent level var
	static int indentLevel(int mod) {
		static int i;
		i += mod;
		return i;
	}

private:
	std::string _getTime() {
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
	std::string currentLabel;
	std::ostringstream _oss;
};

class LoggerScope {
public:
	LoggerScope(std::string n) {
		_methodName = n;
		Logger::indentLevel(1);
	}

	~LoggerScope() {
		LOGGER_LOG("METHD","36m") << "< END: " << _methodName;
		Logger::indentLevel(-1);
	}
private:
	std::string _methodName;
};

#endif // LOGGER_H
