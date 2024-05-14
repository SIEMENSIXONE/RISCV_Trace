#include "MainComponent.h"
//
//==============================================================================
using namespace juce;
//
MainComponent::PerformanceAnalyzer::PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, vector<string>>& _callingMap, map<string, vector<string>>& _callersMap, map<string, juce::Colour> &_funcColoursMap, MainComponent& _mainComponent)
{
    mainComponent = &_mainComponent;
    lines = &_linesInfoVector;
    funcAddrMap = &_funcAddrMap;
    callingMap = &_callingMap;
    callersMap = &_callersMap;
    //
    funcColoursMap = &_funcColoursMap;
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
    table = new ProfileTable(*tableData, *funcColoursMap, *mainComponent);
    addAndMakeVisible(table);
    table->setBounds(0, 0, getWidth(), getHeight());
}
//
MainComponent::PerformanceAnalyzer::~PerformanceAnalyzer()
{
    if (table != nullptr) delete(table);
    delete(timesCalledMap);
    delete(execTimeMapTotalSelf);
}
//
void MainComponent::PerformanceAnalyzer::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PerformanceAnalyzer", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}
//
void MainComponent::PerformanceAnalyzer::resized()
{
    table->setSize(getWidth(), getHeight());
}
//
void MainComponent::PerformanceAnalyzer::tableSetSelectedRow(const string& funcName) {
    table->setSelectedRow(funcName);
}
//
void MainComponent::PerformanceAnalyzer::setFontSize(const int size) {
    if (size < 0) return;
    //
    table->setFontSize(size);
}
//
MainComponent::PerformanceAnalyzer::ProfileTable::ProfileTable(vector<array<std::string, 6>>& _data, map<string, juce::Colour> &_funcColoursMap, MainComponent & _mainComponent) {
    data = &_data;
    mainComponent = &_mainComponent;
    funcColoursMap = _funcColoursMap;
    rowsColoursMap;
    //
    refreshRowsColoursMap();
    //
    box.setModel(this);
    box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
    box.setRowHeight(30);
    //
    int width = getWidth();
    if (width <= 0) width = 7;
    int singleColumnWidth = width / 7;
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
MainComponent::PerformanceAnalyzer::ProfileTable::~ProfileTable() {
    box.getHeader().setLookAndFeel(nullptr);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::white);
    if (rowIsSelected) g.setColour(juce::Colours::hotpink);
    g.fillRect(0, 0, width, height);
    //!!!
    box.getHorizontalScrollBar().setVisible(false);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    juce::String fontTypeface = "Courier New";
    //float fontSize = (float)(TraceComponent::lineHeight - 6);
    juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
    juce::Font font(fontTypeface, (float) fontSize, fontStyle);
    g.setFont(font);
    g.setColour(juce::Colours::black);
    std::string val = (data->at(rowNumber)).at(columnId - 1);
    if ((rowsColoursMap.size() != 0) && (rowsColoursMap.find(rowNumber) != rowsColoursMap.end())) g.setColour(rowsColoursMap.at(rowNumber));
    g.drawText(val, 2, 0, width - 4, height, Justification::centred, true);
    g.setColour(juce::Colours::white);
    g.fillRect(width - 1, 0, 1, height);
    g.setColour(juce::Colours::white);
    if ((rowsColoursMap.size() != 0) && (rowsColoursMap.find(rowNumber) != rowsColoursMap.end())) g.setColour(rowsColoursMap.at(rowNumber));
    g.drawRect(0, 0, width, height);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::sortOrderChanged(int newSortColumnId, bool isForwards)
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
        refreshRowsColoursMap();
        setSelectedRow(selectedFunc);
    }
}
//
int MainComponent::PerformanceAnalyzer::ProfileTable::getNumRows() {
    return (int)data->size();
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::resized() {
    //
    box.setBounds(getLocalBounds());
    int width = getWidth();
    int singleColumnWidth = width / 7;
    //
    for (int i = 0; i < box.getHeader().getNumColumns(true); i++) {
        int columnId = box.getHeader().getColumnIdOfIndex(i, true);
        if (columnId == 1) box.getHeader().setColumnWidth(columnId, 2 * singleColumnWidth);
        else box.getHeader().setColumnWidth(columnId, singleColumnWidth);
    }
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setSelectedRow(int index) {
    SparseSet<int> sparseSet;
    sparseSet.addRange(Range<int>(index, index + 1));
    selectedRow = index;
    box.setSelectedRows(sparseSet);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setSelectedRow(const string& name) {
    for (int i = 0; i < data->size(); i++) {
        array<std::string, 6> row = data->at(i);
        if (row.at(0) == name) {
            selectedFunc = name;
            setSelectedRow(i);
        }
    }
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::clearSelection() {
    SparseSet<int> sparseSet;
    sparseSet.addRange(Range<int>(0, 0));
    box.setSelectedRows(sparseSet);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::refreshRowsColoursMap() {
    rowsColoursMap = *(new map<int, juce::Colour>());
    int rowCtr = 0;
    for (vector<array<std::string, 6>> ::iterator it = data->begin(); it != data->end(); it++) {
        string funcName = it->at(0);
        if ((funcColoursMap.size() != 0) && (funcColoursMap.find(funcName) != funcColoursMap.end())) {
            rowsColoursMap.insert({ rowCtr, funcColoursMap.at(funcName) });
        }
        rowCtr++;
    }
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setFontSize(const int size) {
    if (size < 0) return;
    //
    fontSize = size;
}
//
MainComponent::PerformanceAnalyzer::ProfileTable::MyLookAndFeel::MyLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::hotpink);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::MyLookAndFeel::drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header,
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
    float headerFontSize = (float)(TraceComponent::lineHeight - 8);
    juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
    Font font(fontTypeface, headerFontSize, fontStyle);
    g.setFont(font);
    //
    g.drawFittedText(columnName, area, Justification::centred, 1);
    g.setColour(juce::Colours::white);
    g.drawRect(0, 0, width, height);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::cellClicked(int rowNumber, int columnId, const MouseEvent&) {
    setSelectedRow(rowNumber);
    array<std::string, 6> row = data->at(rowNumber);
    selectedFunc = row.at(0);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::selectedRowsChanged(int lastRowSelected) {
    mainComponent->setSelectedFunc(data->at(lastRowSelected).at(0), 3);
}
//
MainComponent::TitlePanel::TitlePanel(const std::string &str){
    text = str;
}
//
MainComponent::TitlePanel::~TitlePanel(){
    
}
//
void MainComponent::TitlePanel::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText (text, getLocalBounds(), Justification::centred, true);   // draw some placeholder text
}
//
void MainComponent::TitlePanel::resized()
{
}
//
MainComponent::AsSubComponent::ScrollableWindow::ScrollableWindow(vector<TraceParser::TraceLineStruct>& vec){
    TraceViewport = new Viewport("traceViewport");
    TraceWindow = new TraceComponent(vec, *TraceViewport);
    TraceViewport->setViewedComponent(TraceWindow, false);
    TraceWindow->setTopLeftPosition(TraceViewport->getPosition());
    TraceViewport->setScrollBarsShown(true, false);
    TraceViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colours::grey);
    TraceViewport->setScrollBarThickness(10);
    addAndMakeVisible(TraceWindow);
    addAndMakeVisible(TraceViewport);
    TraceViewport->setBounds(0, 0, getWidth()/2, getHeight());
}
//
MainComponent::AsSubComponent::ScrollableWindow::~ScrollableWindow(){
    delete(TraceWindow);
    delete(TraceViewport);
}
//
void MainComponent::AsSubComponent::ScrollableWindow::resized(){
    TraceWindow->setSize(getWidth(), TraceWindow->getHeight());
    TraceViewport->setSize(getWidth(), getHeight());
}
//
void MainComponent::AsSubComponent::ScrollableWindow::paint (Graphics& g){
    g.fillAll(Colours::white);
}
//
void MainComponent::AsSubComponent::ScrollableWindow::scrollToFunc(const string &funcName, int occuranceNum){
    vector<int> linesVector = TraceWindow->getFuncLines(funcName);
    int index = 0;
    if (linesVector.size()!=0){
        if (!(((occuranceNum) < 0) || ((occuranceNum) > linesVector.size()))) index = occuranceNum;
        int line = linesVector.at(index);
        TraceWindow->setTopLine(line);
        TraceWindow->setSelectedLine(line);
    }
}
//
void MainComponent::AsSubComponent::ScrollableWindow::clearSelection() {
    TraceWindow->clearSelections();
}
//
int MainComponent::AsSubComponent::ScrollableWindow::getNumberOfOccurances(const string &funcName){
    vector<int> linesVector = TraceWindow->getFuncLines(funcName);
    return (int) linesVector.size();
}
//
map<string, juce::Colour> MainComponent::AsSubComponent::ScrollableWindow::getFuncColoursMap() {
    return TraceWindow->getFuncColoursMap();
}
//
void MainComponent::AsSubComponent::ScrollableWindow::setFontSize(const int size) {
    if (size < 0) return;
    //
    TraceWindow->setFontSize(size);
}
//
MainComponent::AsSubComponent::OccurancesPanel::OccurancesPanel(AsSubComponent &asComp){
    parent = &asComp;
    numPanel = new OccuranceNumberPanel(parent->getCurrentSelectedOccurance());
    totalPanel = new OccuranceNumberPanel(-1);
    //
    outOfPanel = new TitlePanel("out of");
    occuranceTitlePanel = new TitlePanel("Occurance");
    //
    decrButton = new TextButton("<");
    incrButton = new TextButton(">");
    //
    decrButton->addListener(this);
    incrButton->addListener(this);
    //
    addAndMakeVisible(numPanel);
    addAndMakeVisible(totalPanel);
    addAndMakeVisible(decrButton);
    addAndMakeVisible(incrButton);
    //
    addAndMakeVisible(occuranceTitlePanel);
    addAndMakeVisible(outOfPanel);
}
//
MainComponent::AsSubComponent::OccurancesPanel::~OccurancesPanel(){
    incrButton->removeListener(this);
    decrButton->removeListener(this);
    delete(numPanel);
    delete(totalPanel);
    delete(incrButton);
    delete(decrButton);
    delete(occuranceTitlePanel);
    delete(outOfPanel);
}
//
//
void MainComponent::AsSubComponent::OccurancesPanel::paint (Graphics& g){
    g.fillAll(Colours::grey);
}
//
void MainComponent::AsSubComponent::OccurancesPanel::resized(){
    Rectangle<int> area = Rectangle<int>(0, 0, getWidth()/2, getHeight());
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    Grid grid;
    grid.templateRows = {Track (Fr (1))};
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track(Fr(4)), Track (Fr (1)), Track(Fr(4)), Track(Fr(1)) };
    grid.items = {GridItem (decrButton), GridItem (incrButton), GridItem(occuranceTitlePanel), GridItem (numPanel), GridItem(outOfPanel), GridItem(totalPanel) };
    grid.performLayout(area);
}
//
MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::TitlePanel(const string& _text) {
    text = _text;
}
//
MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::~TitlePanel() {
}
//
void MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText(text, getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}
//
void MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::resized()
{}
//
MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::OccuranceNumberPanel(int num){
    currentOccurNum = num;
}
//
MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::~OccuranceNumberPanel(){
    
}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::paint(Graphics &g){
    g.fillAll (juce::Colours::white);
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::black);
    g.setFont (12.0f);
    string text;
    if (currentOccurNum >= 0){
       text = to_string(currentOccurNum + 1);
    } else {
        text = "-";
    }
    g.drawText (text, getLocalBounds(), juce::Justification::centred, true);
    
}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::resized(){
}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::setNumber(int num){
    currentOccurNum = num;
    repaint();
    resized();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::setPanelNumbers(int num, int max){
    numPanel->setNumber(num);
    if (max <= 0) numPanel->setNumber(-1);
    totalPanel->setNumber(max-1);
    repaint();
    resized();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::incrButtonClicked(){
    parent->incrCurrentSelectedOccurance();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::decrButtonClicked(){
    parent->decrCurrentSelectedOccurance();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::buttonClicked(Button* button){
    
    if (button == incrButton)
    {
        incrButtonClicked();
    }
    if (button == decrButton){
        decrButtonClicked();
    }
}
//
MainComponent::AsSubComponent::AsSubComponent(vector<TraceParser::TraceLineStruct> &vec, MainComponent &_mainComponent){
    scrollableWindow = new ScrollableWindow(vec);
    addAndMakeVisible(scrollableWindow);
    //
    mainComponent = &_mainComponent;
    //
    occurancesPanel = new OccurancesPanel(*this);
    addAndMakeVisible(occurancesPanel);
    //
    functionsComboBox = new ComboBox("FunctionsComboBox");
    functionsComboBox->setText("Choose function...");
    functionsComboBox->setJustificationType (Justification::centred);
    functionsComboBox->setEditableText(false);
    //
    searchField = new TextEditor("searchField");
    searchField->setMultiLine(false);
    searchField->addListener(this);
    //
    searchField->setColour(TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    searchField->setColour(TextEditor::ColourIds::textColourId, juce::Colours::black);
    searchField->setColour(TextEditor::ColourIds::highlightColourId, juce::Colours::hotpink);
    searchField->setColour(TextEditor::ColourIds::highlightedTextColourId, juce::Colours::lightpink);
    searchField->setColour(TextEditor::ColourIds::outlineColourId, juce::Colours::black);
    searchField->setColour(TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::hotpink);
    searchField->setColour(TextEditor::ColourIds::shadowColourId, juce::Colours::grey);
    //
    searchField->setText(defaultSearchfieldText);

    addAndMakeVisible(searchField);
    //
    funcOccurances = new map<string, int>();
    set<string> funcNames;
    for (int i = 0; i < (&vec)->size(); i++) {
        funcNames.insert((&vec)->at(i).func);
    }
    //
    comboBoxItemID = new map<string, int>();
    int ctr = 1;
    for (set<string>::iterator it = funcNames.begin(); it != funcNames.end(); it++) {
        string funcName = *it;
        int occurancesNum = scrollableWindow->getNumberOfOccurances(funcName);
        
        if (funcName != "") {
            funcOccurances->insert({funcName, occurancesNum});
            //
            pair<string, int> tmp = make_pair(funcName, ctr++);
            comboBoxItemID->insert(tmp);
            functionsComboBox->addItem(tmp.first, tmp.second);
        }
 
    }
    //
    functionsComboBox->onChange = [this] { 
        selectedFunction = functionsComboBox->getText().toStdString();
        mainComponent->setSelectedFunc(selectedFunction, 1);
        searchField->setText(defaultSearchfieldText);
    };
    //functionsComboBox->getItem
    //functionsComboBox->setSelectedId(1);
    addAndMakeVisible(functionsComboBox);
}
//
MainComponent::AsSubComponent::~AsSubComponent(){
    delete(scrollableWindow);
    delete(functionsComboBox);
    delete(occurancesPanel);
    delete(searchField);
}
//
void MainComponent::AsSubComponent::paint (Graphics& g){
    g.fillAll(Colours::white);
}
//
void MainComponent::AsSubComponent::resized(){
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    Grid grid;
    grid.templateRows = {Track (Fr (1)),Track (Fr (1)),Track(Fr(1)), Track (Fr (40))};
    grid.templateColumns = { Track (Fr (1))};
    grid.items = {GridItem (functionsComboBox),GridItem(searchField), GridItem (occurancesPanel), GridItem (scrollableWindow)};
    grid.performLayout (getLocalBounds());
}
//
void MainComponent::AsSubComponent::selectFuncInCombobox(const string& funcName) {
    if (comboBoxItemID->find(funcName) == comboBoxItemID->end()) return;
    //
    functionsComboBox->setSelectedId(comboBoxItemID->at(funcName));
}
//
int MainComponent::AsSubComponent::getFuncOccuranceNumber(const string& funcName){
    if (funcOccurances->find(funcName) == funcOccurances->end()) return -1;
    return funcOccurances->at(funcName);
}
//
void MainComponent::AsSubComponent::setSelectedFunc(const string& funcName) {
    if (funcOccurances->find(funcName) == funcOccurances->end()) {
        selectedFunction = "";
        scrollableWindow->clearSelection();
        functionsComboBox->setSelectedId(0);
        functionsComboBox->setText("<No entries>");
        occurancesPanel->setPanelNumbers(0, 0);
        return;
    }
    else {
        selectedFunction = funcName;
        selectedFuncOccurance = 0;
        scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
        functionsComboBox->setSelectedId(comboBoxItemID->at(selectedFunction), juce::NotificationType::dontSendNotification);
        searchField->setText(defaultSearchfieldText, false);
        occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
        return;
    }
}
//
string MainComponent::AsSubComponent::getCurrentSelectedFunction(){
    return selectedFunction;
}
//
int MainComponent::AsSubComponent::getCurrentSelectedOccurance(){
    return selectedFuncOccurance;
}
//
void MainComponent::AsSubComponent::incrCurrentSelectedOccurance(){
    selectedFuncOccurance = (selectedFuncOccurance + 1) % getFuncOccuranceNumber(getCurrentSelectedFunction());
    scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
    occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
}
//
void MainComponent::AsSubComponent::decrCurrentSelectedOccurance(){
    selectedFuncOccurance = (selectedFuncOccurance - 1 + getFuncOccuranceNumber(getCurrentSelectedFunction())) % getFuncOccuranceNumber(getCurrentSelectedFunction());
    scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
    occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
}
//
map<string, juce::Colour> MainComponent::AsSubComponent::getFuncColoursMap() {
    return scrollableWindow->getFuncColoursMap();
}
//
void MainComponent::AsSubComponent::setFontSize(const int size) {
    if (size < 0) return;
    //
    scrollableWindow->setFontSize(size);
}
//
void MainComponent::AsSubComponent::textEditorTextChanged(TextEditor& editor) {
    mainComponent->setSelectedFunc(editor.getText().toStdString(), 1);
}
//
MainComponent::CodeSubComponent::CodeSubComponent(const std::vector<std::string>&filepaths, map<string, string> map, MainComponent& _mainComponent){
    mainComponent = &_mainComponent;
    //
    std::vector<std::string> filesContentVector;
    std::vector<std::string> filesNamesVector;
    std::vector<std::string> filepathsVector = filepaths;
    //
    for (std::vector<std::string>::iterator it = filepathsVector.begin(); it != filepathsVector.end(); it++) {
        string content;
        std::getline(std::ifstream(*it), content, '\0');
        filesContentVector.push_back(content);
        filesNamesVector.push_back(it->substr(it->find_last_of("/\\") + 1));
    }
    //
    codeWindows = new vector<CodeComponent*>();
    //
    for (int i = 0; i < min(filesContentVector.size(), filesNamesVector.size()); i++) {
        codeWindows->push_back(new CodeComponent(filesNamesVector.at(i), filesContentVector.at(i), map));
    }
    //
    tabs = new MyTabbedComponent(*codeWindows);
    tabs->setBounds(0, 0, getWidth(), getHeight());
    addAndMakeVisible(tabs);
}
//
MainComponent::CodeSubComponent::~CodeSubComponent(){
    //delete(CodeWindow);
    delete(codeWindows);
    delete(tabs);
}
//
void MainComponent::CodeSubComponent::paint (Graphics& g){
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}
void MainComponent::CodeSubComponent::resized(){
    //
    for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
        (*it)->setSize(getWidth(), getHeight());
    }
    //
    tabs->setBounds(0, 0, getWidth(), getHeight());
}
//
void MainComponent::CodeSubComponent::selectFunc(const string& funcName) {
    int tabIndex = 0;
    //
    for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
        if ((*it)->selectFunc(funcName)) tabs->setCurrentTabIndex(tabIndex);
        tabIndex++;
    }
}
//
void MainComponent::CodeSubComponent::setFontSize(const int size) {
    if (size < 0) return;
    //
    for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
        (*it)->setFontSize(size);
    }
}
//
MainComponent::CodeSubComponent::MyTabbedComponent::MyTabbedComponent(vector<CodeComponent*>& codeComponents)
    : TabbedComponent(TabbedButtonBar::TabsAtTop)
{
    //auto colour = findColour(ResizableWindow::backgroundColourId);
    auto colour = juce::Colours::black;
    //
    for (vector<CodeComponent*>::iterator it = codeComponents.begin(); it != codeComponents.end(); it++) {
        addTab((*it)->getFilename(), colour, *it, true);
    }
}
//
MainComponent::AnalyzerSubComponent::AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, vector<string>>& _callingMap, map<string, vector<string>>& _callersMap, map<string, pair<string, string>>& _addrCallingCalledMap, map<string, juce::Colour> &_funcColoursMap, MainComponent& _mainComponent) {
    mainComponent = &_mainComponent;
    performanceAnalyzer = new PerformanceAnalyzer(_linesInfoVector, _funcAddrMap, _callingMap, _callersMap, _funcColoursMap, *mainComponent);
    addAndMakeVisible(performanceAnalyzer);
}
//
MainComponent::AnalyzerSubComponent::~AnalyzerSubComponent() {
    delete(performanceAnalyzer);
}
//
void MainComponent::AnalyzerSubComponent::paint(Graphics& g) {
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::black);
    g.setFont(22.0f);
    g.drawText("AnalyzerSubComponent", getLocalBounds(), juce::Justification::centred, true);
}
//
void MainComponent::AnalyzerSubComponent::resized() {
    performanceAnalyzer->setSize(getWidth(), getHeight());
}
//
void MainComponent::AnalyzerSubComponent::setSelectedFunc(const string & funcName) {
    performanceAnalyzer->tableSetSelectedRow(funcName);
}
//
void MainComponent::AnalyzerSubComponent::setFontSize(const int size) {
    if (size < 0) return;
    //
    performanceAnalyzer->setFontSize(size);
}
//
MainComponent::PlaceholderSubComponent::PlaceholderSubComponent(){}
//
MainComponent::PlaceholderSubComponent::~PlaceholderSubComponent(){}
//
void MainComponent::PlaceholderSubComponent::paint (Graphics& g){
    g.fillAll (juce::Colours::white);
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::black);
    g.setFont (22.0f);
    g.drawText ("Empty! Choose a project or create a new one.", getLocalBounds(), juce::Justification::centred, true);
}
//
void MainComponent::PlaceholderSubComponent::resized(){}
//
MainComponent::MainComponent()
{
    currentSettings;
    loadSettings();
    //
    saveSettingsButton = new TextButton("Save");
    saveSettingsButton->addListener(this);
    //
    menuBar = new MenuBarComponent(this);
    addAndMakeVisible(menuBar);
    //
    //
    placeholderPanel = new PlaceholderSubComponent();
    addAndMakeVisible(placeholderPanel);
    //
    setSize (1440, 800);
}

MainComponent::~MainComponent()
{
    if (menuBar != nullptr) delete(menuBar);
    if (placeholderPanel != nullptr) delete(placeholderPanel);
    if (asPanelTitle != nullptr) delete(asPanelTitle);
    if (codePanelTitle != nullptr) delete(codePanelTitle);
    if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
    if (asPanel != nullptr) delete(asPanel);
    if (codePanel != nullptr) delete(codePanel);
    if (analyzerPanel != nullptr) delete(analyzerPanel);
    if (createProjWindow != nullptr) delete(createProjWindow);
    if (settingsWindow != nullptr) delete(settingsWindow);
    if (aboutWindow != nullptr) delete(aboutWindow);
    //
    if (saveSettingsButton != nullptr) delete(saveSettingsButton);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}
//
void MainComponent::resized()
{
    placeholderPanel->setSize(getWidth(), getHeight());
    //
    Rectangle<int> menuArea = Rectangle<int>(0, 0, getWidth(), menuHeight);
    Rectangle<int> workspaceArea = Rectangle<int>(0, menuHeight, getWidth(), getHeight() - menuHeight);
    //
    menuBar->setBounds(menuArea);
    //
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    //
    if (projectOpened){
        Grid grid;
        grid.templateRows = {Track (Fr (1)), Track (Fr (20)) };
        grid.templateColumns = { Track (Fr (2)), Track (Fr (4)), Track(Fr(2))};
        grid.items = {GridItem (asPanelTitle), GridItem (codePanelTitle), GridItem(analyzerPanelTitle), GridItem (asPanel), GridItem (codePanel), GridItem(analyzerPanel)};
        grid.performLayout (workspaceArea);
    }else{
        placeholderPanel->setBounds(workspaceArea);
    }
}
//
void MainComponent::setSelectedFunc(const string& funcName, int callerID) {
    if (callerID != 1) asPanel->setSelectedFunc(funcName);
    if (callerID != 2) codePanel->selectFunc(funcName);
    if (callerID != 3) analyzerPanel->setSelectedFunc(funcName);
}
//
void MainComponent::loadSettings() {
    juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    currentSettings = TSettingsParser::getSettingsFromFile(workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
void MainComponent::createProjectFile() {
    createProjWindow = new CreateProjectWindow("New project");
    createProjWindow->setVisible(true);
}
//
void MainComponent::openProjectFile(const string filepath) {
    TProjectParser::Project newProject = TProjectParser::getProjectFromFile(filepath);
    project = newProject;
    //
    if (asPanelTitle != nullptr) delete(asPanelTitle);
    if (codePanelTitle != nullptr) delete(codePanelTitle);
    if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
    if (asPanel != nullptr) delete(asPanel);
    if (codePanel != nullptr) delete(codePanel);
    if (analyzerPanel != nullptr) delete(analyzerPanel);
    //
    asPanelTitle = new TitlePanel("Assembly trace");
    codePanelTitle = new TitlePanel("Code");
    analyzerPanelTitle = new TitlePanel("Analyzer");
    //
    addAndMakeVisible(asPanelTitle);
    addAndMakeVisible(codePanelTitle);
    addAndMakeVisible(analyzerPanelTitle);
    //
    //Objdump parsing
    TObjdumpParser *parser = new TObjdumpParser();
    parser->parseFile(project.objdump);
    map<string, string> firstFuncAddrMap = parser->getFirstFuncAddrMap();
    map<string, string> addrFuncMap = parser->getAddrFuncMap();
    map<string, vector<string>> funcAddrMap = parser->getFuncAddrMap();
    //
    map<string, string> lastFuncAddrMap;
    for (map<string, vector<string>>::iterator it = funcAddrMap.begin(); it != funcAddrMap.end(); it++) {
        lastFuncAddrMap.insert({ (it->second).at((it->second).size() - 1), it->first});
    }
    //
    map<string, vector<string>> callingMap = parser->getCallingMap();
    map<string, vector<string>> callersMap = parser->getCallersMap();
    map<string, pair<string, string>> addrCallerCalled = parser->getAddrCallerCalled();
    //
    //Trace parsing
    vector<TraceParser::TraceLineStruct> *vec = new vector<TraceParser::TraceLineStruct>();
    TraceParser traceParser(*vec);
    traceParser.parseTrace(project.trace);
    traceParser.addFuncAddresses(addrFuncMap);
    traceParser.markFirstLines(firstFuncAddrMap);
    traceParser.markLastLines(lastFuncAddrMap);
    //
    //
    asPanel = new AsSubComponent(*vec, *this);
    map<string, juce::Colour> funcColoursMap = asPanel->getFuncColoursMap();
    codePanel = new CodeSubComponent(project.code, firstFuncAddrMap, *this);
    analyzerPanel = new AnalyzerSubComponent(*vec, funcAddrMap, callingMap, callersMap, addrCallerCalled, funcColoursMap, *this);
    setFontSizes();
    //
    //
    addAndMakeVisible(asPanel);
    addAndMakeVisible(codePanel);
    addAndMakeVisible(analyzerPanel);
    projectOpened = true;
    resized();
}
//
void MainComponent::chooseProjectFile(){
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.JSON");
    auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != File{}) openProjectFile((file.getFullPathName().toStdString()));
    });
}
//
void MainComponent::saveProject(){
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.JSON");
    auto chooserFlags = FileBrowserComponent::saveMode;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != File{}) 
        {
            TProjectParser::saveProjectToFile(project, file.getFullPathName().toStdString());
        }
    });
}
//
void MainComponent::closeProjectFile(){
    if (asPanelTitle != nullptr) delete(asPanelTitle);
    if (codePanelTitle != nullptr) delete(codePanelTitle);
    if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
    if (asPanel != nullptr) delete(asPanel);
    if (codePanel != nullptr) delete(codePanel);
    if (analyzerPanel != nullptr) delete(analyzerPanel);
    //
    asPanelTitle = nullptr;
    codePanelTitle = nullptr;
    analyzerPanelTitle = nullptr;
    asPanel = nullptr;
    codePanel = nullptr;
    analyzerPanel = nullptr;
    //
    projectOpened = false;
}
//
void MainComponent::openSettingsWindow() {
    if (settingsWindow != nullptr) delete(settingsWindow);
    settingsWindow = new SettingsWindow("Settings", *saveSettingsButton);
    settingsWindow->setVisible(false);
    settingsWindow->addComponentListener(this);
    settingsWindow->setVisible(true);
    
}
//
void MainComponent::openAboutWindow() {
    if (aboutWindow != nullptr) delete(aboutWindow);
    aboutWindow = new AboutWindow("About");
    aboutWindow->setVisible(false);
    aboutWindow->addComponentListener(this);
    aboutWindow->setVisible(true);
}
//
void MainComponent::updateCurrentSettings() {
    if (projectOpened) {
        setFontSizes();
    }
}
//
void MainComponent::setFontSizes() {
    analyzerPanel->setFontSize(currentSettings.analyzerFontSize);
    codePanel->setFontSize(currentSettings.codeFontSize);
    asPanel->setFontSize(currentSettings.traceFontSize);
    repaint();
    resized();
}
//
StringArray MainComponent::getMenuBarNames(){
    return {"File", "Settings", "Help" };
}
//
PopupMenu MainComponent::getMenuForIndex (int index, const String&){
    PopupMenu menu;
    if (isEnabled()) {
        //File
        if (index == 0) {
            //
            std::function<void()> createProjFunc = [this]() { createProjectFile(); };
            std::function<void()> chooseProjFunc = [this]() { chooseProjectFile(); };
            std::function<void()> saveProjFunc = [this]() { saveProject(); };
            std::function<void()> closeProjFunc = [this]() { closeProjectFile(); };
            //
            menu.addItem(String("New"), createProjFunc);
            menu.addItem(String("Open..."), chooseProjFunc);
            menu.addItem(String("Save..."), saveProjFunc);
            menu.addItem(String("Close"), closeProjFunc);
        }
        //
        //Settings
        else if (index == 1) {
            std::function<void()> openGeneralSettings = [this]() { openSettingsWindow(); };
            //
            menu.addItem(String("General"), openGeneralSettings);
        }
        //
        //Help
        else if (index == 2) {
            std::function<void()> openAboutProgram = [this]() { openAboutWindow(); };
            //
            menu.addItem(String("About"), openAboutProgram);
        }
    }
    return menu;
}
//
void MainComponent::menuItemSelected (int	menuItemID, int	topLevelMenuIndex){}
//
void MainComponent::buttonClicked(Button* button) {
    if (button == saveSettingsButton) {
        loadSettings();
        updateCurrentSettings();
    }
}

//
void MainComponent::componentVisibilityChanged(Component& component) {
    if (component.isVisible()) {
        //visible
        component.setAlwaysOnTop(true);
        this->setEnabled(false);
        //
        repaint();
        resized();
    }
    else {
        //not visible
        this->setEnabled(true);
        //
        repaint();
        resized();
    }
}
