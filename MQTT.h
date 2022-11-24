#pragma once
#include <string>
#include <sstream>

#include "MQTTPublish.h"
#include "MQTTSubscribe.h"
#include "MQTTPubSub.h"

class MQTT {
public:
	static int menu(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
private:
	static std::vector<std::string> splitStringBySpace(std::string input);

	static int publish(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
	static int subscribe(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
	static int pubsub(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
};