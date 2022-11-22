#pragma once
#include "mqtt/async_client.h"

class MQTTSubscribe {
public:
	MQTTSubscribe(const std::string serverAddress, const std::string clientID, const int QoS, const bool NO_LOCAL = true);
	~MQTTSubscribe();

	// Setup functions
	void connectToServer();
	void disconnectFromServer();

	// Active functions
	void subscribe(std::string topic);
private:
	const std::string serverAddress;
	const std::string clientID;
	const int QoS; // Quality of Service
	const bool NO_LOCAL;

	mqtt::async_client* cli;
};