//
//  ObjdumpParser.cpp
//  objdump_parser
//
//  Created by Веселяев Михаил on 10.04.2024.
//
//
#include "ObjdumpParser.hpp"
//
TObjdumpParser::TObjdumpParser(){
    FFirstAddrFuncMap = new map<string, string>();
    FAddrFuncMap = new map<string, string>();
    FFuncAddrMap = new map<string, vector<string>>();
    FCallersMap = new map<string, vector<string>>();
    FCallingMap = new map<string, vector<string>>();
    FAddrCallerCalled = new map<string, pair<string, string>>();
}
//
TObjdumpParser::~TObjdumpParser(){
    delete(FFirstAddrFuncMap);
    delete(FAddrFuncMap);
    delete(FFuncAddrMap);
    delete(FCallersMap);
    delete(FCallingMap);
    delete(FAddrCallerCalled);
}
//
void TObjdumpParser::parseFile(const string &filename){
    string result;
    string buf;
    set<string> funcNames;
    ifstream fin(filename);
    //
    if(!fin.is_open())
    {
        //TODO: Exception
        //cout<< "-----------------------------------" <<endl;
        //cout<<"Error! Failed to open file."<<endl;
        //cout<< "-----------------------------------" <<endl;
        return;
    }
    //
    string curFuncName = "";
    while (getline(fin, buf)) {
        string addr;
        bool flag = true;
        //
        for (int i = 0; i < 16; i++) {
            char c = buf[i];
            //
            if (!(isADigitHex(c))) {
                flag = false;
                break;
            }
            addr += c;
        }
        //
        if (flag) {
            curFuncName = parseFuncName(buf);
            FFirstAddrFuncMap->insert({ addr, curFuncName });
            funcNames.insert(curFuncName);
        }
    }
    //
    fin.clear();
    fin.seekg(0);
    curFuncName = "";
    while (getline(fin, buf)){
        string addr;
        //
        if (buf.length() > 12) {
            if ((buf[0] == ' ') && (buf[1] == ' ') && (buf[2] == ' ') && (buf[3] == ' ')) {
                for (int i = 4; i < 12; i++) {
                    char c = buf[i];
                    if (!(isADigitHex(c))) break;
                    addr += c;
                }
                //
                string tmp = "00000000" + addr;
                if ((curFuncName != "") && (tmp.length() == 16)) {
                    FAddrFuncMap->insert({ tmp, curFuncName });
                    //
                    if (FFuncAddrMap->find(curFuncName) == FFuncAddrMap->end()) {
                        vector<string> vecTmp;
                        vecTmp.push_back(tmp);
                        FFuncAddrMap->insert({ curFuncName, vecTmp });
                    }
                    else {
                        (FFuncAddrMap->at(curFuncName)).push_back(tmp);
                    }
                    //
                    string calledFunc = parseFuncName(buf);
                    //
                    if (calledFunc != "") {
                        //
                        if (FCallersMap->find(calledFunc) == FCallersMap->end()) {
                            vector<string> tmpVec;
                            tmpVec.push_back(curFuncName);
                            FCallersMap->insert({ calledFunc, tmpVec });
                        }
                        else {
                            (FCallersMap->at(calledFunc)).push_back(curFuncName);
                        }
                        //
                        if (FCallingMap->find(curFuncName) == FCallingMap->end()) {
                            vector<string> tmpVec;
                            tmpVec.push_back(calledFunc);
                            FCallingMap->insert({ curFuncName, tmpVec });
                        }
                        else {
                            (FCallingMap->at(curFuncName)).push_back(calledFunc);
                        }
                        //
                        if (funcNames.find(calledFunc) != funcNames.end()) FAddrCallerCalled->insert({tmp, make_pair(curFuncName, calledFunc)});
                    }
                }

            }
        }
        //
        bool flag = true;
        //
        for(int i = 0; i < 16; i++){
            char c = buf[i];
            //
            if (!(isADigitHex(c))) {
                flag = false;
                break;
            }
            addr+=c;
        }
        //
        if (flag) { 
            curFuncName = parseFuncName(buf);
        }
    }
    fin.close();
}
//
void TObjdumpParser::print(){
    for( auto it = FFirstAddrFuncMap->begin(); it != FFirstAddrFuncMap->end(); ++it )
    {
        cout << it->first;
        string& value = it->second;
        cout << " " << value << endl;
    }
}
//
map<string, string> TObjdumpParser::getFirstFuncAddrMap(){
    return *FFirstAddrFuncMap;
}
//
map<string, string> TObjdumpParser::getAddrFuncMap() {
    return *FAddrFuncMap;
}
//
map<string, std::vector<string>> TObjdumpParser::getFuncAddrMap() {
    return *FFuncAddrMap;
}
//
map<string, std::vector<string>> TObjdumpParser::getCallersMap() {
    return *FCallersMap;
}
map<string, std::vector<string>> TObjdumpParser::getCallingMap() {
    return *FCallingMap;
}
//
map<string, pair<string, string>> TObjdumpParser::getAddrCallerCalled() {
    return *FAddrCallerCalled;
}
//
bool TObjdumpParser::isADigitHex(const char &c){
    char numbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    //
    for (int i = 0; i < sizeof(numbers); i++){
        if (c == numbers[i]) return true;
    }
    //
    return false;
}
//
string TObjdumpParser::parseFuncName(const string &str){
    int openingBracketPos = -1;
    int closingBracketPos = -1;
    //
    for (int i = 0; i < str.length(); i++){
        char c = str[i];
        if (c == '<') openingBracketPos = i;
        if (c == '>') closingBracketPos = i;
    }
    //
    if ((openingBracketPos < 0) || (closingBracketPos < 0) || ((closingBracketPos - openingBracketPos) <= 1)) {
        //TODO: Exception
        //cout<< "-----------------------------------" <<endl;
        //cout<<"Error! Not a func name."<<endl;
        //cout<< "-----------------------------------" <<endl;
        return "";
    }
    //
    if ((str[openingBracketPos + 1] == 'm') && (str[openingBracketPos + 2] == 'a') && (str[openingBracketPos + 3] == 'i') && (str[openingBracketPos + 4] == 'n')) return "main";
    //
//    if ((str[openingBracketPos + 1] != '_') || (str[openingBracketPos + 2] != 'Z') || (!isANumber(str[openingBracketPos + 3]))){
//        //TODO: Exception
//        cout<< "-----------------------------------" <<endl;
//        cout<<"Error! Wrong func name format."<<endl;
//        cout<< "-----------------------------------" <<endl;
//        return "Error! Wrong func name format.";
//    }
    //
    string name;
//    int nameLength = str[openingBracketPos + 3] - '0';
//    for (int i = openingBracketPos + 4; i < nameLength + openingBracketPos + 4; i++){
//        name+= str[i];
//    }
    for (int i = openingBracketPos + 1; i < closingBracketPos; i++){
        char c = str[i];
        //
        if (c == '+') return "";
        if (c == '-') return "";
        if (c == '.') return "";
        //
        name+=c;
    }
    //
    return name;
}
