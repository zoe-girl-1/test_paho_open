#include "MQTTSubscribe.h"
//#include <sstream>

MQTTSubscribe::MQTTSubscribe(const std::string serverAddress, const std::string clientID, const int QoS, const bool NO_LOCAL) 
 : serverAddress(serverAddress), clientID(clientID), QoS(QoS), NO_LOCAL(NO_LOCAL) {
	std::cout << "Initialising server host '" << this->serverAddress << "'... ";
	try {
		this->cli = new mqtt::async_client(this->serverAddress, this->clientID, mqtt::create_options(MQTTVERSION_5));
		
		// set a callback for connection lost. This just exits the app.
		this->cli->set_connection_lost_handler([](const std::string&) { // lambda
			std::cout << "*** Connection Lost ***" << std::endl;
			exit(2);
		});
		
		// set the callback for incoming messages
		cli->set_message_callback([](mqtt::const_message_ptr msg) { // lambda
			std::cout << msg->get_payload_str() << std::endl;
		});
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to initialize server host" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
MQTTSubscribe::~MQTTSubscribe() {
	std::cout << " ~DECONSTRUCTOR MQTTSUBSCRIBE " << std::endl;
	delete this->cli;
}

void MQTTSubscribe::connectToServer() {
	std::cout << "Connecting to server... ";
	try {
		// Last Will and Testament setup
		//auto lwt = mqtt::message(this->topicInput, "<<<" + this->clientID + " was disconnected>>>", this->QoS, false);

		// set up the connection options
		mqtt::properties connectProperties{ {mqtt::property::SESSION_EXPIRY_INTERVAL, 604800} };
		auto tempConOpts = mqtt::connect_options_builder().mqtt_version(MQTTVERSION_5).properties(connectProperties)
			.clean_start(true)/*.will(std::move(lwt))*/.keep_alive_interval(std::chrono::seconds(120)).finalize();
		auto tok = cli->connect(tempConOpts);
		tok->wait();
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to connect to server" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
void MQTTSubscribe::disconnectFromServer() {
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

void MQTTSubscribe::subscribe(std::string topic) {
	system("cls");
	std::cout << "Subscribing to topic(s)... ";
	try {
		// subscribe to topic using "no local" so we don't receive our own messages
		auto subscribeOptions = mqtt::subscribe_options(this->NO_LOCAL);
		mqtt::topic top(*this->cli, topic, this->QoS);
		top.subscribe(subscribeOptions)->wait();
		std::cout << "...OK" << std::endl;
		system("cls");
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to subscribe to topic" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}