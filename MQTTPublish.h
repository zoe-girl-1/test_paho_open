#pragma once
#include "mqtt/async_client.h"

class MQTTPublish {
public:
	MQTTPublish(const std::string serverAddress, const std::string clientID, const int QoS);
	~MQTTPublish();

	// Setup functions
	void connectToServer();
	void disconnectFromServer();

	// Active function
	void publish(std::string topic, /*std::vector<*/std::string/*>*/ messages);

private:
	const std::string serverAddress;
	const std::string clientID;
	const int QoS; // Quality of Service

	mqtt::async_client* cli;
};