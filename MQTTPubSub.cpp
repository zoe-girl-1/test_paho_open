#include "MQTTPubSub.h"

MQTTPubSub::MQTTPubSub(const std::string serverAddress, const std::string clientID, const int QoS, const bool NO_LOCAL) 
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
		this->cli->set_message_callback([](mqtt::const_message_ptr msg) { // lambda
			std::cout << msg->get_payload_str() << std::endl;
		});

		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to initialize server host" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
MQTTPubSub::~MQTTPubSub() {
	std::cout << " ~DECONSTRUCTOR MQTTPUBSUB " << std::endl;
	delete this->cli;
}

void MQTTPubSub::connectToServer() {
	std::cout << "Connecting to server... ";
	try {
		// Last Will and Testament setup
		//auto lwt = mqtt::message(this->topicInput, "<<<" + this->clientID + " was disconnected>>>", this->QoS, false);

		// set up the connection options
		mqtt::properties connectProperties{ {mqtt::property::SESSION_EXPIRY_INTERVAL, 604800} };
		auto tempConOpts = mqtt::connect_options_builder().mqtt_version(MQTTVERSION_5).properties(connectProperties)
			.clean_start(true)/*.will(std::move(lwt))*/.keep_alive_interval(std::chrono::seconds(120)).finalize();
		auto tok = this->cli->connect(tempConOpts);
		tok->wait();
		std::cout << "...OK" << std::endl;
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to connect to server" << std::endl;
		std::cerr << e << std::endl;
		// change state..
	}
}
void MQTTPubSub::disconnectFromServer() {
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

int MQTTPubSub::sub(std::string topic) {
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
		return 1;
	}
	return 0;
}
int MQTTPubSub::pub(std::string topic, std::string message) {
	try {
		mqtt::topic top(*this->cli, topic, this->QoS);
		mqtt::token_ptr tok;
		/*for (int i = 0; i < messages.size(); i++) { // multiple messages
		tok = top.publish(messages[i]);
		} tok->wait();*/ // Wait until last message is published.
		tok = top.publish(this->clientID + ": " + message);
		tok->wait(); // Wait until last message is published.
	} catch (const mqtt::exception& e) {
		std::cout << "...Failed to publish to topic" << std::endl;
		std::cerr << e << std::endl;
		// change state..
		return 1;
	}
	return 0;
}
int MQTTPubSub::pubsub(std::vector<std::string> topics_sub, std::vector<std::string> topics_pub) {
	for (auto x : topics_sub) {
		if (sub(x) != 0) {
			std::cout << "ERROR: Could not subscribe to topics.." << std::endl;
			return 1;
		}
	}
	// print subscribed to
	std::cout << "Subscribed to ";
	for (int i = 0; i < topics_sub.size(); i++) {
		std::cout << topics_sub[i];
		if (i < topics_sub.size() - 1)
			std::cout << ", ";
	} std::cout << ", you can publish messages by typing in the console: (Press enter to quit subscribing and publishing.)\n";

	std::string usrMsg;
	while (std::getline(std::cin, usrMsg)) {
		if (!usrMsg.empty()) {
			usrMsg = this->clientID + ": " + usrMsg;
			for (auto x : topics_pub) {
				if (pub(x, usrMsg) != 0) {
					std::cout << "ERROR: Could not publish to topics.." << std::endl;
					return 1;
				}
			}
		} else break;
	}
}