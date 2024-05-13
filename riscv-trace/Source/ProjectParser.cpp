//
//  ProjectParser.cpp
//  RISCV_Trace - App
//
//  Created by Веселяев Михаил on 23.04.2024.
//
//
#include "ProjectParser.hpp"
//
//
string TRACE_FLAG = "Trace";
string OBJDUMP_FLAG = "Objdump";
string CODE_FLAG = "Code";
//
using json = nlohmann::json;
//
TProjectParser::TProjectParser() {}
//
TProjectParser::~TProjectParser() {}
//
TProjectParser::Project TProjectParser::getProjectFromFile(const string &filename) {
    //
    //
    Project project;
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
        return project;
    }
    std::ifstream i(filename);
    json j;
    i >> j;
    if (!j.contains(TRACE_FLAG)) return project;
    if (!j.contains(OBJDUMP_FLAG)) return project;
    if (!j.contains(CODE_FLAG)) return project;
    //
    project.trace = j[TRACE_FLAG];
    project.objdump = j[OBJDUMP_FLAG];
    project.code = j[CODE_FLAG];
    //
    fin.close();
    return project;
}
//
bool TProjectParser::saveProjectToFile(Project &project, const string &filename) {
    std::ofstream file;
    file.open(filename, std::ios::out);
    //
    if (file.is_open()) {
        json j;
        j[TRACE_FLAG] = project.trace;
        j[OBJDUMP_FLAG] = project.objdump;
        j[CODE_FLAG] = project.code;
        file << std::setw(4) << j << std::endl;
    }
    else {
        return false;
    }
    //
    file.close();
    return true;
}

