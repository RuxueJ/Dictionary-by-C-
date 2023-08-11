#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <cctype>
#include <algorithm>
#include <functional>


using namespace std;

struct DictionaryOutPut
{
	string keyword;
	map<string, vector<string>> innerMap;

};
const string HELP_INFO = "\t|\n\t PARAMETER HOW-TO, please enter:\n"
"\t 1.A search key -then 2. An optional part of speech -then\n"
"\t 3.An optiona 'distinct' -then 4. An optional 'reverse'\n\t|\n";

const string NOT_FOUND_INFO = "\t|\n \t<NOT FOUND> To be considered for the next release. Thank you.\n\t|\n";

const set<string> SPEECHSET = { "noun","adjective","adverb","verb","pronoun","preposition","conjunction","interjection" };

unordered_map <string, map<string, vector<string>>> loadData();
void showMenu(const unordered_map <string, map<string, vector<string>>>&);
void run(const unordered_map <string, map<string, vector<string>>>&);

bool checkFirstToken(const unordered_map <string, map<string, vector<string>>>&, string&);
bool checkSecondToken(const unordered_map <string, map<string, vector<string>>>&, string&, string&);
bool checkThirdToken(string&);
bool checkFourthToken(string&);

DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>&, string&);
DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>&, string&, string&);
DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>&, string&, string&, string&);
DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>&, string&, string&, string&, string&);

void Reverse(DictionaryOutPut&);
void Distinct(DictionaryOutPut&);

string toLowercase(string&);
void printDefinition(DictionaryOutPut&);
void printReverseDefinition(DictionaryOutPut& output);
string trim(const string& str);
bool startWithCsc(const string&);
void transferKey(string&);


int main() {

	unordered_map <string, map<string, vector<string>>> outerMap = loadData();
	showMenu(outerMap);
	run(outerMap);

	return 0;
}


unordered_map <string, map<string, vector<string>>> loadData() {
	cout << "! Opening data file...";

	string filePath;
	cin >> filePath;

	fstream ioFile;
	ioFile.open(filePath, ios::in);
	while (!ioFile.is_open()) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "<!>ERROR<!> ===> File could not be opened." << endl;
		cout << "<!>ERROR<!> ===> Provided file path:" << filePath << endl;
		cout << "<!>Enter the CORRECT data file path:";
		cin >> filePath;
		ioFile.open(filePath, ios::in);
	}



	ifstream inputFile(filePath);
	//while (!inputFile.is_open()) {
	//	cin.clear();
	//	cin.ignore(1000, '\n');
	//	cout << "<!>ERROR<!> ===> File could not be opened." << endl;
	//	cout << "<!>ERROR<!> ===> Provided file path:" << filePath << endl;
	//	cout << "<!>Enter the CORRECT data file path:";
	//	cin >> filePath;
	//	ifstream inputFile(filePath);
	//}



	cin.clear();
	cin.ignore(1000, '\n');
	unordered_map <string, map<string, vector<string>>> outerMap;


	cout << "! Loading data..." << endl;
	string line;

	while (getline(inputFile, line)) {

		string token;
		vector<string> tokens;
		istringstream iss(line);
		string delimiter = "-=>>";


		while (getline(iss, token, '|')) {
			tokens.push_back(token);
		}

		// set keyword
		string keyword = tokens[0];


		for (int i = 1; i < tokens.size(); i++) {

			string data = tokens[i];

			size_t delimiterPos = data.find(delimiter);

			if (delimiterPos != string::npos) {
				string speech = data.substr(0, delimiterPos);
				speech = trim(speech);
				string definition = data.substr(delimiterPos + delimiter.length());
				definition = trim(definition);
				definition[0] = toupper(definition[0]);

				outerMap[keyword][speech].push_back(definition);
				sort(outerMap[keyword][speech].begin(), outerMap[keyword][speech].end());
			}

		}


	}


	for (auto& outerPair : outerMap) {

		for (auto& innerPair : outerPair.second) {

			sort(innerPair.second.begin(), innerPair.second.end());
		}
	}


	cout << "! Loading completed..." << endl;

	/*for (const auto& outerPair : outerMap) {
		std::cout << "Keyword: " << outerPair.first <<":" << outerPair.first.length() << std::endl;

		for (const auto& innerPair : outerPair.second) {
			std::cout << "  Speech: " << innerPair.first <<":" << innerPair.first.length() <<  std::endl;

			for (const auto& definition : innerPair.second) {
				std::cout << "    Definition: " << definition << std::endl;
			}
		}
	}*/

	inputFile.close();
	cout << "! Closing data file..." << endl;

	return outerMap;
}

