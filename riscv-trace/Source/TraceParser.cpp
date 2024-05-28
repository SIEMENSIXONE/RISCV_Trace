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
void TraceParser::markLastLines(map<string, std::vector<string>> &retFuncAddrMap) {
    for (vector<TraceLineStruct>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        //if (it + 1 != FTraceLines->end()) {
        //    if (((it->func) != ((it + 1)->func)) && (addrCallerCalled.find(it->addr) == addrCallerCalled.end())) {
        //        it->isLastLine = true;
        //    }
        //}

        if (retFuncAddrMap.find(it->func) != retFuncAddrMap.end()) {
            vector<string> retAddrs = retFuncAddrMap.at(it->func);
            for (vector<string>::iterator it1 = retAddrs.begin(); it1 != retAddrs.end(); it1++) {
                if (*it1 == it->addr)  it->isLastLine = true;
            }
        }
    }
}
//
void TraceParser::markCallLines(map<string, string>& /*firstLineAddrMap*/, map<string, pair<string, string>> &addrCallerCalled, map<string, set<string>>& callingMap) {
    for (vector<TraceLineStruct>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        string addr = it->addr;
        string func = it->func;
        string instr = it->instr;
        //
        //if (addrCallerCalled.find(addr) != addrCallerCalled.end()) {
        //    pair<string, string> pair = addrCallerCalled.at(addr);
        //    if (it + 1 != FTraceLines->end()) {
        //        if ((it + 1)->func == pair.second) {
        //            if ((firstLineAddrMap.find((it + 1)->addr) != firstLineAddrMap.end()) && (firstLineAddrMap.at((it + 1)->addr) == pair.second)) it->isCallLine = true;
        //        }
        //    }
        //}

        if ((it + 1) != FTraceLines->end()) {
            if (((it + 1)->isFirstLine) && (!it->isLastLine)) {
                addrCallerCalled.insert({addr, make_pair(func, (it + 1)->func)});
                it->isCallLine = true;
                if (callingMap.find(func) == callingMap.end()) {
                    set<string> tmp;
                    tmp.insert((it + 1)->func);
                    callingMap.insert({func, tmp});
                }
                else {
                    callingMap.at(func).insert((it + 1)->func);
                }
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
