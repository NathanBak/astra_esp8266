/*
 * Astra.cpp
 *
 *  Created on: Feb 17, 2020
 *      Author: Nathan Bak
 */

#include "Astra.h"



AstraClient::AstraClient() {
}

void AstraClient::printSettings() {
	Serial.print("Database id:       ");
	Serial.println(id);
	Serial.print("Database region:   ");
	Serial.println(region);
	Serial.print("Database username: ");
	Serial.println(username);
	Serial.print("Database password: ");
	Serial.println("********");
}

int AstraClient::connect(const char* id, const char* region,
		const char* username, const char* password) {
	this->id = id;
	this->region = region;
	this->username = username;
	this->password = password;
	connect();
}

int AstraClient::connect() {
	String jsonSnippet= String("{\"username\":\"") + username + "\",\"password\":\"" + password + "\"}";

	int code = request(POST, "/api/rest/v1/auth", jsonSnippet.c_str());

	if (response.startsWith("{\"authToken\":")) {
		Serial.println("auth token obtained successfully!");
		response = response.substring(14);
		response = response.substring(0, response.indexOf("\""));
		authToken = response;
		return 0;
	} else {
		Serial.println("error authenticating");
		Serial.println(response);
		return 1;
	}
}

int AstraClient::getKeyspaces() {
	int code = request(GET, "/api/rest/v1/keyspaces", NULL);
	Serial.print("Keyspaces:");
	Serial.println(response);
	return code;
}

int AstraClient::getTables(const char* keyspace) {
	String path = String("/api/rest/v1/keyspaces/") + keyspace + "/tables";
	int code = request(GET, path.c_str(), NULL);
	Serial.println("Tables:");
	Serial.println(response);
	return code;
}

int AstraClient::addRow(const char* keyspace, const char* table, int colCount, KeyVal columns[]) {
	  String path = String("/api/rest/v1/keyspaces/") + keyspace + "/tables/" + table + "/rows";

	  String body = "{\"columns\":[";

	  for (int i=0; i<colCount; i++) {
		  body.concat("{\"name\":\"");
		  body.concat(columns[i].key);
		  body.concat("\",\"value\":\"");
		  body.concat(columns[i].val);
		  body.concat("\"}");
		  if (i<colCount-1) {
			  body.concat(",");
		  }
	  }

	  body.concat("]}");

	  Serial.print("da add body: ");
	  Serial.println(body);


	  int code = request(POST, path.c_str(), body.c_str());

	  if (code != 201) {
	    Serial.print(code);
	    Serial.println(" error adding row");
	    Serial.println(response);
	  }
	  return code;
}


int AstraClient::request(httpMethod hm, const char* path, const char* body) {
	int code = _request(hm, path, body);

	// if we get an auth error, get a new auth token and try again
	if (code == 401) {
		connect();
		return _request(hm, path, body);
	}
	return code;
}

int AstraClient::_request(httpMethod hm, const char* path, const char* body) {
	WiFiClientSecure client;
	client.setInsecure();

	const String hypen = "-";
	const char* appsSubdomain = ".apps.apollo.datastax.com";
	String hostStr = id + hypen + region + appsSubdomain;
	const char* host = hostStr.c_str();

	response = "";
	Serial.println("connecting to ");
	Serial.println(host);
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return -1;
	}
	Serial.println("requesting path: ");
	Serial.println(path);

	switch (hm) {
	case POST:
		client.print("POST ");
		break;
	default:
		client.print("GET ");
	}
	client.print(
			path + String(" HTTP/1.1\r\n") + "Host: " + host + "\r\n"
					+ "Content-Type: application/json\r\n"
					+ "X-Cassandra-Token: " + authToken + "\r\n"
					+ "Connection: close\r\n");

	if (body != NULL) {
		client.print("Content-Length: ");
		client.println(strlen(body)); // number of bytes in body
		client.println(); // we need an empty line between headers and body here
		client.print(body);
	}
	client.println();

	int code = 0;

	Serial.println("request sent");
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		Serial.println(line);
		// get http response code
		if (code == 0 && line.startsWith("HTTP/1.1 ")) {
			String codeStr = line.substring(9);
			codeStr = codeStr.substring(0, 3);
			code = atoi(codeStr.c_str());
		}
		if (line == "\r") {
			// done reading headers
			break;
		}
	}

	// read body of response
	response = client.readStringUntil('\n');
	Serial.println(response);
	Serial.println("closing connection");
	client.stop();

	return code;
}

