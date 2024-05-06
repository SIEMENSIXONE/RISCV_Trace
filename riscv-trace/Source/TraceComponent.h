/*
  ==============================================================================

    TraceComponent.h
    Created: 14 Apr 2024 12:42:18pm
    Author:  Веселяев Михаил

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "TraceParser.hpp"
//
//==============================================================================
/*
*/
//
//
class TraceComponent  : public juce::Component
{
public:
    const static int lineHeight = 18;
    //
    class TraceLine : public juce::Component
    {
    public:
        class TraceLineElement : public juce::Component
        {
        public:
            TraceLineElement(const std::string &);
            ~TraceLineElement();
            void paint (juce::Graphics&) override;
            void resized() override;
            string getText();
            //
            juce::String fontTypeface = "Courier New";
            float fontSize = (float) (lineHeight - 4);
            juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::plain;
            //
            juce::Colour borderColor = juce::Colours::black;
            juce::Colour backgroundColor= juce::Colours::white;
            juce::Colour textColor= juce::Colours::black;
            //
            juce::Justification textJustification = juce::Justification::centred;
            int textXOffset = 0;
        protected:
            std::string text;
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TraceLineElement)
        };
        //
        class TraceFuncElement : public TraceLineElement
        {
        public:
            TraceFuncElement(const std::string &, TraceLine &, TraceComponent &);
            ~TraceFuncElement();
            void paint (juce::Graphics&) override;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TraceFuncElement)
        private:
            TraceComponent* traceComp;
            TraceLine* traceLine;
        };
        //
        TraceLine(TraceParser::TraceLineStruct &, juce::Colour &, TraceComponent &);
        ~TraceLine() override;
        void paint (juce::Graphics&) override;
        void resized() override;
        string getFuncName();
        void setSelected(bool);
        bool isSelected();
        TraceParser::TraceLineStruct getLineInfo();
        //
    private:
        TraceParser::TraceLineStruct lineInfo;
        juce::Colour funcColour;
        juce::Colour defaultBorderColour;
        juce::Colour selectedFuncBackgroundColour;
        juce::Colour defaultFuncTextColour;
        juce::Colour selectedFuncTextColour;
        //
        TraceLineElement *time;
        TraceLineElement *addr;
        TraceFuncElement *func;
        TraceLineElement *instr;
        //
        bool isLineSelected = false;
        TraceComponent* traceComp;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TraceLine)
    };
    //
    TraceComponent(vector<TraceParser::TraceLineStruct> &, juce::Viewport&);
    ~TraceComponent() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    vector<int> getFuncLines(const string&);
    void setTopLine(int);
    int getTopLine();
    void setSelectedLine(int);
    void jumpToSelectedLine();
    void clearSelections();
    int getViewPosition();
private:
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
    juce::Colour headerBackgroundColour = juce::Colours::grey;
    map<string, juce::Colour> *funcColours;
    map<string, vector<int>> *funcLines;
    vector<TraceParser::TraceLineStruct> *TraceLinesVector;
    vector<std::unique_ptr<TraceLine>> *FTraceLines;
    //
    juce::Viewport* viewport;
    int scrollStep = 5;
    int topLine = 0;
    int selectedLine = -1;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TraceComponent)
};
