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
string TRACE_FLAG = "TRAC";
string OBJDUMP_FLAG = "OBJD";
string CODE_FLAG = "CODE";
//
TProjectParser::TProjectParser() {}
//
TProjectParser::~TProjectParser() {}
//
TProjectParser::Project TProjectParser::getProjectFromFile(const string &filename) {
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
    //
    while (getline(fin, buf)){
        string flag;
        string val;
        int flagLength = (int) min(min(TRACE_FLAG.length(), OBJDUMP_FLAG.length()), CODE_FLAG.length());
        //
        for(int i = 0; i < flagLength; i++){
            flag+=buf[i];
        }
        //
        for(int i = flagLength + 1; i < buf.length(); i++){
            val+=buf[i];
        }
        //
        if (flag == TRACE_FLAG) project.trace = val;
        //
        if (flag == OBJDUMP_FLAG) project.objdump = val;
        //
        if (flag == CODE_FLAG) project.code = val;
    }
    //
    fin.close();
    return project;
}

bool TProjectParser::saveProjectToFile(Project &project, const string &filename) {
    std::ofstream file;
    file.open(filename, std::ios::out);
    //
    if (file.is_open()){
        file
        << TRACE_FLAG << '|' << project.trace << std::endl
        << OBJDUMP_FLAG << '|' << project.objdump << std::endl
        << CODE_FLAG << '|' << project.code << std::endl;
    }else{
        return false;
    }
    file.close();
    return true;
}

