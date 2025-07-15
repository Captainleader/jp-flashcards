#ifndef ROMAJI_CONVERTER_HPP
#define ROMAJI_CONVERTER_HPP

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;


inline string romaji_to_hiragana(string text){
    string result = "";
    unordered_map<string,string> RomajiHiragana ={

// 3 letter syllable

    {"kyu", "きゅ"}, {"kya", "きゃ"}, {"kyo", "きょ"},{"cyu", "きゅ"}, {"cya", "きゃ"}, {"cyo", "きょ"},
     {"gyu", "ぎゅ"}, {"gya", "ぎゃ"}, {"gyo", "ぎょ"}, 
    {"syu", "しゅ"}, {"sya", "しゃ"}, {"syo", "しょ"}, {"jyu", "じゅ"}, {"jya", "じゃ"}, {"jyo", "じょ"}, 
    {"shu", "しゅ"}, {"sha", "しゃ"}, {"sho", "しょ"}, {"ju", "じゅ"}, {"ja", "じゃ"}, {"jo", "じょ"}, 
    {"tyu", "ちゅ"}, {"tya", "ちゃ"}, {"tyo", "ちょ"}, {"dya", "ぢゃ"}, {"dyu", "ぢゅ"}, {"dyo", "ぢょ"}, 
    {"hyu", "ひゅ"}, {"hya", "ひゃ"}, {"hyo", "ひょ"}, {"byu", "びゅ"}, {"bya", "びゃ"}, {"byo", "びょ"}, 
    {"pyu", "ぴゅ"}, {"pya", "ぴゃ"}, {"pyo", "ぴょ"}, {"myu", "みゅ"}, {"mya", "みゃ"}, {"myo", "みょ"}, 
    {"nyu", "にゅ"}, {"nyo", "にょ"}, {"nya", "にゃ"}, {"ryu", "りゅ"}, {"rya", "りゃ"}, {"ryo", "りょ"},
    {"shi", "し"},{"tsu", "つ"},{"chu", "ちゅ"}, {"cha", "ちゃ"}, {"cho", "ちょ"},{"ltu", "っ"},

    // 2 letter syllable

    {"ka", "か"}, {"ki", "き"}, {"ku", "く"}, {"ke", "け"}, {"ko", "こ"},
    {"ca", "か"}, {"ci", "き"}, {"cu", "く"}, {"ce", "け"}, {"co", "こ"}, 
    {"sa", "さ"}, {"si", "し"}, {"su", "す"}, {"se", "せ"}, {"so", "そ"}, 
    {"ta", "た"}, {"ti", "ち"}, {"tu", "つ"}, {"te", "て"}, {"to", "と"}, 
    {"na", "な"}, {"ni", "に"}, {"nu", "ぬ"}, {"ne", "ね"}, {"no", "の"}, 
    {"ha", "は"}, {"hi", "ひ"}, {"hu", "ふ"}, {"he", "へ"}, {"ho", "ほ"}, 
    {"ma", "ま"}, {"mi", "み"}, {"mu", "む"}, {"me", "め"}, {"mo", "も"}, 
    {"ra", "ら"}, {"ri", "り"}, {"ru", "る"}, {"re", "れ"}, {"ro", "ろ"}, 
    {"ga", "が"}, {"gi", "ぎ"}, {"gu", "ぐ"}, {"ge", "げ"}, {"go", "ご"}, 
    {"za", "ざ"}, {"zi", "じ"}, {"zu", "ず"}, {"ze", "ぜ"}, {"zo", "ぞ"}, 
    {"da", "だ"}, {"di", "ぢ"}, {"du", "づ"}, {"de", "で"}, {"do", "ど"}, 
    {"ba", "ば"}, {"bi", "び"}, {"bu", "ぶ"}, {"be", "べ"}, {"bo", "ぼ"}, 
    {"pa", "ぱ"}, {"pi", "ぴ"}, {"pu", "ぷ"}, {"pe", "ぺ"}, {"po", "ぽ"}, 
    {"wa", "わ"}, {"wo", "を"}, {"ya", "や"}, {"yu", "ゆ"}, {"yo", "よ"}, 
    {"ji", "じ"}, {"fu", "ふ"}, {"nn", "ん"},
    {"je", "じぇ"}, {"fi", "ふぃ"}, {"fe", "ふぇ"}, {"fo", "ふぉ"}, {"fa", "ふぁ"},

    // 1 letter syllable

    {"a", "あ"}, {"i", "い"}, {"u", "う"}, {"e", "え"}, {"o", "お"},{"n", "ん"},{"m", "ん"},{ "-","ー"},
 };

    while (!text.empty()) {
        if (text.length() > 1 && text[0] == text[1] && std::string("naiueo-").find(text[0]) == std::string::npos) {
            result += "っ";
            text = text.substr(1);   
        }

        if (text.length() > 2 && RomajiHiragana.find(text.substr(0,3)) != RomajiHiragana.end()) {
            result += RomajiHiragana[text.substr(0,3)];
            text = text.substr(3);
        }else if (text.length() > 1 &&  RomajiHiragana.find(text.substr(0,2)) != RomajiHiragana.end()) {
            result += RomajiHiragana[text.substr(0,2)];
            text = text.substr(2);
        }
        else if (RomajiHiragana.find(text.substr(0,1)) != RomajiHiragana.end()) {
            result += RomajiHiragana[text.substr(0,1)];
            text = text.substr(1);
        }
        else {
            result += text.substr(0,1);
            text = text.substr(1);
        }
    }
    return result;
}

#endif