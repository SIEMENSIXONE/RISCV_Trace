//
//  SettingsParser.hpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 13.05.2024.
//
//
#ifndef SettingsParser_hpp
#define SettingsParser_hpp
//
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "JSON/json.hpp"
//
using namespace std;
//
class TSettingsParser {
    //
public:
    struct Settings {
        int interfaceFontSize = 9;
        int traceFontSize = 9;
        int codeFontSize = 9;
        int analyzerFontSize = 9;
        //
        string lang = "ENG";
    };
    //
    TSettingsParser();
    ~TSettingsParser();
    static Settings getSettingsFromFile(const string &);
    static bool saveSettingsToFile(Settings&, const string &);
};
//
#endif /* SettingsParser_hpp */
