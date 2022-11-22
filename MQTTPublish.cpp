#include "MQTTPublish.h"

MQTTPublish::MQTTPublish(const std::string serverAddress, const std::string clientID, const int QoS)
 : serverAddress(serverAddress), clientID(clientID), QoS(QoS) {
	std::cout << "Initialising server host '" << this->serverAddress << "'... ";
	try {
		this->cli = new mqtt::async_client(this->serverAddress, this->clientID);
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to initialize server host" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
MQTTPublish::~MQTTPublish() {
	std::cout << " ~DECONSTRUCTOR MQTTPUBLISH " << std::endl;
	delete this->cli;
}

void MQTTPublish::connectToServer() {
	std::cout << "Connecting to server... ";
	try {
		this->cli->connect()->wait();
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to connect to server" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
void MQTTPublish::disconnectFromServer() {
	std::cout << "Disconnecting from server... ";
	try {
		this->cli->disconnect()->wait();
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to disconnect from server" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}

void MQTTPublish::publish(std::string topic, /*std::vector<*/std::string/*>*/ messages) {
	std::cout << "Publishing message... ";
	try {
		mqtt::topic top(*this->cli, topic, this->QoS);
		mqtt::token_ptr tok;
		/*for (int i = 0; i < messages.size(); i++) { // multiple messages
		tok = top.publish(messages[i]);
		} tok->wait();*/ // Wait until last message is published.
		tok = top.publish(this->clientID + ": " + messages);
		tok->wait(); // Wait until last message is published.
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to publish message" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}