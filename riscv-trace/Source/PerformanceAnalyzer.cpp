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
PerformanceAnalyzer::PerformanceAnalyzer(vector<std::unique_ptr<TraceComponent::TraceLine>>& traceLines)
{
    FTraceLines = &traceLines;
    //
    for (vector<std::unique_ptr<TraceComponent::TraceLine>>::iterator it = FTraceLines->begin(); it != FTraceLines->end(); it++) {
        TraceParser::TraceLineStruct lineInfo = (it->get())->getLineInfo();
        //bool isCall = (it->get())->isFirstFuncLine();
        //lineInfo.
    }
}
//
PerformanceAnalyzer::~PerformanceAnalyzer()
{
    delete(FTraceLines);
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
}
