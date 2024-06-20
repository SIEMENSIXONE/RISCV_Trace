/*
  ==============================================================================

    CodeComponent.h
    Created: 14 Apr 2024 7:10:29pm
    Author:  Веселяев Михаил

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CodeParser.hpp"

//==============================================================================
/*
*/
class CodeComponent  : public juce::Component
{
public:
    CodeComponent(std::string&, std::string &, map<string, string>&);
    ~CodeComponent() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    string getFilename();
    bool selectFunc(const string&);
    void setFontSize(const int);
private:
    string filename;
    //
    juce::CodeDocument codeDocument;
    juce::CPlusPlusCodeTokeniser cppTokeniser;
    juce::CodeEditorComponent *codeEditor;
    //
    TCodeParser::CCode codeInfo;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeComponent)
};
