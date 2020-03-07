/*
 * Astra.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Nathan Bak
 */

#ifndef ASTRA_H_
#define ASTRA_H_

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <AstraLogger.h>

class KeyValStore {
public:

	KeyValStore();

	AstraLogger* logger;

	int init(const char* id, const char* region, const char* username, const char* password, const char* keyspace);
	int writeValue(const char* key, const char* val);
	String readValue(const char* key);

private:
	enum httpMethod  {GET, POST};
	const char* appsSubdomain = ".apps.astra.datastax.com";
	const int httpsPort = 443;

	const char* id;
	const char* region;
	const char* username;
	const char* password;
	const char* keyspace;

	String authToken;
	String response;

	int connect();
	int createTable();
	int request(httpMethod hm, const char* path, const char* body);
	int _request(httpMethod hm, const char* path, const char* body);
};

class AstraClient {
public:
	struct KeyVal {
		const char* key;
		const char* val;
	};

	AstraClient();

	AstraLogger* logger;

	int connect(const char* id, const char* region, const char* username, const char* password);
	int getKeyspaces();
	int getTables(const char* keyspace);
	int addRow(const char* keyspace, const char* table, int colCount, KeyVal columns[]);

private:
	enum httpMethod  {GET, POST};
	const char* appsSubdomain = ".apps.astra.datastax.com";
	const int httpsPort = 443;

	const char* id;
	const char* region;
	const char* username;
	const char* password;

	String authToken;
	String response;

	int connect();
	int request(httpMethod hm, const char* path, const char* body);
	int _request(httpMethod hm, const char* path, const char* body);

//	void setAuthToken();
//	void logDebug(char* msg);
//	void logDebug(const char* msg);
//	int request(httpMethod hm, const char* path, const char* body);
//	int _request(httpMethod hm, const char* path, const char* body);
};

#endif /* ASTRA_H_ */
