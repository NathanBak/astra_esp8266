#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Astra.h>

// Wifi information
const char* ssid = "myWifi";  // Name of your wifi network
const char* password = "myWifi";  // Password for your wifi network

// Astra Database information

// Database UUID - you can find it in the URL when viewing the database in the
// Astra web UI it is after the "database/" so if the URL looks like
// https://astra.datastax.com/database/12345678-abcd-90ef-bcdf-123456789abc
// then your dbUUID will be 12345678-abcd-90ef-bcdf-123456789abc
const char* dbId = "12345678-abcd-90ef-bcdf-123456789abc";

// Database region - you set this when creating the database.  If you forgot,
// you can find it on the Astra web UI.  When viewing your database, it will be
// in the "Locations" field in the "Size and Location" section.  Note that the
// cloud provider (like "gcp") is not part of the region.
const char* dbRegion = "us-east1";

// Database username - this is different than your user name for your Astra
// account.  It is the database user name that was specified when creating the
// database.  You can find it in the Astra web UI--when viewing your database,
// it will be in the "Username" field in the "Connection Details" section.
const char* dbUser = "myDbUser";

// Database password - this is different than the password for your Astra
// account.  It is the database password that was specified when creating the
// database.  This value cannot be retrieved.  If your forgot your password, you
// can reset it from the Astra web UI--when viewing your database, click the
// "reset password" link in the "Connection Details" section.
const char* dbPassword = "myDbPassword";

// Database keyspace - this was set when you created the database.  You can find
// it in the Astra web UI--when viewing your database, it will be in the
// "Keyspace" field in the "Connection Details" section.
const char* keyspace = "myKeyspace";

KeyValStore kvs = KeyValStore();

void setup() {

	// Setup serial connection
	Serial.begin(115200);

	// Connect to Wifi
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}
	
	// Set the log level to be used by the KeyValStore client.
	// Values include LEVEL_NONE, LEVEL_DEBUG, LEVEL_INFO, LEVEL_ERROR
	kvs.logger->setLevel(LEVEL_INFO);

	// Initialize KeyValStore client.  This will connect to the database and
	// make sure we can authenticate.  It will then create the kvstore table (if
	// it does not already exist).  The init() method returns 0 for success or
	// non-zero if it fails.
	kvs.init(dbId, dbRegion, dbUser, dbPassword, keyspace);

	// Write some key/val combinations to the Astra Database
	// The writeValue() method returns 0 for success or non-zero for failure
	kvs.writeValue("Gadgetmobile", "Inspector Gadget");
	kvs.writeValue("Lightning McQueen", "Cars");
	kvs.writeValue("Ecto-1", "Ghostbusters");
	kvs.writeValue("General Lee", "Dukes of Hazzard");
	kvs.writeValue("Herbie", "The Love Bug");
	kvs.writeValue("Mutt Cutts", "Dumb and Dumber");
	kvs.writeValue("KITT", "Knight Rider");
	kvs.writeValue("Mystery Machine", "Scooby Doo");

	// Read back a value for a key in the Astra Database
	// The readValue() returns String(NULL) if there is an error or the value
	// cannot be found.
	String show = kvs.readValue("Herbie");
	Serial.println(show);  // This should print "The Love Bug"

	// Initialize random seed for use in loop() below
	randomSeed(analogRead(0));
}

void loop() {
	// write a random value to the Astra database
	kvs.writeValue("random value", String(random(2000000000)).c_str());

	// Sleep for a random interval up to 2 minutes
	delay(random(120000));
}

