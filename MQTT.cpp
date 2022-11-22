#include "MQTT.h"

std::vector<std::string> MQTT::splitStringBySpace(std::string input) {
	std::stringstream ss(input);
	std::string temp;
	std::vector<std::string> topics;
	while (ss >> temp)
		topics.push_back(temp);
	return topics;
}

void MQTT::publish(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS) {
	MQTTPublish pub(SERVER_ADDRESS, CLIENT_ID, QOS);
	pub.connectToServer();
	system("cls");

	std::string topicInput;
	while (true) {
		std::cout << "Which topic do you wish to publish to? (Multiple topics are split with spaces) (Leave empty to quit publishing.)\n ";
		std::getline(std::cin, topicInput); // get topic(s)
		if (topicInput.length() != 0) {
			// split topics in multiple
			system("cls");
			while (true) {
				std::cout << "What message do you wish to publish? (Leave empty to head back to picking a topic.)\n ";
				std::string messageInput;
				std::getline(std::cin, messageInput);
				if (messageInput.length() != 0)
					for (auto x : splitStringBySpace(topicInput))
						pub.publish(x, messageInput);
				else break;
				system("cls");
			}
		} else break;
		system("cls");
	}
	pub.disconnectFromServer();
}
void MQTT::subscribe(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS) {
	MQTTSubscribe sub(SERVER_ADDRESS, CLIENT_ID, QOS, true);
	sub.connectToServer();

	system("cls");
	// get topic(s)
	std::cout << "Which topic(s) do you wish to subscribe to? (Leave empty to quit subscribing.)" << std::endl;
	std::string topicInput;
	while (topicInput.empty())
		std::getline(std::cin, topicInput); // get topic(s)

	// split topic(s)
	std::vector<std::string> topics;
	for (auto x : splitStringBySpace(topicInput))
		topics.push_back(x);

	// subscribe
	for (auto x : topics)
		sub.subscribe(x);

	// print subscribed to
	std::cout << "Subscribed to ";
	for (int i = 0; i < topics.size(); i++) {
		std::cout << topics[i];
		if (i < topics.size() - 1)
			std::cout << ", ";
	} std::cout << ":       (Press enter to quit subscribing.)\n";

	std::string msg;
	while (std::getline(std::cin, msg))
		if (msg.empty()) break;

	sub.disconnectFromServer();
}
void MQTT::pubsub(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS) {
	MQTTPubSub ps(SERVER_ADDRESS, CLIENT_ID, QOS, true);
	ps.connectToServer();

	system("cls");
	// get topic(s) for publish
	std::cout << "Which topic(s) do you wish to publish to? (Topics are split with spaces)" << std::endl;
	std::string topicInput;
	while (topicInput.empty())
		std::getline(std::cin, topicInput); // get topic(s)

	// split topic(s)
	std::vector<std::string> topics_pub;
	for (auto x : splitStringBySpace(topicInput))
		topics_pub.push_back(x);

	//std::cin.ignore();

	// get topic(s) for subscribe
	std::cout << "Which topic(s) do you wish to subscribe to? (Topics are split with spaces)" << std::endl;
	topicInput = "";
	while (topicInput.empty())
		std::getline(std::cin, topicInput); // get topic(s)

	// split topic(s)
	std::vector<std::string> topics_sub;
	for (auto x : splitStringBySpace(topicInput))
		topics_sub.push_back(x);

	ps.pubsub(topics_sub, topics_pub);
	ps.disconnectFromServer();
}
void MQTT::menu(std::string CLIENT_ID, std::string SERVER_ADDRESS, int QOS) {
	int input = 0;
	while (input != 4) {
		std::cout << "~Welcome to the MQTT Client!~\nWhat do you want to do? Type in the number you want.\n "
			"1. Publish\n 2. Subscribe\n 3. Publish & Subscribe\n 4. Quit\n ";
		std::cin >> input;
		switch (input) {
		case 1:
			std::cin.ignore();
			system("cls");
			publish(CLIENT_ID, SERVER_ADDRESS, QOS);
			system("cls");
			break;
		case 2:
			std::cin.ignore();
			system("cls");
			subscribe(CLIENT_ID, SERVER_ADDRESS, QOS);
			system("cls");
			break;
		case 3:
			std::cin.ignore();
			system("cls");
			pubsub(CLIENT_ID, SERVER_ADDRESS, QOS);
			system("cls");
			break;
		case 4:
			system("cls");
			break;
		default:
			system("cls");
			break;
		}
	}
	std::cout << "\nByeBye!\n";
}