#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using string = std::string;
using json = nlohmann::json;

void strToLower(string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char character) {return tolower(character);});
}

string collectString(bool lower) {
	std::string input;
	std::getline(std::cin, input); 
	if (lower)
		strToLower(input);
	return input;
}

int main(int argc, char* argv[]) {
	std::cout << "Say 'help' for help (changing webhooks, username, etc.)" << std::endl;
	std::cout << "Enter webhook URL" << std::endl;
	string input;
	string webhook;
	json requestBody = {
		{"content", ""}
	};

	input = collectString(false);
	webhook = input;

	while (true) {
		std::cout << "Enter message:" << std::endl;
		input = collectString(true);

		if (input == "exit")
			break;
		else if (input == "help") {
			std::cout << "'help' for this message" << std::endl;
			std::cout << "'exit' to exit" << std::endl;
			std::cout << "'change' to change webhook" << std::endl;
			std::cout << "'username' to edit the username seen when the webhook is sent (doesn't reset)" << std::endl;
		}
		else if (input == "change") {
			std::cout << "Enter new webhook URL:" << std::endl;
			input = collectString(false); webhook = input;
			std::cout << "Changed!" << std::endl;
			continue;
		}
		else if (input == "username") {
			std::cout << "Enter new username (press enter to reset):" << std::endl;
			input = collectString(false); if (input == "") requestBody.erase("username"); else requestBody["username"] = input;
			std::cout << "Changed!" << std::endl;
			continue;
		}

		requestBody["content"] = input;

		cpr::Response response = cpr::Post(
			cpr::Url{ webhook },
			cpr::Body{ requestBody.dump() },
			cpr::Header{ {"Content-Type", "application/json"} }
		);
		
		std::cout << "Returned status code: " << response.status_code << std::endl;
	}

	return 0;
}
