#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <cctype>  // for std::isdigit
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cmath>



using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::ifstream;
using std::getline;




inline std::string futureTimeFromNow(int secondsToAdd) {
    using namespace std::chrono;

    // Get current system time and add seconds
    system_clock::time_point now = system_clock::now();
    system_clock::time_point future = now + seconds(secondsToAdd);

    // Convert to time_t for formatting
    std::time_t future_time = system_clock::to_time_t(future);
    std::tm* tm_ptr = std::localtime(&future_time);

    // Format as yyyy:mm:dd:hh:mm:ss
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y:%m:%d:%H:%M:%S");

    return oss.str();
}


string stripToNumbersAndColons(const string& input);

class Cards {
public:
    vector<string> stream;

    string Id() const { return stream[0]; }
    string question() const { return stream[1]; }
    string answer() const { return stream[2]; }
    string description() const { return stream[3]; }
    string level() const { return stream[4]; }
    string time() const { return stripToNumbersAndColons(stream[5]); }
    string rawTime() const { return stream[5]; }  // for saving
};



vector<string> splitString(const string& str, char delimiter);

inline vector<Cards> load_cards(string FileToRead) {
    ifstream MyFile(FileToRead);
    vector<Cards> CardsArray;
    string line;
    Cards y;

    while (getline(MyFile, line)) {
        y.stream = splitString(line, '\t');
        CardsArray.push_back(y);
    }

    MyFile.close();
    return CardsArray;
}


inline std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_ptr = std::localtime(&current_time);

    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y:%m:%d:%H:%M:%S");
    return oss.str();
}


inline std::pair<int, Cards> getCurrentCardAndIndex(const vector<Cards>& CardsArray) {
    int currentIndex = -1;
    std::string minTime = "9999:00:00:00:00:00";
    Cards currentCard;

    std::string nowTime = getCurrentTimeString();

    for (size_t i = 0; i < CardsArray.size(); i++) {
        std::string cardTime = CardsArray[i].time();
        if (cardTime != "0" && cardTime < nowTime && cardTime < minTime) {
            minTime = cardTime;
            currentIndex = static_cast<int>(i);
            currentCard = CardsArray[i];
        }
    }
if (currentIndex == -1) {
    for (size_t i = 0; i < CardsArray.size(); ++i) {
        if (CardsArray[i].time() == "0") {
            currentIndex = static_cast<int>(i);
            currentCard = CardsArray[i];
            break;
        }
    }
}

    return {currentIndex, currentCard};
}


inline void replaceCardsArray(vector<Cards>& CardsArray, int index, bool correct) {
    if (index < 0 || index >= static_cast<int>(CardsArray.size())) return;

    Cards card = CardsArray[index];

    // Get and update level
    int level = std::stoi(card.stream[4]);
    if (correct) {
        level++;
    } else if (level > 0) {
        level = 0;
    }
    card.stream[4] = std::to_string(level);

    // Compute next time using your formula
    int seconds = static_cast<int>(std::pow(2.6, level) * 60);
    card.stream[5] = futureTimeFromNow(seconds);  // Update time directly

    CardsArray[index] = card;
}



inline string stripToNumbersAndColons(const string& input) {
    string result;
    for (char c : input) {
        if (std::isdigit(static_cast<unsigned char>(c)) || c == ':') {
            result += c;
        }
    }
    return result;
}



inline vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    std::istringstream iss(str);
    string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}



inline void saveCards(const string& FileToRead, const vector<Cards>& CardsArray) {
    std::ofstream MyFile(FileToRead);
    if (!MyFile) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    for (const auto& card : CardsArray) {
        string lineToSave = card.Id() + '\t' + card.question() + '\t' +
                            card.answer() + '\t' + card.description() + '\t' +
                            card.level() + '\t' + card.rawTime() + '\n';
        MyFile << lineToSave;
    }
}
