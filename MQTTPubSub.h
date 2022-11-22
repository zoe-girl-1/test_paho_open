#pragma once
#include "mqtt/async_client.h"

class MQTTPubSub {
public:
	MQTTPubSub(const std::string serverAddress, const std::string clientID, const int QoS, const bool NO_LOCAL = true);
	~MQTTPubSub();

	// Setup functions
	void connectToServer();
	void disconnectFromServer();

	// Active functions
	int sub(std::string topic);
	int pub(std::string topic, std::string message);
	int pubsub(std::vector<std::string> topics_sub, std::vector<std::string> topics_pub);
private:
	std::string topicInput;

	const std::string serverAddress;
	const std::string clientID;
	const int QoS;
	const bool NO_LOCAL;

	mqtt::async_client* cli;
};