/*
  ==============================================================================

    CodeComponent.cpp
    Created: 14 Apr 2024 7:10:29pm
    Author:  Веселяев Михаил

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CodeComponent.h"

//==============================================================================
CodeComponent::CodeComponent(std::string &code, map<string, string>& funcAddrMap)
{
    codeEditor = new juce::CodeEditorComponent(codeDocument, &cppTokeniser);
    codeEditor->setColour(juce::CodeEditorComponent::ColourIds::backgroundColourId, juce::Colours::whitesmoke);
    codeEditor->setColour(juce::CodeEditorComponent::ColourIds::lineNumberBackgroundId, juce::Colours::grey);
    codeEditor->setColour(juce::CodeEditorComponent::ColourIds::lineNumberTextId, juce::Colours::white);
    codeEditor->setColour(juce::CodeEditorComponent::ColourIds::highlightColourId, juce::Colours::hotpink);
    codeEditor->getLookAndFeel().setColour(juce::ScrollBar::thumbColourId, juce::Colours::grey);
    codeEditor->setScrollbarThickness(10);
    codeEditor->loadContent(code);
    codeEditor->setReadOnly(true);
    ////////////////////////////////////////////////////////////////
    juce::CodeEditorComponent::ColourScheme  myColourScheme;
    myColourScheme.set("Error", juce::Colours::darkred);
    myColourScheme.set("Comment", juce::Colours::grey);
    myColourScheme.set("Keyword", juce::Colours::deeppink);
    myColourScheme.set("Operator", juce::Colours::black);
    myColourScheme.set("Identifier", juce::Colours::black);
    myColourScheme.set("Integer", juce::Colours::rebeccapurple);
    myColourScheme.set("Float", juce::Colours::orangered);
    myColourScheme.set("String", juce::Colours::firebrick);
    myColourScheme.set("Bracket", juce::Colours::black);
    myColourScheme.set("Punctuation", juce::Colours::black);
    myColourScheme.set("Preprocessor Text", juce::Colours::darkorange);
    codeEditor->setColourScheme(myColourScheme);
    ////////////////////////////////////////////////////////////////
    //
    codeInfo = TCodeParser::getCodeFromFile(code, funcAddrMap);
    //
    addAndMakeVisible(codeEditor);
}
//
CodeComponent::~CodeComponent()
{
    delete(codeEditor);
}
//
void CodeComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("CodeComponent", getLocalBounds(),
                juce::Justification::centred, true);
}
//
void CodeComponent::resized()
{
    codeEditor->setSize(getWidth(), getHeight());
}
//
void CodeComponent::selectFunc(const string& funcName) {
    if (codeInfo.functions.find(funcName) == codeInfo.functions.end()) {
        return;
    }
    //
    pair<int, int> funcInfo = codeInfo.functions.at(funcName);
    int pos = funcInfo.first;
    int line = funcInfo.second;
    if ((line - 1) >= 0) line = line - 1;
    codeEditor->scrollToLine(line);
    //
    juce::Range<int> range(pos, pos + (int) funcName.length());
    codeEditor->setHighlightedRegion(range);
    return;
}
