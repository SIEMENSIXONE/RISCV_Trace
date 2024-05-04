//
//  CodeParser.cpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 23.04.2024.
//
//
#include "CodeParser.hpp"
//
TCodeParser::TCodeParser() {}
//
TCodeParser::~TCodeParser() {}
//
TCodeParser::CCode TCodeParser::getCodeFromFile(const string &code, map<string, string>& funcAddrMap) {
    CCode result;
    std::vector<string> funcNameVector;
    //
    for (map<string, string>::iterator it = funcAddrMap.begin(); it != funcAddrMap.end(); it++) funcNameVector.push_back(it->second);
    //
    string buf;
    std::stringstream fin(code);
    //
    int symbNum = 0;
    int lineNum = 0;
    while (getline(fin, buf, ';')) { // читаем до ';'
        for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
            string funcName = *it;
            //
            if (result.functions.find(funcName) == result.functions.end()) {
                int findResult = buf.find(funcName);
                if (findResult != -1) {
                    //нашли название функции в строке
                    int ctr = findResult + funcName.length();
                    //
                    while (ctr < buf.length()) {
                        if (buf[ctr] == ';') break; // сюда вроде не попадаем в любом случае
                        if (buf[ctr] == '{') {
                            //нашли! записываем
                            int lineNumInside = 0;
                            for (int i = 0; i < findResult; i++) {
                                if (buf[i] == '\n') lineNumInside++;
                            }
                            //
                            int funcLineNum = lineNum + lineNumInside; // нашли номер строки
                            int funcSymbPos = symbNum + findResult; // нашли номер символа
                            //
                            pair<int, int> tmp = make_pair(funcSymbPos, funcLineNum);
                            result.functions.insert({ funcName, tmp });
                            break;
                        }
                        ctr++;
                    }
                }
            }
        }
        symbNum += buf.length() + 1;
        lineNum +=count(buf.begin(), buf.end(), '\n');
    }
    return result;
}
