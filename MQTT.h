#pragma once
#include <string>
#include <sstream>

#include "MQTTPublish.h"
#include "MQTTSubscribe.h"
#include "MQTTPubSub.h"

class MQTT {
public:
	static void menu(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
private:
	static std::vector<std::string> splitStringBySpace(std::string input);

	static void publish(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
	static void subscribe(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
	static void pubsub(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS);
};