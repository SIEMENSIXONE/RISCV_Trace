//
//  ProjectParser.hpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 23.04.2024.
//
//
#ifndef ProjectParser_hpp
#define ProjectParser_hpp
//
#include <JuceHeader.h>
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
class TProjectParser {
    //
public:
    struct Project{
        juce::File project;
        juce::File trace;
        juce::File objdump;
        vector<juce::File> code;
    };
    //
    TProjectParser();
    ~TProjectParser();
    static Project getProjectFromFile(juce::File);
    static bool saveProjectToFile(Project &, juce::File);
};
//
#endif /* ProjectParser_hpp */
