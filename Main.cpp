// Inspired by topic_publish.cpp and async_consume.cpp from Frank Pagliughi <fpagliughi@mindspring.com>
//
// It's an example of how to send messages as an MQTT publisher and subscriber
// using the C++ asynchronous client interface using a 'topic' object to repeatedly
// send/receive data to/from topic(s). The program has 3 modes. Only publishing,
// only subscribing and both publishing/subscribing at the same time. With the
// Pub/Sub mode you can publish and subscribe to multiple topics, these can be
// different for publish and subscribe.
//
// The program demonstrates:
//  - Connecting to an MQTT server/broker
//  - Publishing messages to multiple topics
//  - User identification when publishing messages
//  - Subscribing to multiple topics
//  - Lambda functions to catch the received subscribe messages
//  - Use of the 'topic' class for both publishing and subscribing
//
//	To use the custom broker (if mosquitto broker is initialized and installed) put "net start mosquitto" into an admin cmd.
//  To stop disconnect the broker use "net stop mosquitto".
//

#include "MQTT.h"

// Example of arguments
// SERVER_ADDRESS = "tcp://test.mosquitto.org:1883"
//	OR "tcp://10.74.53.79:1883"
// QOS			  = 1
// USER_ID		  = user1

bool checkQOS(std::string string) {
	bool ret = true;
	if (string == "1" || string == "2" || string == "3")
		ret = false;
	return ret;
}

int main(int argc, char* argv[]) {
	if (argc != 4 || checkQOS(argv[2])) {
		std::cout << "USAGE: paho_mqtt_cpp_test <SERVER_ADDRESS> <QOS> <USER_ID>\n-QOS can only be 1, 2 or 3." << std::endl;
		return 1;
	}
	std::string SERVER_ADDRESS{ argv[1] }, CLIENT_ID{ argv[3] };
	int QOS = atoi(argv[2]);
	if (MQTT::menu(CLIENT_ID, SERVER_ADDRESS, QOS) == 1)
		std::cout << "\nAn error occured, so the program was stopped.\n";
}