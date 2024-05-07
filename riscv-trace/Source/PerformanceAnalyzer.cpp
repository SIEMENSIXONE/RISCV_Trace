/*
  ==============================================================================

    PerformanceAnalyzer.cpp
    Created: 2 May 2024 12:36:39pm
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PerformanceAnalyzer.h"

//==============================================================================
PerformanceAnalyzer::PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, vector<string>>& _callingMap, map<string, vector<string>>& _callersMap)
{
    lines = &_linesInfoVector;
    funcAddrMap = &_funcAddrMap;
    callingMap = &_callingMap;
    callersMap = &_callersMap;
    //
    timesCalledMap = new map<string, int>();
    execTimeMapTotal = new map<string, int>();
    execTimeMapTotalSelf = new map<string, int>();
    execTimeMapOneInstance = new map<string, int>();
    //
    int totalTime = (int)lines->size();
    std::vector<string> funcNameVector;
    for (map<string, vector<string>>::iterator it = _funcAddrMap.begin(); it != _funcAddrMap.end(); it++) funcNameVector.push_back(it->first);
    //
    vector<string> curFuncs;
    for (vector<TraceParser::TraceLineStruct>::iterator it = lines->begin(); it != lines->end(); it++) {
        bool flag = false;
        if (it->func != "") {
            if (it->isFirstLine)
                curFuncs.push_back(it->func);
            //
            for (int i = 0; i < curFuncs.size(); i++) {
                string funcTmp = curFuncs.at(i);
                if (it->func == funcTmp) flag = true;
                //
                if (execTimeMapTotal->find(funcTmp) == execTimeMapTotal->end()) {
                    execTimeMapTotal->insert({ funcTmp, 1 });
                }
                else {
                    execTimeMapTotal->at(funcTmp)++;
                }
            }
            //
            if (!flag) {
                if (execTimeMapTotal->find(it->func) == execTimeMapTotal->end()) {
                    execTimeMapTotal->insert({ it->func, 1 });
                }
                else {
                    execTimeMapTotal->at(it->func)++;
                }
            }
            //
            if (it->isLastLine) {
                curFuncs.pop_back();
            }
        }
    }
    //
    for (vector<TraceParser::TraceLineStruct>::iterator it = lines->begin(); it != lines->end(); it++) {
        if (it->isFirstLine) {
            string funcName = it->func;
            if (timesCalledMap->find(funcName) == timesCalledMap->end()) {
                timesCalledMap->insert({ funcName, 1 });
            }
            else {
                timesCalledMap->at(funcName)++;
            }
        }
        //
        if (it->func != "") {
            string funcName = it->func;
            if (execTimeMapTotalSelf->find(funcName) == execTimeMapTotalSelf->end()) {
                execTimeMapTotalSelf->insert({ funcName, 1 });
            }
            else {
                execTimeMapTotalSelf->at(funcName)++;
            }

        }
    }
    //
    for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
        string funcName = *it;
        if (execTimeMapTotalSelf->find(funcName) != execTimeMapTotalSelf->end()) {
            int fullExecTime = execTimeMapTotalSelf->at(funcName);
            int timesCalled = 1;
            if (timesCalledMap->find(funcName) != timesCalledMap->end()) {
                if (timesCalledMap->at(funcName) > 1) timesCalled = timesCalledMap->at(funcName);
            }
            //
            execTimeMapOneInstance->insert({ funcName, fullExecTime / timesCalled });
        }
    }
    //
    const int columnsNum = 6;
    vector<array<std::string, columnsNum>>* tableData = new vector<array<std::string, columnsNum>>();
    //
    for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
        string funcName = *it;
        array<std::string, columnsNum> newRow;
        newRow.fill("-");
        //
        //name
        newRow.at(0) = funcName;
        //
        //time
        int totalExecTime = 0;
        if (execTimeMapTotal->find(funcName) != execTimeMapTotal->end()) totalExecTime = execTimeMapTotal->at(funcName);
        newRow.at(1) = "-";
        if (totalExecTime != 0) newRow.at(1) = to_string(totalExecTime);
        //
        //time %
        newRow.at(2) = "-";
        if (totalExecTime != 0) newRow.at(2) = to_string((totalExecTime * 100) / totalTime);
        //
        //called
        int timesCalled = 0;
        if (timesCalledMap->find(funcName) != timesCalledMap->end()) timesCalled = timesCalledMap->at(funcName);
        newRow.at(3) = "-";
        if (timesCalled != 0) newRow.at(3) = to_string(timesCalled);
        //
        //self
        if (execTimeMapTotalSelf->find(funcName) != execTimeMapTotalSelf->end()) {

            //// от общего времени
            //newRow.at(4) = to_string(((totalExecTime - execTimeMapTotalSelf->at(funcName)) * 100 ) / totalTime) + "%";
            //newRow.at(5) = to_string((execTimeMapTotalSelf->at(funcName) * 100) / totalTime) + "%";


            // от собственного времени функции
            newRow.at(4) = to_string(((totalExecTime - execTimeMapTotalSelf->at(funcName)) * 100) / totalExecTime);
            newRow.at(5) = to_string((execTimeMapTotalSelf->at(funcName) * 100) / totalExecTime);
        }
        //
        tableData->push_back(newRow);
    }
    //
    table = new ProfileTable(*tableData);
    addAndMakeVisible(table);
    table->setBounds(0, 0, getWidth(), getHeight());
}
//
PerformanceAnalyzer::~PerformanceAnalyzer()
{
    if (table != nullptr) delete(table);
    delete(timesCalledMap);
    delete(execTimeMapTotalSelf);
}
//
void PerformanceAnalyzer::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PerformanceAnalyzer", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}
//
void PerformanceAnalyzer::resized()
{
    table->setSize(getWidth(), getHeight());
}
//
PerformanceAnalyzer::ProfileTable::ProfileTable(vector<array<std::string, 6>>& _data) {
    data = &_data;
    //
    box.setModel(this);
    box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
    box.setRowHeight(30);
    //
    int width = getParentWidth();
    int singleColumnWidth = (3 * (width / 10)) / 7;
    //
    box.getHeader().addColumn("Name", 1, (int)(2 * singleColumnWidth));
    box.getHeader().addColumn("Time", 2, singleColumnWidth);
    box.getHeader().addColumn("Time (%)", 3, singleColumnWidth);
    box.getHeader().addColumn("Called", 4, singleColumnWidth);
    box.getHeader().addColumn("Children (%)", 5, singleColumnWidth);
    box.getHeader().addColumn("Self (%)", 6, singleColumnWidth);
    box.getHeader().setLookAndFeel(&myLookAndFeel);
    //
    addAndMakeVisible(box);
    //
}
//
PerformanceAnalyzer::ProfileTable::~ProfileTable() {
    box.getHeader().setLookAndFeel(nullptr);
}
//
void PerformanceAnalyzer::ProfileTable::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::grey);
    if (rowIsSelected) g.setColour(juce::Colours::hotpink);
    g.fillRect(0, 0, width, height);
    g.setColour(juce::Colours::white);
    g.drawRect(0, 0, width, height);
}
//
void PerformanceAnalyzer::ProfileTable::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    juce::String fontTypeface = "Courier New";
    float fontSize = (float)(TraceComponent::lineHeight - 6);
    juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
    juce::Font font(fontTypeface, fontSize, fontStyle);
    g.setFont(font);
    std::string val = (data->at(rowNumber)).at(columnId - 1);
    g.drawText(val, 2, 0, width - 4, height, Justification::centred, true);
    g.setColour(juce::Colours::white);
    g.fillRect(width - 1, 0, 1, height);
}
//
void PerformanceAnalyzer::ProfileTable::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        bool changesFlag = true;
        while (changesFlag) {
            changesFlag = false;
            for (int i = 0; i < data->size() - 1; i++) {
                if (!isForwards)
                {
                    if (newSortColumnId == 1) {
                        string val1 = data->at(i)[newSortColumnId - 1];
                        string val2 = data->at(i + 1)[newSortColumnId - 1];
                        //
                        if (val1 < val2) {
                            array<std::string, 6> tmp = data->at(i);
                            data->at(i) = data->at(i + 1);
                            data->at(i + 1) = tmp;
                            //
                            changesFlag = true;
                        }
                    }
                    else
                    {
                        string val1 = data->at(i)[newSortColumnId - 1];
                        string val2 = data->at(i + 1)[newSortColumnId - 1];
                        int compVal1 = -1;
                        int compVal2 = -1;
                        if (val1 != "-") compVal1 = stoi(val1);
                        if (val2 != "-") compVal2 = stoi(val2);
                        if (compVal1 < compVal2) {
                            array<std::string, 6> tmp = data->at(i);
                            data->at(i) = data->at(i + 1);
                            data->at(i + 1) = tmp;
                            //
                            changesFlag = true;
                        }
                    }
                }
                else
                {
                    if (newSortColumnId == 1) {
                        string val1 = data->at(i)[newSortColumnId - 1];
                        string val2 = data->at(i + 1)[newSortColumnId - 1];
                        //
                        if (val1 > val2) {
                            array<std::string, 6> tmp = data->at(i);
                            data->at(i) = data->at(i + 1);
                            data->at(i + 1) = tmp;
                            //
                            changesFlag = true;
                        }
                    }
                    else
                    {
                        string val1 = data->at(i)[newSortColumnId - 1];
                        string val2 = data->at(i + 1)[newSortColumnId - 1];
                        int compVal1 = -1;
                        int compVal2 = -1;
                        if (val1 != "-") compVal1 = stoi(val1);
                        if (val2 != "-") compVal2 = stoi(val2);
                        if (compVal1 > compVal2) {
                            array<std::string, 6> tmp = data->at(i);
                            data->at(i) = data->at(i + 1);
                            data->at(i + 1) = tmp;
                            //
                            changesFlag = true;
                        }
                    }
                }
            }
        }
        //
        box.updateContent();
    }
}
//
int PerformanceAnalyzer::ProfileTable::getNumRows() {
    return (int)data->size();
}
//
void PerformanceAnalyzer::ProfileTable::resized() {
    box.setBounds(getLocalBounds());
}
//
PerformanceAnalyzer::ProfileTable::MyLookAndFeel::MyLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::hotpink);
}
//
void PerformanceAnalyzer::ProfileTable::MyLookAndFeel::drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header,
    const String& columnName, int /*columnId*/,
    int width, int height, bool isMouseOver, bool isMouseDown,
    int columnFlags)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(0, 0, width, height);
    auto highlightColour = header.findColour(TableHeaderComponent::highlightColourId);

    if (isMouseDown)
        g.fillAll(highlightColour);
    else if (isMouseOver)
        g.fillAll(highlightColour.withMultipliedAlpha(0.625f));

    Rectangle<int> area(width, height);
    area.reduce(4, 0);

    if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
    {
        Path sortArrow;
        sortArrow.addTriangle(0.0f, 0.0f,
            0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
            1.0f, 0.0f);

        g.setColour(juce::Colours::hotpink);
        g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced(2).toFloat(), true));
    }
    //
    g.setColour(juce::Colours::white);
    juce::String fontTypeface = "Courier New";
    float fontSize = (float)(TraceComponent::lineHeight - 8);
    juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
    Font font(fontTypeface, fontSize, fontStyle);
    g.setFont(font);
    //
    g.drawFittedText(columnName, area, Justification::centred, 1);
    g.setColour(juce::Colours::white);
    g.drawRect(0, 0, width, height);
}
