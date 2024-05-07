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
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "JSON/json.h"
//
using namespace std;
//
class TProjectParser {
    //
public:
    struct Project{
        string trace;
        string objdump;
        vector<string> code;
    };
    //
    TProjectParser();
    ~TProjectParser();
    static Project getProjectFromFile(const string &);
    static bool saveProjectToFile(Project &, const string &);
};
//
#endif /* ProjectParser_hpp */
