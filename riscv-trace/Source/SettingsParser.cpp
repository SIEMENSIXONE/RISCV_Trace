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
string INTERFACE_FONT_FLAG = "Interface";
string TRACE_FONT_FLAG = "Trace";
string CODE_FONT_FLAG = "Code";
string ANALYZER_FONT_FLAG = "Analyzer";
string LANG_FLAG = "Lang";
//
using json = nlohmann::json;
//
TSettingsParser::TSettingsParser() {}
//
TSettingsParser::~TSettingsParser() {}
//
TSettingsParser::Settings TSettingsParser::getSettingsFromFile(const string &filename) {
    //
    Settings settings = *new Settings();
    //
    string result;
    string buf;
    ifstream fin(filename);
    //
    if(!fin.is_open())
    {
        return settings;
    }
    //
    std::ifstream i(filename);
    json j;
    i >> j;
    //
    if (!j.contains(INTERFACE_FONT_FLAG)) return settings;
    if (!j.contains(TRACE_FONT_FLAG)) return settings;
    if (!j.contains(CODE_FONT_FLAG)) return settings;
    if (!j.contains(ANALYZER_FONT_FLAG)) return settings;
    if (!j.contains(LANG_FLAG)) return settings;
    //
    settings.interfaceFontSize = j[INTERFACE_FONT_FLAG];
    settings.traceFontSize = j[TRACE_FONT_FLAG];
    settings.codeFontSize = j[CODE_FONT_FLAG];
    settings.analyzerFontSize = j[ANALYZER_FONT_FLAG];
    settings.lang = j[LANG_FLAG];
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
        j[INTERFACE_FONT_FLAG] = settings.interfaceFontSize;
        j[TRACE_FONT_FLAG] = settings.traceFontSize;
        j[CODE_FONT_FLAG] = settings.codeFontSize;
        j[ANALYZER_FONT_FLAG] = settings.analyzerFontSize;
        j[LANG_FLAG] = settings.lang;
        file << std::setw(4) << j << std::endl;
    }
    else {
        return false;
    }
    //
    file.close();
    return true;
}

