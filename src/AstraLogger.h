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
#define LEVEL_TRACE 10
#define LEVEL_DEBUG 20
#define LEVEL_INFO 30
#define LEVEL_WARN 40
#define LEVEL_ERROR 50
#define LEVEL_NONE 100


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
