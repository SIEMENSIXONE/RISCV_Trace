//
//  ObjdumpParser.hpp
//  objdump_parser
//
//  Created by Веселяев Михаил on 10.04.2024.
//
//
#ifndef ObjdumpParser_hpp
#define ObjdumpParser_hpp
//
#include <JuceHeader.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>

//
using namespace std;
//
class TObjdumpParser {
    //
public:
    TObjdumpParser();
    ~TObjdumpParser();
    void parseFile(juce::File);
    void print();
    map<string, string> getFirstFuncAddrMap();
    map<string, std::vector<string>> getRetFuncAddrMap();
    map<string, string> getAddrFuncMap();
    map<string, std::vector<string>> getFuncAddrMap();
    map<string, std::vector<string>> getCallersMap();
    map<string, std::set<string>> getCallingMap();
    map<string, pair<string, string>> getAddrCallerCalled();
    //
protected:
    static bool isADigitHex(const char &);
    static string parseFuncName(const string &);
    //
    map<string, string> *FFirstAddrFuncMap;
    map<string, std::vector<string>>* FRetFuncAddrMap;
    map<string, string> *FAddrFuncMap;
    map<string, std::vector<string>>* FFuncAddrMap;
    map<string, std::vector<string>>* FCallersMap; // called - [callers]
    map<string, std::set<string>>* FCallingMap; // caller - [called]
    map<string, pair<string, string>> *FAddrCallerCalled; // call_addr - {caller, called}
};
//
#endif /* ObjdumpParser_hpp */
