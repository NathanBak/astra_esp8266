# astra_esp8266

## Introduction
Access a Cassandra database from an ESP8266 or ESP32.  The astra_esp8266 library provides basic operations to connect with a Datastax Astra database.  The libray also tries to provided "greased" paths for various common use cases such as basic key/value storage and storing time series sensor data. 

## Prereqs
- Astra Database:  This library requires a [Datastax Astra database](https://www.datastax.com/cloud/datastax-astra) which is built on top of Cassandra.  The free tier of the database will be more than adequate for most uses.  To create an Astra database, go to [https://astra.datastax.com/](https://astra.datastax.com/) and follow the instructions to register and create a database.

## Examples

### Key/Value Storage
It is easy to use an Astra database as a basic key/value store.  The keyValStore example shows how to connect to an Astra database, write some data to the database, and then read back data from the database.  In order to be able to connect to your wifi network and also your Astra database, the following const char* values need to be set:

- ssid
- password
- dbId
- dbRegion
- dbUser
- dbPassword

The comments in keyValStore.ino should describe what information needs to be specified.  Once the file has been updated with your own information, the sketch should be ready to compiled, uploaded, and run.

The Astra KeyValStore client can be easily embedded into any sketch where you need to store or retrieve simple key/val type data.  The client takes care of creating the database table--the table name is "kvstore" if you want to access it via CQL (see the Astra website for different ways to access your database).

### Timeseries Data
_coming soon_