void showMenu(const unordered_map <string, map<string, vector<string>>>& outerMap) {

	size_t keyNumber = outerMap.size();

	size_t defiNumber = 0;

	for (const auto& outerPair : outerMap) {
		string keyword = outerPair.first;

		const auto& innerMap = outerPair.second;
		for (const auto& innerPair : innerMap) {
			const auto& definitionSet = innerPair.second;
			/*	cout  <<keyword << ": definition number: " << definitionSet.size() << endl;*/
			defiNumber += definitionSet.size();
		}
	}

	cout << "====== DICTIONARY 340 C++ =====" << endl;

	cout << "------ Keywords: " << keyNumber << endl;
	cout << "------ Definitions: " << defiNumber << endl;
}

void run(const unordered_map <string, map<string, vector<string>>>& outerMap) {
	bool flag = true;
	int count = 1;
	while (flag) {
		std::cout << "Search [" << count << "]: ";
		string line;
		getline(cin, line);


		if (line.empty()) {
			std::cout << HELP_INFO;
		}
		else {
			istringstream iss(line);
			vector<string> tokens;
			string token;
			while (iss >> token) {
				tokens.push_back(token);
			}
			DictionaryOutPut output;
			int numberTokens = tokens.size();
			string firstToken = toLowercase(tokens[0]);
			string secondToken;
			string thirdToken;
			string fourthToken;
			switch (numberTokens) {
			case 1:
				if (firstToken == "!help") {
					cout << HELP_INFO << endl;
				}
				else if (firstToken == "!q") {
					cout << "-----THANK YOU-----" << endl;
					flag = false;
				}
				else {
					if (checkFirstToken(outerMap, firstToken)) {
						output = searchGo(outerMap, firstToken);
						printDefinition(output);
					}
				}
				break;

			case 2:
				secondToken = toLowercase(tokens[1]);

				if (!checkSecondToken(outerMap, firstToken, secondToken)) {
					cout << NOT_FOUND_INFO << endl;
					cout << HELP_INFO << endl;
					break;
				}
				output = searchGo(outerMap, firstToken, secondToken);
				if (secondToken == "reverse") {
					printReverseDefinition(output);
				}
				else {
					printDefinition(output);
				}


				break;

			case 3:
				secondToken = toLowercase(tokens[1]);
				thirdToken = toLowercase(tokens[2]);
				output = searchGo(outerMap, firstToken, secondToken, thirdToken);
				if (thirdToken == "reverse") {
					printReverseDefinition(output);
				}
				else {
					printDefinition(output);
				}
				break;

			case 4:
				secondToken = toLowercase(tokens[1]);
				thirdToken = toLowercase(tokens[2]);
				fourthToken = toLowercase(tokens[3]);
				output = searchGo(outerMap, firstToken, secondToken, thirdToken, fourthToken);
				if (fourthToken == "reverse" || secondToken == "reverse" || thirdToken == "reverse") {
					printReverseDefinition(output);
				}
				else {
					printDefinition(output);
				}
				break;

			default:
				cout << HELP_INFO << endl;

			}

		}

		count++;
	}
}

