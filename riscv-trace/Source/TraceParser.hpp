//
//  TraceParser.hpp
//
//
//  Created by Веселяев Михаил on 13.04.2024.
//
//
#ifndef TraceParser_hpp
#define TraceParser_hpp
//
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
//
using namespace std;
//
static char IN_DELIM = '|';
static int DELIM_CTR = 2;
//
class TraceParser {
public:
    struct TraceLineStruct{
        int time = 0;
        string addr = "";
        string func = "";
        string instr = "";
        bool isFirstLine = false;
        //
        friend ostream& operator<< (std::ostream& stream, const TraceLineStruct& traceLine);
    };
    //
    TraceParser(vector<TraceLineStruct> &);
    ~TraceParser();
    void addLine(const TraceLineStruct&);
    void parseTrace(const string& traceFilepath);
    void addFuncAddresses(map<string, string> &);
    void markFirstLines(map<string, string>&);
    friend ostream& operator<< (std::ostream& stream, const TraceParser& traceLine);
    //
private:
    vector<TraceLineStruct> *FTraceLines;
};

#endif /* Trace_hpp */
