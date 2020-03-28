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


// Name of table created in the database keyspace.  The code below assumes that
// the table was created with a CQL statement like:
//	 CREATE TABLE IF NOT EXISTS containernames (
//	   adjective text,
//	   surname text,
//	   randval int,
//	   PRIMARY KEY ((adjective), surname)
//	   );
const char* table = "containernames";

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
	
	// Set the log level to be used by the Astra client.
	// Values include LEVEL_NONE, LEVEL_DEBUG, LEVEL_INFO, LEVEL_ERROR
	client.logger->setLevel(LEVEL_INFO);

	// Initialize Astra client.  This will connect to the database and make sure
	// we can authenticate.  The connect() method returns 0 for success or non-
	// zero if it fails.
	client.connect(dbId, dbRegion, dbUser, dbPassword);

	// Initialize random seed for use in loop() below
	// (Replace 19750317 with analogRead(0) for a non-fixed seed)
	randomSeed(19750317);
}

// The adjective_name combination arrays and generation code below are based on
// https://github.com/moby/moby/blob/master/pkg/namesgenerator/names-generator.go

const char *left[108] = {
    "admiring",
    "adoring",
    "affectionate",
    "agitated",
    "amazing",
    "angry",
    "awesome",
    "beautiful",
    "blissful",
    "bold",
    "boring",
    "brave",
    "busy",
    "charming",
    "clever",
    "cool",
    "compassionate",
    "competent",
    "condescending",
    "confident",
    "cranky",
    "crazy",
    "dazzling",
    "determined",
    "distracted",
    "dreamy",
    "eager",
    "ecstatic",
    "elastic",
    "elated",
    "elegant",
    "eloquent",
    "epic",
    "exciting",
    "fervent",
    "festive",
    "flamboyant",
    "focused",
    "friendly",
    "frosty",
    "funny",
    "gallant",
    "gifted",
    "goofy",
    "gracious",
    "great",
    "happy",
    "hardcore",
    "heuristic",
    "hopeful",
    "hungry",
    "infallible",
    "inspiring",
    "interesting",
    "intelligent",
    "jolly",
    "jovial",
    "keen",
    "kind",
    "laughing",
    "loving",
    "lucid",
    "magical",
    "mystifying",
    "modest",
    "musing",
    "naughty",
    "nervous",
    "nice",
    "nifty",
    "nostalgic",
    "objective",
    "optimistic",
    "peaceful",
    "pedantic",
    "pensive",
    "practical",
    "priceless",
    "quirky",
    "quizzical",
    "recursing",
    "relaxed",
    "reverent",
    "romantic",
    "sad",
    "serene",
    "sharp",
    "silly",
    "sleepy",
    "stoic",
    "strange",
    "stupefied",
    "suspicious",
    "sweet",
    "tender",
    "thirsty",
    "trusting",
    "unruffled",
    "upbeat",
    "vibrant",
    "vigilant",
    "vigorous",
    "wizardly",
    "wonderful",
    "xenodochial",
    "youthful",
    "zealous",
    "zen"
    };

