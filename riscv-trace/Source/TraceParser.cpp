//
//  TraceParser.cpp
//
//
//  Created by Веселяев Михаил on 13.04.2024.
//
//
#include "TraceParser.hpp"
//
using namespace std;
//
ostream& operator<<(ostream& stream, const TraceParser::TraceLineStruct& traceLine){
    stream << traceLine.time << IN_DELIM << traceLine.addr << IN_DELIM << traceLine.func<< IN_DELIM << traceLine.instr << IN_DELIM;
    return stream;
}
//
TraceParser::TraceParser(vector<TraceLineStruct> &vec){
    FTraceLines = &vec;
}
//
TraceParser::~TraceParser(){
    delete(FTraceLines);
}
//
void TraceParser::addLine(const TraceParser::TraceLineStruct &line){
    FTraceLines->push_back(line);
}
//
void TraceParser::parseTrace(const string& traceFilepath){
    ifstream fin(traceFilepath);
    string line;
    if(!fin.is_open())
    {
        //TODO: Exception
        cout<< "-----------------------------------" <<endl;
        cout<<"Error! Failed to open file."<<endl;
        cout<< "-----------------------------------" <<endl;
        return;
    }
    //
    while (getline(fin, line)){
        size_t lineLength = line.length();
        int delimCtr = 0;
        for (int i = 0; i < lineLength; i++){
            if (line[i] == IN_DELIM) delimCtr++;
        }
        //
        if (delimCtr != DELIM_CTR){
            //TODO: Exception
            cout<< "-----------------------------------" <<endl;
            cout<<"Error! Wrong trace format."<<endl;
            cout<< "-----------------------------------" <<endl;
            return;
        }
        //
        TraceParser::TraceLineStruct traceLine;
        string buf;
        int delimPos = -1;
        for (int i = 0; i < lineLength; i++){
            //
            if (line[i] == IN_DELIM){
                delimPos = i;
                break;
            }
            buf+=line[i];
        }
        //
        traceLine.time = stoi(buf);
        //
        buf = "";
        for (int i = delimPos + 3; i < lineLength; i++){
            //
            if (line[i] == IN_DELIM){
                delimPos = i;
                break;
            }
            buf+=line[i];
        }
        traceLine.addr = buf;
        //
        buf = "";
        for (int i = delimPos + 1; i < lineLength; i++){
            //
            if (line[i] == IN_DELIM){
                delimPos = i;
                break;
            }
            buf+=line[i];
        }
        traceLine.instr = buf;
        this->addLine(traceLine);
    }
    
}
//
void TraceParser::addFuncAddresses(map<string, string> &addrFuncMap){
    for (vector<TraceLineStruct>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        string addr = it->addr;
        if (addrFuncMap.find(addr) != addrFuncMap.end()) {
            string func = addrFuncMap.at(addr);
            it->func = func;
        }
    }
}
//
void TraceParser::markFirstLines(map<string, string>& firstLineAddrMap) {
    for (vector<TraceLineStruct>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        string addr = it->addr;
        if (firstLineAddrMap.find(addr) != firstLineAddrMap.end()) {
            it->isFirstLine = true;
        }
    }
}
//
void TraceParser::markLastLines(map<string, string>& lastLineAddrMap, map<string, pair<string, string>> &addrCallerCalled) {
    for (vector<TraceLineStruct>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        if (lastLineAddrMap.find(it->addr) != lastLineAddrMap.end()) {
            it->isLastLine = true;
        }
        
        if (it + 1 != FTraceLines->end()) {
            if (((it->func) != ((it + 1)->func)) && (addrCallerCalled.find(it->addr) == addrCallerCalled.end())) {
                it->isLastLine = true;
            }
        }
    }
}
//
ostream& operator<< (std::ostream& stream, const TraceParser& traceLine){
    for( auto it = traceLine.FTraceLines->begin(); it != traceLine.FTraceLines->end(); ++it )
    {
        stream << setw(6) << it->time << IN_DELIM << setw(16) << it->addr<< IN_DELIM << setw(10) << it->func<< IN_DELIM << it->instr << endl;
    }
    return stream;
}
