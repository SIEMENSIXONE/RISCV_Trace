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
TProjectParser::Project TProjectParser::getProjectFromFile(juce::File filename) {
    //
    //
    Project project;
    //
    string result;
    string buf;
 
    ifstream fin(filename.getFullPathName().toWideCharPointer());
    //
    if(!fin.is_open())
    {
        return project;
    }
    std::ifstream i(filename.getFullPathName().toWideCharPointer());
    json j;
    i >> j;
    if (!j.contains(TRACE_FLAG)) return project;
    if (!j.contains(OBJDUMP_FLAG)) return project;
    if (!j.contains(CODE_FLAG)) return project;
    //
    const string traceTmp = j[TRACE_FLAG];
    const string objdumpTmp = j[OBJDUMP_FLAG];
    vector<string> codeFiles = j[CODE_FLAG];
    //
    project.trace = juce::File(traceTmp);
    project.objdump = juce::File(objdumpTmp);
    project.code;
    //
    for (vector<string>::iterator it = codeFiles.begin(); it != codeFiles.end(); it++) {
        const string codeFileStr = *it;
        juce::File codeFile(codeFileStr);
        project.code.push_back(codeFile);
    }
    //
    fin.close();
    return project;
}
//
bool TProjectParser::saveProjectToFile(Project &project, juce::File filename) {
    std::ofstream file;
    file.open(filename.getFullPathName().toWideCharPointer(), std::ios::out);
    //
    if (file.is_open()) {
        json j;
        j[TRACE_FLAG] = project.trace.getFullPathName().toStdString();
        j[OBJDUMP_FLAG] = project.objdump.getFullPathName().toStdString();
        //
        vector<string> filePathsVector;
        for (vector<juce::File>::iterator it = project.code.begin(); it != project.code.end(); it++) {
            filePathsVector.push_back(it->getFullPathName().toStdString());
        }
        //
        j[CODE_FLAG] = filePathsVector;
        //
        file << std::setw(4) << j << std::endl;
    }
    else {
        return false;
    }
    //
    file.close();
    return true;
}