const char *right[237] = {
	"albattani",
	"allen",
	"almeida",
	"antonelli",
	"agnesi",
	"archimedes",
	"ardinghelli",
	"aryabhata",
	"austin",
	"babbage",
	"banach",
	"banzai",
	"bardeen",
	"bartik",
	"bassi",
	"beaver",
	"bell",
	"benz",
	"bhabha",
	"bhaskara",
	"black",
	"blackburn",
	"blackwell",
	"bohr",
	"booth",
	"borg",
	"bose",
	"bouman",
	"boyd",
	"brahmagupta",
	"brattain",
	"brown",
	"buck",
	"burnell",
	"cannon",
	"carson",
	"cartwright",
	"carver",
	"cerf",
	"chandrasekhar",
	"chaplygin",
	"chatelet",
	"chatterjee",
	"chebyshev",
	"cohen",
	"chaum",
	"clarke",
	"colden",
	"cori",
	"cray",
	"curran",
	"curie",
	"darwin",
	"davinci",
	"dewdney",
	"dhawan",
	"diffie",
	"dijkstra",
	"dirac",
	"driscoll",
	"dubinsky",
	"easley",
	"edison",
	"einstein",
	"elbakyan",
	"elgamal",
	"elion",
	"ellis",
	"engelbart",
	"euclid",
	"euler",
	"faraday",
	"feistel",
	"fermat",
	"fermi",
	"feynman",
	"franklin",
	"gagarin",
	"galileo",
	"galois",
	"ganguly",
	"gates",
	"gauss",
	"germain",
	"goldberg",
	"goldstine",
	"goldwasser",
	"golick",
	"goodall",
	"gould",
	"greider",
	"grothendieck",
	"haibt",
	"hamilton",
	"haslett",
	"hawking",
	"hellman",
	"heisenberg",
	"hermann",
	"herschel",
	"hertz",
	"heyrovsky",
	"hodgkin",
	"hofstadter",
	"hoover",
	"hopper",
	"hugle",
	"hypatia",
	"ishizaka",
	"jackson",
	"jang",
	"jemison",
	"jennings",
	"jepsen",
	"johnson",
	"joliot",
	"jones",
	"kalam",
	"kapitsa",
	"kare",
	"keldysh",
	"keller",
	"kepler",
	"khayyam",
	"khorana",
	"kilby",
	"kirch",
	"knuth",
	"kowalevski",
	"lalande",
	"lamarr",
	"lamport",
	"leakey",
	"leavitt",
	"lederberg",
	"lehmann",
	"lewin",
	"lichterman",
	"liskov",
	"lovelace",
	"lumiere",
	"mahavira",
	"margulis",
	"matsumoto",
	"maxwell",
	"mayer",
	"mccarthy",
	"mcclintock",
	"mclaren",
	"mclean",
	"mcnulty",
	"mendel",
	"mendeleev",
	"meitner",
	"meninsky",
	"merkle",
	"mestorf",
	"mirzakhani",
	"moore",
	"morse",
	"murdock",
	"moser",
	"napier",
	"nash",
	"neumann",
	"newton",
	"nightingale",
	"nobel",
	"noether",
	"northcutt",
	"noyce",
	"panini",
	"pare",
	"pascal",
	"pasteur",
	"payne",
	"perlman",
	"pike",
	"poincare",
	"poitras",
	"proskuriakova",
	"ptolemy",
	"raman",
	"ramanujan",
	"ride",
	"montalcini",
	"ritchie",
	"rhodes",
	"robinson",
	"roentgen",
	"rosalind",
	"rubin",
	"saha",
	"sammet",
	"sanderson",
	"satoshi",
	"shamir",
	"shannon",
	"shaw",
	"shirley",
	"shockley",
	"shtern",
	"sinoussi",
	"snyder",
	"solomon",
	"spence",
	"stonebraker",
	"sutherland",
	"swanson",
	"swartz",
	"swirles",
	"taussig",
	"tereshkova",
	"tesla",
	"tharp",
	"thompson",
	"torvalds",
	"tu",
	"turing",
	"varahamihira",
	"vaughan",
	"visvesvaraya",
	"volhard",
	"villani",
	"wescoff",
	"wilbur",
	"wiles",
	"williams",
	"williamson",
	"wilson",
	"wing",
	"wozniak",
	"wright",
	"wu",
	"yalow",
	"yonath",
	"zhukovsky",
	};

void loop() {

  // Add a row with a random adjective, surname, and number
  int adjectiveNumber = random(108);
  int surnameNumber = random(237);
  int randomNumber = random(2000000000);

  // Convert the int to const char*.  Only const char* are allowed for 
  // values even if the column type in the table is an int.  It will be
  // converted back to an int when as it is added to the table.
  String randStr = String(randomNumber).c_str();
  const char* randChars = randStr.c_str();
  
  Serial.println((String)left[adjectiveNumber] + "_" + right[surnameNumber] + "\t" + randStr);
  
  struct AstraClient::KeyVal columns[3] = {
      {"adjective", left[adjectiveNumber]},
      {"surname", right[surnameNumber]},
      {"randval", randChars}
  };
  client.addRow(keyspace, table, 3, columns);

  // Sleep for a random interval up to 20 seconds
  delay(random(20000));
}

  


