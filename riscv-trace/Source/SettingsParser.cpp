//
//  SettingsParser.cpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 13.05.2024.
//
//
#include "SettingsParser.hpp"
//
//
string TRACE_FONT_FLAG = "Trace";
string CODE_FONT_FLAG = "Code";
string ANALYZER_FONT_FLAG = "Analyzer";
//
using json = nlohmann::json;
//
TSettingsParser::TSettingsParser() {}
//
TSettingsParser::~TSettingsParser() {}
//
TSettingsParser::Settings TSettingsParser::getSettingsFromFile(const string &filename) {
    //
    //
    Settings settings = *new Settings();
    //
    string result;
    string buf;
    ifstream fin(filename);
    //
    if(!fin.is_open())
    {
        //TODO: Exception
        cout<< "-----------------------------------" <<endl;
        cout<<"Error! Failed to open file."<<endl;
        cout<< "-----------------------------------" <<endl;
        return settings;
    }
    //
    std::ifstream i(filename);
    json j;
    i >> j;
    //
    if (!j.contains(TRACE_FONT_FLAG)) return settings;
    if (!j.contains(CODE_FONT_FLAG)) return settings;
    if (!j.contains(ANALYZER_FONT_FLAG)) return settings;
    //
    settings.traceFontSize = j[TRACE_FONT_FLAG];
    settings.codeFontSize = j[CODE_FONT_FLAG];
    settings.analyzerFontSize = j[ANALYZER_FONT_FLAG];
    //
    fin.close();
    return settings;
}
//
bool TSettingsParser::saveSettingsToFile(Settings &settings, const string &filename) {
    std::ofstream file;
    file.open(filename, std::ios::out);
    //
    if (file.is_open()) {
        json j;
        j[TRACE_FONT_FLAG] = settings.traceFontSize;
        j[CODE_FONT_FLAG] = settings.codeFontSize;
        j[ANALYZER_FONT_FLAG] = settings.analyzerFontSize;
        file << std::setw(4) << j << std::endl;
    }
    else {
        return false;
    }
    //
    file.close();
    return true;
}

