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
    std::stringstream fin(clearCommentsStringsSlashes(code));
    //
    int symbNum = 0;
    int lineNum = 0;
    while (getline(fin, buf, '}')) { // читаем до
        for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
            string funcName = *it;
            //
            if (result.functions.find(funcName) == result.functions.end()) {
                int findResult = (int) buf.find(funcName);
                if (findResult != -1) {
                    //нашли название функции в строке
                    int ctr = findResult + (int) funcName.length();
                    //
                    bool ignoreNextSemiColon = false;
                    int inBrackets = 0;
                    //
                    while (ctr < buf.length()) {
                        //
                        if (buf[ctr] == '(') inBrackets++;
                        if (buf[ctr] == ')') inBrackets--;
                        //
                        if ((buf[ctr] != ' ') && (buf[ctr] != '\n') && (buf[ctr] != ';') && (buf[ctr] != '\t') && (buf[ctr] != '(') && (buf[ctr] != ')') && (inBrackets == 0)) {
                            ignoreNextSemiColon = true;
                        }
                        //
                        if (buf[ctr] == ';') {
                            if (!ignoreNextSemiColon) {
                                break;
                            }
                            else {
                                ignoreNextSemiColon = false;
                            }
                        }
                        //
                        if ((buf[ctr] == '{') && (inBrackets == 0)) {
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
        symbNum += (int) buf.length() + 1;
        lineNum += (int) count(buf.begin(), buf.end(), '\n');
    }
    return result;
}
//
string TCodeParser::clearCommentsStringsSlashes(const string& code) {
    string result = code;
    //
    bool metSlash = false;
    bool inComment = false;
    bool inLongComment = false;
    bool inString = false;
    bool metStar = false;
    bool metEscapeCharacter = false;
    //
    for (int i = 0; i < code.length(); i++) {
        //
        if (code[i] == '\\') { 
            metEscapeCharacter = true; 
            result[i] = ' ';
            continue;
        }
        //
        if (code[i] == '#') {
            inComment = true;
        }
        //
        if (code[i] == '*') {
            if (metSlash) {
                inLongComment = true;
                result[i - 1] = ' ';
            }
            metStar = true;
        }
        //
        if (code[i] == '/') {
            if (metSlash) {
                inComment = true;
                result[i - 1] = ' ';
            }
            //
            if (metStar) {
                inLongComment = false;
            }
            //
            metSlash = true;
            result[i] = ' '; //!
        }

        if (code[i] != '/') metSlash = false;
        if (code[i] != '*') metStar = false;
        //
        if (code[i] == '\n') {
            inComment = false;
            continue;
        }
        //
        if ((inComment) || (inLongComment)) {
            result[i] = ' ';
        }
        else {
            if (code[i] == '"') {
                if (!metEscapeCharacter) {
                    if (inString) {
                        inString = false;
                    }
                    else {
                        inString = true;
                    }
                }
            }
            //
            if (inString) {
                result[i] = ' ';
            }
        }
        metEscapeCharacter = false;
    }
    //
    //std::ofstream file;
    //file.open("test123.txt", std::ios::out);
    //if (file.is_open()) {
    //    file << result;
    //}
    //file.close();
    return result;
}