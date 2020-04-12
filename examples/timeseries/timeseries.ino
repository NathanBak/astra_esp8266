#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>  // Needed to get current time
#include <WiFiUdp.h>    // Needed by NTPClient.h
#include <TimeLib.h>    // Needed to convert current time into components
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

// Name of table created in the database keyspace.  The code below assumes that
// the table was created with a CQL statement like:
//
// CREATE TABLE IF NOT EXISTS jikandata (
//   sensorid text,
//   year int,
//   month int,
//   day int,
//   hour int,
//   minute int,
//   second int,
//   data int
//   PRIMARY KEY ((sensorid, year, month), day, hour, minute, second)
//   ) WITH CLUSTERING ORDER BY (day DESC, hour DESC, minute DESC, second DESC);
const char* table = "jikandata";

// This is how the sensor is identified within the database.  In this example, 
// it is "ESP_" followed by the mac address of the ESP8266 board (see the
// setSensorId() function).
char sensorId[16];

// The NTPClient is how we get the current time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

AstraClient client = AstraClient();

void setup() {

  // Setup serial connection
  Serial.begin(115200);

  // Connect to Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Initialize sensor id
  setSensorId();

  // Set the log level to be used by the Astra client.
  // Values include LEVEL_NONE, LEVEL_DEBUG, LEVEL_INFO, LEVEL_ERROR
  client.logger->setLevel(LEVEL_INFO);

  // Initialize Astra client.  This will connect to the database and make sure
  // we can authenticate.  The connect() method returns 0 for success or non-
  // zero if it fails.
  client.connect(dbId, dbRegion, dbUser, dbPassword);

  // Initialize time client
  timeClient.begin();

  // Initialize random seed for use in loop() below.  Normally this is where
  // a sensor could be initialized.
  randomSeed(19750317);
}

void loop() {


  // Get time information
  timeClient.update();
  time_t now = timeClient.getEpochTime();
  setTime(now);

  // Here the "data" is just a random integer, but presumably it would
  // really come from a sensor
  int randomNumber = random(2000000000);
  String randStr = String(randomNumber).c_str();
  const char* data = randStr.c_str();

  // Get different time units and convert ints to cstrings
  char theYear[4];
  itoa(year(), theYear, 10);
  char theMonth[8];
  itoa(month(), theMonth, 10);
  char theDay[4];
  itoa(day(), theDay, 10);
  char theHour[4];
  itoa(hour(), theHour, 10);
  char theMinute[4];
  itoa(minute(), theMinute, 10);
  char theSecond[4];
  itoa(second(), theSecond, 10);

  struct AstraClient::KeyVal columns[8] = { 
		  { "sensorid", sensorId }, 
		  { "year", theYear }, 
		  { "month", theMonth }, 
		  { "day", theDay }, 
		  { "hour", theHour }, 
		  { "minute", theMinute }, 
		  { "second", theSecond }, 
		  { "data", data } 
  };
  client.addRow(keyspace, table, 8, columns);

  // Sleep for a random interval up to 20 seconds
  delay(random(20000));
}

void setSensorId() {
  uint8_t macArray[6];
  WiFi.macAddress(macArray);
  sensorId[0] = 'E';
  sensorId[1] = 'S';
  sensorId[2] = 'P';
  sensorId[3] = '_';
  for (int i = 0; i < sizeof(macArray); ++i) {
    sprintf(sensorId, "%s%02X", sensorId, macArray[i]);
  }
}

