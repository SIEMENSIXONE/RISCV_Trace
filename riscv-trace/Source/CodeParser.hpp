//
//  CodeParser.hpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 23.04.2024.
//

#ifndef CodeParser_hpp
#define CodeParser_hpp
//
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
//
using namespace std;
//
class TCodeParser {
    //
public:
    const string DataTypes[29] = 
    { 
        "char", 
        "signed char",
        "unsigned char",
        "short",
        "short int",
        "signed short",
        "signed short int",
        "unsigned short",
        "unsigned short int",
        "int",
        "signed",
        "signed int",
        "unsigned",
        "unsigned int",
        "long",
        "long int",
        "signed long",
        "signed long int",
        "unsigned long",
        "unsigned long int",
        "long long",
        "long long int",
        "signed long long",
        "signed long long int",
        "unsigned long long",
        "unsigned long long int",
        "float",
        "double",
        "long double"
    };
    //
    struct CCode{
        map<string, pair<int, int>> functions; //также должна быть информация о файле, в котором представлена реализация функции
    };
    //
    TCodeParser();
    ~TCodeParser();
    static CCode getCodeFromFile(const string & code, map<string, string>& funcAddrMap);
};
//
#endif /* CodeParser_hpp */
