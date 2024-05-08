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
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

//
using namespace std;
//
class TObjdumpParser {
    //
public:
    TObjdumpParser();
    ~TObjdumpParser();
    void parseFile(const string &);
    void print();
    map<string, string> getFirstFuncAddrMap();
    map<string, string> getAddrFuncMap();
    map<string, std::vector<string>> getFuncAddrMap();
    map<string, std::vector<string>> getCallersMap();
    map<string, std::vector<string>> getCallingMap();
    map<string, pair<string, string>> getAddrCallerCalled();
    //
protected:
    static bool isADigitHex(const char &);
    static string parseFuncName(const string &);
    //
    map<string, string> *FFirstAddrFuncMap;
    map<string, string> *FAddrFuncMap;
    map<string, std::vector<string>>* FFuncAddrMap;
    map<string, std::vector<string>>* FCallersMap; // called - [callers]
    map<string, std::vector<string>>* FCallingMap; // caller - [called]
    map<string, pair<string, string>> *FAddrCallerCalled; // call_addr - {caller, called}
};
//
#endif /* ObjdumpParser_hpp */
