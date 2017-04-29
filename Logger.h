
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
		METHOD();
	This will indent the logs, and then finish when the function goes out of scope. Warning, this can get expensive.

*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>



#define _LOG_FILENAME_MAX 8


#define _LOG_FORMAT(L,A) Logger().gen(L,A,__FILE__,__LINE__)
#define _LOG_NULLSTREAM Logger::nullStream.clear(); Logger::nullStream

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


#define _METHOD_FORMAT(f) LoggerScope _LoggerScope_##f##_scope(f)
#define METHOD() _METHOD_FORMAT(__FUNCTION__)

class LoggerScope {
public:
	LoggerScope(std::string);
	~LoggerScope();

	static int indentLevel;
private:
	std::string name;
};

class Logger {
public:
	Logger();
	~Logger();
	std::ostringstream& gen(std::string, std::string, std::string, int);

	static std::ostringstream nullStream;

private:
	std::ostringstream oss;
	std::string getTime();
	std::string setLevel;

};

Logger::Logger() {};
Logger::~Logger() {
	// Print out, without delay ;)
	std::cerr << oss.str() << std::endl;

	// If FATAL, crash and burn
	if (setLevel == "FATAL") {

		std::exit(EXIT_FAILURE);
	}
}

	// Generate ostream (with meta), return to inline call position
std::ostringstream& Logger::gen(std::string label, std::string ansi, std::string file, int line) {
	// Format: [20:04:09][FATAL]:main.cpp:6: ...
	setLevel = label;
	oss << "[" << getTime() << "][";

	// Supress usage warnings
	ansi = ansi;

	#ifdef LOG_NO_ANSI
		oss << label;
	#else
		oss << "\033[" << ansi << label << "\033[0m";
	#endif

	// Remove leading path name
	file = file.erase(0, file.find_last_of("\\/")+1);

	// Trim/expand to consistent size
	if (file.size() > _LOG_FILENAME_MAX) {
		file = file.substr(0,_LOG_FILENAME_MAX-3);
		file += "...";
	} else if (file.size() < _LOG_FILENAME_MAX) {
		file += std::string(" ", _LOG_FILENAME_MAX - file.size());
	}

	oss << "]:" << file << ":";
	oss << std::setw(3) << std::setfill(' ') << line << ": ";
	oss << std::string(LoggerScope::indentLevel < 0 ? 0 : LoggerScope::indentLevel*2 , ' ');

	return oss;
}

std::string Logger::getTime() {
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

std::ostringstream Logger::nullStream;




#define _METHOD_FORMAT(f) LoggerScope _LoggerScope_##f##_scope(f)
#define METHOD() _METHOD_FORMAT(__FUNCTION__)

LoggerScope::LoggerScope(std::string n) {
	name = n;
	INFO() << "> START: " << name;
	indentLevel++;
}

LoggerScope::~LoggerScope() {
	indentLevel--;
	INFO() << "< END: " << name;
}

int LoggerScope::indentLevel = 0;

#endif // LOGGER_H
