#include "MainComponent.h"
//
//==============================================================================
using namespace juce;
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
int MainComponent::AsSubComponent::ScrollableWindow::getNumberOfOccurances(const string &funcName){
    vector<int> linesVector = TraceWindow->getFuncLines(funcName);
    return (int) linesVector.size();
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
MainComponent::AsSubComponent::AsSubComponent(vector<TraceParser::TraceLineStruct> &vec, CodeSubComponent &_codeWindow){
    scrollableWindow = new ScrollableWindow(vec);
    addAndMakeVisible(scrollableWindow);
    //
    codeWindow = &_codeWindow;
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
       /* ColourIds{
      backgroundColourId = 0x1000200 , textColourId = 0x1000201 , highlightColourId = 0x1000202 , highlightedTextColourId = 0x1000203 ,
      outlineColourId = 0x1000205 , focusedOutlineColourId = 0x1000206 , shadowColourId = 0x1000207
        }*/
    searchField->setColour(TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    searchField->setColour(TextEditor::ColourIds::textColourId, juce::Colours::black);
    searchField->setColour(TextEditor::ColourIds::highlightColourId, juce::Colours::hotpink);
    searchField->setColour(TextEditor::ColourIds::highlightedTextColourId, juce::Colours::lightpink);
    searchField->setColour(TextEditor::ColourIds::outlineColourId, juce::Colours::black);
    searchField->setColour(TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::hotpink);
    searchField->setColour(TextEditor::ColourIds::shadowColourId, juce::Colours::grey);
    //
    searchField->setText("Search...");

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
        setSelectedFunc(selectedFunction);
        searchField->setText("Search...");
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
        return;
    }
    else {
        selectedFunction = funcName;
        selectedFuncOccurance = 0;
        scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
        codeWindow->selectFunc(selectedFunction);
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
//
void MainComponent::AsSubComponent::textEditorTextChanged(TextEditor& editor) {
    setSelectedFunc(editor.getText().toStdString());
}
//
MainComponent::CodeSubComponent::CodeSubComponent(const std::string &filepath, map<string, string> map){
    std::string file_content;
    std::getline(std::ifstream(filepath), file_content, '\0');
    CodeWindow = new CodeComponent(file_content, map);
    addAndMakeVisible(CodeWindow);
    CodeWindow->setSize(5, getParentHeight());
}
//
MainComponent::CodeSubComponent::~CodeSubComponent(){
    delete(CodeWindow);
}
//
void MainComponent::CodeSubComponent::paint (Graphics& g){
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}
void MainComponent::CodeSubComponent::resized(){
    CodeWindow->setSize(getWidth(), CodeWindow->getHeight());
}
//
void MainComponent::CodeSubComponent::selectFunc(const string& funcName) {
    CodeWindow->selectFunc(funcName);
}
//
MainComponent::AnalyzerSubComponent::AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, vector<string>>& _callingMap, map<string, vector<string>>& _callersMap) {
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
    int totalTime = (int) lines->size();
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
                timesCalledMap->insert({funcName, 1});
            }else{
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
            execTimeMapOneInstance->insert({ funcName, fullExecTime / timesCalled});
        }
    }
    //
    const int columnsNum = 6;
    vector<array<std::string, columnsNum>> *tableData = new vector<array<std::string, columnsNum>>();
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
        if (totalExecTime!=0) newRow.at(1) = to_string(totalExecTime);
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
MainComponent::AnalyzerSubComponent::~AnalyzerSubComponent() {
    if (table != nullptr) delete(table);
    delete(timesCalledMap);
    delete(execTimeMapTotalSelf);
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
    table->setSize(getWidth(), getHeight());
}
//
MainComponent::AnalyzerSubComponent::ProfileTable::ProfileTable(vector<array<std::string, 6>>& _data) {
    data = &_data;
    //
    box.setModel(this);
    box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
    box.setRowHeight(30);
    //
    box.getHeader().addColumn("Name", 1, 1.5 * singleColumnWidth);
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
MainComponent::AnalyzerSubComponent::ProfileTable::~ProfileTable() {
    box.getHeader().setLookAndFeel(nullptr);
}
//
void MainComponent::AnalyzerSubComponent::ProfileTable::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
        .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll(Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(alternateColour);
}
//
void MainComponent::AnalyzerSubComponent::ProfileTable::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    juce::String fontTypeface = "Courier New";
    float fontSize = (float) (TraceComponent::lineHeight - 6);
    juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
    juce::Font font(fontTypeface, fontSize, fontStyle);
    g.setFont(font);
    std::string val = (data->at(rowNumber)).at(columnId - 1);
    g.drawText(val, 2, 0, width - 4, height, Justification::centred, true);

    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}
//
void MainComponent::AnalyzerSubComponent::ProfileTable::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
		bool changesFlag = true;
		while (changesFlag) {
			changesFlag = false;
			for (int i = 0; i < data->size() - 1; i++) {
                if (isForwards) 
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
int MainComponent::AnalyzerSubComponent::ProfileTable::getNumRows() {
    return (int) data->size();
}
//
void MainComponent::AnalyzerSubComponent::ProfileTable::resized() {
    box.setBounds(getLocalBounds());
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
        grid.templateColumns = { Track (Fr (3)), Track (Fr (4)), Track(Fr(3))};
        grid.items = {GridItem (asPanelTitle), GridItem (codePanelTitle), GridItem(analyzerPanelTitle), GridItem (asPanel), GridItem (codePanel), GridItem(analyzerPanel)};
        grid.performLayout (workspaceArea);
    }else{
        placeholderPanel->setBounds(workspaceArea);
    }
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
    //
    //Trace parsing
    vector<TraceParser::TraceLineStruct> *vec = new vector<TraceParser::TraceLineStruct>();
    TraceParser traceParser(*vec);
    traceParser.parseTrace(project.trace);
    traceParser.addFuncAddresses(addrFuncMap);
    traceParser.markFirstLines(firstFuncAddrMap);
    traceParser.markLastLines(lastFuncAddrMap);
    codePanel = new CodeSubComponent(project.code, firstFuncAddrMap);
    asPanel = new AsSubComponent(*vec, *codePanel);
    //vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, vector<string>>& _callingMap, map<string, vector<string>>& _callersMap
    analyzerPanel = new AnalyzerSubComponent(*vec, funcAddrMap, callingMap, callersMap);
    addAndMakeVisible(asPanel);
    addAndMakeVisible(codePanel);
    addAndMakeVisible(analyzerPanel);
    projectOpened = true;
    resized();
}
//
void MainComponent::chooseProjectFile(){
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.traceproj");
    auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != File{}) openProjectFile((file.getFullPathName().toStdString()));
    });
}
//
void MainComponent::saveProject(){
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.traceproj");
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
StringArray MainComponent::getMenuBarNames(){
    return {"File"};
}
//
PopupMenu MainComponent::getMenuForIndex (int menuIndex, const String& menuName){
    PopupMenu menu;
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
    //
    return menu;
}
//
void MainComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex){}