string toLowercase(string& str) {

	for (size_t i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

bool checkFirstToken(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken) {
	bool firstTokenFlag = false;

	auto it = outerMap.find(firstToken);
	if (it != outerMap.end()) {
		firstTokenFlag = true;
	}
	else {
		cout << NOT_FOUND_INFO;
		cout << HELP_INFO;
	}
	return firstTokenFlag;
}

bool checkSecondToken(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken, string& secondToken) {
	bool secondTokenFlag = true;

	if (SPEECHSET.count(secondToken) == 1) {

		auto it = outerMap.find(firstToken);
		if (it != outerMap.end()) {
			map<string, vector<string>> innerMap = it->second;



			if (innerMap.count(secondToken) == 0) {
				secondTokenFlag = false;

			}
		}

	}
	return secondTokenFlag;
}

bool checkThirdToken(string& thirdToken) {

	bool thirdTokenFlag;
	if (thirdToken == "reverse") {
		thirdTokenFlag = true;
	}
	else if (thirdToken == "distinct") {
		thirdTokenFlag = true;
	}
	else
	{
		thirdTokenFlag = false;
		cout << "\t|\n\t <The entered 3rd parameter '" << thirdToken << "' is NOT 'distinct'. >\n" <<
			"\t <The entered 3rd parameter '" << thirdToken << "' is NOT 'reverse'.>\n" <<
			"\t <The entered 3rd parameter '" << thirdToken << "' was disregarded.>\n" <<
			"\t <The 3rd parameter should be 'distinct' or 'reverse'.>\n\t|" << endl;
	}

	return thirdTokenFlag;
}

bool checkFourthToken(string& fourthToken) {
	bool fourthTokenFlag;
	if (fourthToken == "reverse") {
		fourthTokenFlag = true;
	}

	else
	{
		fourthTokenFlag = false;
		cout << "\t|\n\t <The entered 4th parameter '" << fourthToken << "' is NOT 'reverse'.>\n" <<
			"\t <The entered 4th parameter '" << fourthToken << "' was disregarded.>\n" <<
			"\t <The 4th parameter should be 'reverse'.>\n\t|" << endl;
	}

	return fourthTokenFlag;
}

DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken) {
	DictionaryOutPut output;
	output.keyword = firstToken;

	auto it = outerMap.find(firstToken);
	if (it != outerMap.end()) {
		output.innerMap = it->second;
	}

	return output;
}

DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken, string& secondToken) {
	DictionaryOutPut output = searchGo(outerMap, firstToken);
	string keyword = output.keyword;
	map<string, vector<string>>& innerMap = output.innerMap;
	//for (const auto& pair : innerMap) {
	//	std::cout << "Key: " << pair.first << std::endl;
	//	std::cout << "Values: ";
	//	for (const auto& value : pair.second) {
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;
	//}

	if (secondToken == "reverse") {
		Reverse(output);
	}
	else if (secondToken == "distinct") {
		Distinct(output);
	}
	else if (SPEECHSET.count(secondToken) == 0) {
		cout << "\t|\n\t <The entered 2nd parameter '" << secondToken << "' is NOT a part of speech.>\n" <<
			"\t <The entered 2nd parameter '" << secondToken << "' is NOT 'distinct'. > \n" <<
			"\t <The entered 2nd parameter '" << secondToken << "' is NOT 'reverse'.>\n" <<
			"\t <The entered 2nd parameter '" << secondToken << "' was disregarded.>\n" <<
			"\t <The 2nd parameter should be a part of speech or 'distinct' or 'reverse'.>\n\t|" << endl;
	}
	else {
		auto foundSpeech = innerMap.find(secondToken);
		innerMap.erase(innerMap.begin(), foundSpeech);
		innerMap.erase(next(foundSpeech, 1), innerMap.end());
	}
	//for (const auto& pair : innerMap) {
	//	std::cout << "Key: " << pair.first << std::endl;
	//	std::cout << "Values: ";
	//	for (const auto& value : pair.second) {
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;
	//}


	return output;

}

DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken, string& secondToken, string& thirdToken) {
	DictionaryOutPut output = searchGo(outerMap, firstToken, secondToken);
	if (checkThirdToken(thirdToken)) {
		if (thirdToken == "reverse") {
			Reverse(output);
		}
		else if (thirdToken == "distinct") {
			Distinct(output);
		}
	}

	return output;

}

DictionaryOutPut searchGo(const unordered_map <string, map<string, vector<string>>>& outerMap, string& firstToken, string& secondToken, string& thirdToken, string& fourthToken) {
	DictionaryOutPut output = searchGo(outerMap, firstToken, secondToken, thirdToken);

	if (checkFourthToken(fourthToken)) {
		if (fourthToken == "reverse") {
			Reverse(output);
		}

	}

	return output;




}

void Reverse(DictionaryOutPut& output) {

	map<string, vector<string>>& innerMap = output.innerMap;

	//cout << "before reverse" << endl;
	//for (const auto& pair : innerMap) {
	//	std::cout << "Key: " << pair.first << std::endl;
	//	std::cout << "Values: ";
	//	for (const auto& value : pair.second) {
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;
	//}



	for (auto it = innerMap.rbegin(); it != innerMap.rend(); ++it) {
		const string& speech = it->first;
		vector<string>& definitions = it->second;
		sort(definitions.rbegin(), definitions.rend());

	}



	//cout << "after reverse" << endl;

	//for (const auto& pair : innerMap) {
	//	std::cout << "Key: " << pair.first << std::endl;
	//	std::cout << "Values: ";
	//	for (const auto& value : pair.second) {
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;
	//}

}

void Distinct(DictionaryOutPut& output) {
	string keyword = output.keyword;
	map<string, vector<string>>& innerMap = output.innerMap;

	for (auto& entry : innerMap) {
		vector<string> uniqueDefinitions;
		set<string> tempSet(entry.second.begin(), entry.second.end()); // Create a temporary set with unique elements

		// Populate the vector with the unique elements
		for (const auto& element : tempSet) {
			uniqueDefinitions.push_back(element);
		}

		entry.second = uniqueDefinitions;
	}

}

void printDefinition(DictionaryOutPut& output) {

	string& keyword = output.keyword;

	transferKey(keyword);
	map<string, vector<string>> innerMap = output.innerMap;
	cout << "\t|" << endl;

	for (const auto& entry : innerMap) {
		const string& speech = entry.first;
		vector<string> definitions = entry.second;

		for (string& element : definitions) {
			cout << "\t " << output.keyword << " [" << speech << "] : " << element << endl;
		}
	}
	cout << "\t|" << endl;
}

void printReverseDefinition(DictionaryOutPut& output) {
	string& keyword = output.keyword;

	transferKey(keyword);

	if (startWithCsc(keyword)) {
		for (int i = 0; i < 3; i++) {
			keyword[i] = toupper(keyword[i]);
		}
	}
	else {
		keyword[0] = toupper(output.keyword[0]);
	}


	map<string, vector<string>> innerMap = output.innerMap;

	map<string, vector<string>, greater<string>> reversedMap(innerMap.begin(), innerMap.end());


	cout << "\t|" << endl;

	for (const auto& entry : reversedMap) {
		const string& speech = entry.first;
		vector<string> definitions = entry.second;

		for (string& element : definitions) {
			cout << "\t " << output.keyword << " [" << speech << "] : " << element << endl;
		}
	}
	cout << "\t|" << endl;
}

string trim(const string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos) {
		// The string contains only whitespaces
		return "";
	}

	return str.substr(start, end - start + 1);
}

bool startWithCsc(const string& str) {
	if (str.length() < 3) {
		return false;
	}
	return str.substr(0, 3) == "csc";
}

void transferKey(string& keyword) {


	if (startWithCsc(keyword)) {
		for (int i = 0; i < 3; i++) {
			keyword[i] = toupper(keyword[i]);
		}
	}
	else {
		keyword[0] = toupper(keyword[0]);
	}
}