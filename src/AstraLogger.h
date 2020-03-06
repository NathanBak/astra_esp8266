/*
 * AstraLogger.h
 *
 *  Created on: March 5, 2020
 *      Author: Nathan Bak
 */

#ifndef ASTRALOGGER_H_
#define ASTRALOGGER_H_

#include <Arduino.h>

class AstraLogger {
public:
#define LEVEL_NONE 0
#define LEVEL_TRACE 2
#define LEVEL_DEBUG 4
#define LEVEL_INFO 6
#define LEVEL_WARN 8
#define LEVEL_ERROR 10

	AstraLogger();
	int getLevel();
	void setLevel(int level);

	void log(int level, const char* msg);
	void log(int level, String msg);
	void log(int level, int msg);

private:
	int logLevel = LEVEL_INFO;
	bool shouldLog(int level);
	void printPrefix(int level);
};

#endif /* ASTRALOGGER_H_ */
