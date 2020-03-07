/*
 * AstraLogger.cpp
 *
 *  Created on: March 5, 2020
 *      Author: Nathan Bak
 */

#include "AstraLogger.h"

AstraLogger::AstraLogger() {
}

int AstraLogger::getLevel() {
	return this->logLevel;
}

void AstraLogger::setLevel(int level) {
	this->logLevel = level;
}

void AstraLogger::log(int level, const char* msg) {
	if (shouldLog(level)) {
		printPrefix(level);
		Serial.println(msg);
	}
}

void AstraLogger::log(int level, String msg) {
	if (shouldLog(level)) {
		printPrefix(level);
		Serial.println(msg);
	}
}

void AstraLogger::log(int level, int msg) {
	if (shouldLog(level)) {
		printPrefix(level);
		Serial.println(msg);
	}
}

bool AstraLogger::shouldLog(int level) {
	return level >= this->logLevel;
}

void AstraLogger::printPrefix(int level) {

	switch (level) {
	case LEVEL_TRACE:
		Serial.print("TRACE  ");
		break;
	case LEVEL_DEBUG:
		Serial.print("DEBUG  ");
		break;
	case LEVEL_INFO:
		Serial.print("INFO   ");
		break;
	case LEVEL_WARN:
		Serial.print("WARN   ");
		break;
	case LEVEL_ERROR:
		Serial.print("ERROR  ");
		break;
	default:
		Serial.print(level);
		Serial.print("      ");
		break;
	}
}

