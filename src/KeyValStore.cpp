/*
 * Astra.cpp
 *
 *  Created on: Feb 17, 2020
 *      Author: Nathan Bak
 */

#include "Astra.h"

KeyValStore::KeyValStore() {
	this->logger = new AstraLogger();
}

int KeyValStore::init(const char* id, const char* region, const char* username,
		const char* password, const char* keyspace) {
	this->id = id;
	this->region = region;
	this->username = username;
	this->password = password;
	this->keyspace = keyspace;

	logger->log(LEVEL_INFO, String("Database id:       ") + id);
	logger->log(LEVEL_INFO, String("Database region:   ") + region);
	logger->log(LEVEL_INFO, String("Database username: ") + username);
	logger->log(LEVEL_INFO, String("Database password: ") + "********");
	logger->log(LEVEL_INFO, String("Database keyspace: ") + keyspace);

	int code = connect();
	if (code) {
		return code;
	}

	return createTable();
}

int KeyValStore::connect() {
	String jsonSnippet = String("{\"username\":\"") + username
			+ "\",\"password\":\"" + password + "\"}";

	int code = request(POST, "/api/rest/v1/auth", jsonSnippet.c_str());

	if (response.startsWith("{\"authToken\":")) {
		logger->log(LEVEL_DEBUG, "auth token obtained successfully!");
		response = response.substring(14);
		response = response.substring(0, response.indexOf("\""));
		authToken = response;
		logger->log(LEVEL_INFO,
				"Connected and authenticated to Astra database");
		return 0;
	} else {
		logger->log(LEVEL_ERROR, "Error authenticating");
		logger->log(LEVEL_ERROR, response);
		return 1;
	}
}

int KeyValStore::writeValue(const char* key, const char* val) {
	String path = String("/api/rest/v1/keyspaces/") + keyspace
			+ "/tables/kvstore/rows";

	String body = "{\"columns\":[{\"name\":\"key\", \"value\":\"";
	body.concat(key);
	body.concat("\"},{\"name\":\"val\", \"value\":\"");
	body.concat(val);
	body.concat("\"}]}");

	logger->log(LEVEL_DEBUG, "add body: ");
	logger->log(LEVEL_DEBUG, body);

	int code = request(POST, path.c_str(), body.c_str());

	if (code != 201) {
		logger->log(LEVEL_DEBUG, code + " error adding row");
		logger->log(LEVEL_ERROR, response);
		return code;
	}

	logger->log(LEVEL_INFO, String("SET\t") + key + "\t=\t" + val);
	return 0;
}

String KeyValStore::readValue(const char* key) {
	String path = String("/api/rest/v1/keyspaces/") + keyspace
			+ "/tables/kvstore/rows/" + key;

	int code = request(GET, path.c_str(), NULL);

	if (code != 200) {
		logger->log(LEVEL_DEBUG, code);
		logger->log(LEVEL_ERROR, "Error reading row");
		logger->log(LEVEL_ERROR, response);
		return String(NULL);
	}

	String val = response.substring(response.indexOf("val\":\"") + 6);

	return val.substring(0, val.lastIndexOf("\"}],"));
}

int KeyValStore::createTable() {
	String path = String("/api/rest/v1/keyspaces/") + keyspace + "/tables";

	const char* body =
			"{\"name\":\"kvstore\",\"ifNotExists\":true,\"columnDefinitions\":[{\"name\":\"key\",\"typeDefinition\":\"text\",\"static\":false},{\"name\":\"val\",\"typeDefinition\":\"text\",\"static\":false}],\"primaryKey\":{\"partitionKey\":[\"key\"]},\"tableOptions\":{\"defaultTimeToLive\":0}}";

	logger->log(LEVEL_DEBUG, "add body: ");
	logger->log(LEVEL_DEBUG, body);

	int code = request(POST, path.c_str(), body);

	if (code != 201) {
		logger->log(LEVEL_DEBUG, code);
		logger->log(LEVEL_ERROR, "error creating table");
		logger->log(LEVEL_ERROR, response);
		return code;
	}
	return 0;
}

int KeyValStore::request(httpMethod hm, const char* path, const char* body) {
	int code = _request(hm, path, body);

// if we get an auth error, get a new auth token and try again
	if (code == 401) {
		connect();
		return _request(hm, path, body);
	}
	return code;
}

int KeyValStore::_request(httpMethod hm, const char* path, const char* body) {
	WiFiClientSecure client;
	client.setInsecure();

	const String hypen = "-";
	String hostStr = id + hypen + region + appsSubdomain;
	const char* host = hostStr.c_str();

	response = "";
	logger->log(LEVEL_DEBUG, "connecting to ");
	logger->log(LEVEL_DEBUG, host);
	if (!client.connect(host, httpsPort)) {
		logger->log(LEVEL_ERROR, "connection failed");
		return 1;
	}
	logger->log(LEVEL_DEBUG, "requesting path: ");
	logger->log(LEVEL_DEBUG, path);

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

	logger->log(LEVEL_DEBUG, "request sent");
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		logger->log(LEVEL_DEBUG, line);
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
	logger->log(LEVEL_DEBUG, response);
	logger->log(LEVEL_DEBUG, "closing connection");
	client.stop();

	return code;
}

