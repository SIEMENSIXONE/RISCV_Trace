/*
  ==============================================================================

    TraceComponent.cpp
    Created: 14 Apr 2024 12:42:18pm
    Author:  Веселяев Михаил

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TraceComponent.h"

//==============================================================================
//
TraceComponent::TraceLine::TraceLineElement::TraceLineElement(const std::string &str){
    text = str;
}
//
TraceComponent::TraceLine::TraceLineElement::~TraceLineElement(){
}
//
void TraceComponent::TraceLine::TraceLineElement::paint (juce::Graphics& g){
    g.fillAll (backgroundColor);
    //
    g.setColour (borderColor);
    g.drawRect (getLocalBounds(), 1);
    //
    g.setColour (textColor);
    juce::Font font(fontTypeface, fontSize, fontStyle);
    g.setFont(font);
    
    g.drawText (text, textXOffset, 0, getWidth(), getHeight(), textJustification, true);
//    g.drawText (text, getLocalBounds(),
//                textJustification, true);
}
//
void TraceComponent::TraceLine::TraceLineElement::resized(){
}
//
string TraceComponent::TraceLine::TraceLineElement::getText() {
    return text;
}
//
TraceComponent::TraceLine::TraceFuncElement::TraceFuncElement(const std::string &str, TraceLine &_traceLine, TraceComponent &_traceComp)
:TraceLineElement(str)
{
    fontSize = lineHeight;
    traceLine = &_traceLine;
    traceComp = &_traceComp;
    addMouseListener(this, false);
}
//
TraceComponent::TraceLine::TraceFuncElement::~TraceFuncElement()
{
    removeMouseListener(this);
}
//
void TraceComponent::TraceLine::TraceFuncElement::paint (juce::Graphics& g){
    g.fillAll (backgroundColor);
    //
    //g.setColour (borderColor);
    //g.drawRect (getLocalBounds(), 1);
    //
    bool funcNameVisibility = traceLine->getLineInfo().isFirstLine;
    g.setColour(borderColor);
    if (funcNameVisibility) g.drawLine(0, 0, getWidth(), 0);
    //
    g.setColour (textColor);
    juce::Font font(fontTypeface, fontSize, fontStyle);
    g.setFont(font);
    if (funcNameVisibility) g.drawText (text, getLocalBounds(), textJustification, true);
}
//
void TraceComponent::TraceLine::TraceFuncElement::mouseDown(const juce::MouseEvent& event) {
    //traceComp->clearSelections();
    //if (traceLine->getFuncName() != "") traceLine->isLineSelected = true;
    //traceComp->repaint();
    //traceComp->resized();
    //traceComp->jumpToSelectedLine();
    //
}
//
TraceComponent::TraceLine::TraceLine(TraceParser::TraceLineStruct &lineInfoIn, juce::Colour &curFuncColour, TraceComponent& _traceComp){
    traceComp = &_traceComp;
    lineInfo = lineInfoIn;
    funcColour = curFuncColour;
    //
    defaultBorderColour = juce::Colours::black;
    selectedFuncBackgroundColour = juce::Colours::red;
    defaultFuncTextColour = juce::Colours::white;
    selectedFuncTextColour = juce::Colours::hotpink;
    //
    time = new TraceLineElement(std::to_string(lineInfo.time));
    addr = new TraceLineElement("0x" + lineInfo.addr);
    func = new TraceFuncElement(lineInfo.func, *this, *traceComp);
    instr = new TraceLineElement(lineInfo.instr);
    //
    time->backgroundColor = juce::Colours::grey;
    time->borderColor = defaultBorderColour;
    time->textColor = juce::Colours::whitesmoke;
    //
    addr->backgroundColor = juce::Colours::darkgrey;
    addr->borderColor = defaultBorderColour;
    addr->textColor = juce::Colours::whitesmoke;
    //
    instr->textJustification = juce::Justification::left;
    instr->borderColor = defaultBorderColour;
    instr->textXOffset = 15;
    //
    func->backgroundColor = funcColour;
    func->textColor = juce::Colours::white;
    //
    addAndMakeVisible(time);
    addAndMakeVisible(addr);
    addAndMakeVisible(func);
    addAndMakeVisible(instr);
    //
}
//
TraceComponent::TraceLine::~TraceLine(){
    delete(time);
    delete(addr);
    delete(func);
    delete(instr);
}
//
void TraceComponent::TraceLine::paint (juce::Graphics& g){
    g.fillAll (juce::Colours::white);   // clear the background

    g.setColour (juce::Colours::pink);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    //
    std::ostringstream ss;
    ss << lineInfo;
    g.drawText (ss.str(), getLocalBounds(),
                juce::Justification::left, true);   // draw some placeholder text
    if (isLineSelected) {
        func->fontStyle = juce::Font::FontStyleFlags::bold;
        func->textColor = selectedFuncTextColour;
    }
    else
    {
        func->fontStyle = juce::Font::FontStyleFlags::plain;
        func->textColor = defaultFuncTextColour;
    }
}
//
void TraceComponent::TraceLine::resized(){
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    grid.templateRows    = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (2)), Track (Fr (4)), Track (Fr (8)), Track (Fr (10))};
    grid.items = { juce::GridItem (time), juce::GridItem (func), juce::GridItem (addr), juce::GridItem (instr)};
    grid.performLayout (getLocalBounds());
}
//
string TraceComponent::TraceLine::getFuncName() {
    return func->getText();

}
//
void TraceComponent::TraceLine::setSelected(bool isSelected) {
    isLineSelected = isSelected;
}
//
bool TraceComponent::TraceLine::isSelected() {
    return isLineSelected;
}
//
TraceParser::TraceLineStruct TraceComponent::TraceLine::getLineInfo() {
    return lineInfo;
}
//
TraceComponent::TraceComponent(vector<TraceParser::TraceLineStruct>& vec, juce::Viewport& traceViewport)
{
    viewport = &traceViewport;
    //
    funcColours = new map<string, juce::Colour>();
    funcLines = new map<string, vector<int>>();
    //
    for (int i = 0; i < (&vec)->size(); i++) {
        if ((&vec)->at(i).func != "") funcColours->insert({(&vec)->at(i).func, juce::Colour((rand() % 255), (rand() % 255), (rand() % 255))});
    }
    //
    FTraceLines = new std::vector<std::unique_ptr<TraceLine>>();
    //
    juce::Colour curFuncColour = juce::Colours::black;
    for (int i = 0; i < (&vec)->size(); i++) {
        string funcName = (&vec)->at(i).func;
        if (funcName != "") {
            curFuncColour = funcColours->at(funcName);
            //
            if ((&vec)->at(i).isFirstLine) {
                if (funcLines->find(funcName) == funcLines->end()) {
                    vector<int> tmp;
                    tmp.push_back(i);
                    funcLines->insert({ funcName, tmp });
                }
                else {
                    vector<int>* tmp = &(funcLines->at(funcName));
                    tmp->push_back(i);
                }
            }
        }
        //
        TraceParser::TraceLineStruct newLineInfo = (&vec)->at(i);
        TraceLine *newLine = new TraceLine(newLineInfo, curFuncColour, *this);
        FTraceLines->emplace_back(newLine);
    }
    //
    for (int i = 0; i < FTraceLines->size(); i++){
        addAndMakeVisible(*(FTraceLines->at(i)));
    }
    //
    setSize(getParentWidth()/2, (int) FTraceLines->size()*lineHeight);
}
//
TraceComponent::~TraceComponent()
{
    delete(FTraceLines);
}
//
void TraceComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    //
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Trace", getLocalBounds(),
                juce::Justification::centred, true);
}
//
void TraceComponent::resized()
{
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    //
    for (int i = 0; i < FTraceLines->size(); i++){
        fb.items.add(juce::FlexItem (*FTraceLines->at(i)).withMinWidth (getWidth()).withMinHeight (lineHeight).withMaxHeight (lineHeight));
    }
    //
    fb.performLayout (getLocalBounds());
}
//
vector<int> TraceComponent::getFuncLines(const string &funcName){
    if (funcLines->find(funcName) == funcLines->end()) {
        return (*new vector<int>());
    } else return (funcLines->at(funcName));
}
//
void TraceComponent::setTopLine(int val) {
    int height = viewport->getHeight();
    int maxLinesOnScreen = height / lineHeight;
    int totalLines = FTraceLines->size();
    //
    if (val < 0) topLine = 0;
    else if (val > totalLines - maxLinesOnScreen)  topLine = totalLines - maxLinesOnScreen;
    else
    {
        topLine = val;
    }
    //
    viewport->setViewPosition(0, lineHeight * (topLine));
}
//
int TraceComponent::getTopLine() {
    return topLine;
}
//
void TraceComponent::setSelectedLine(int lineNumber) {
    int totalLines = FTraceLines->size();
    if ((lineNumber < 0) || (lineNumber >= totalLines)) return;
    //
    if (FTraceLines->at(lineNumber)->getFuncName() != "") {
        clearSelections();
        //
        FTraceLines->at(lineNumber)->setSelected(true);
        //
        int height = viewport->getHeight();
        int maxLinesOnScreen = height / lineHeight;
        setTopLine(lineNumber - maxLinesOnScreen / 4);
    }
    repaint();
    resized();
}
//
void TraceComponent::jumpToSelectedLine() {
    int lineNumber = 0;
    for (vector<std::unique_ptr<TraceLine>>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        if (it->get()->isSelected()) break;
        lineNumber++;
    }
    int height = viewport->getHeight();
    int maxLinesOnScreen = height / lineHeight;
    setTopLine(lineNumber - maxLinesOnScreen / 4);
}
//
void TraceComponent::clearSelections() {
    for (vector<std::unique_ptr<TraceLine>>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        it->get()->setSelected(false);
    }
}
//
int TraceComponent::getViewPosition() {
    return viewport->getViewPositionY();
}
//
void TraceComponent::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) {
    if (wheel.deltaY < 0) setTopLine((topLine + scrollStep));
    if (wheel.deltaY > 0) setTopLine((topLine - scrollStep));
}
