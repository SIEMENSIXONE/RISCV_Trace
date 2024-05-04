/*
  ==============================================================================

    PerformanceAnalyzer.h
    Created: 2 May 2024 12:36:39pm
    Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TraceComponent.h"

//==============================================================================
/*
*/
class PerformanceAnalyzer  : public juce::Component
{
public:
    struct FuncData {
        int numOfCalls = 0;
        int executionTime = 0;
        int timeRelationToTotal = 0;
    };
    //
    PerformanceAnalyzer(vector<std::unique_ptr<TraceComponent::TraceLine>>&);
    ~PerformanceAnalyzer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    map<string, FuncData> performanceTable;
    int totalExecutionTime = 0;
    vector<std::unique_ptr<TraceComponent::TraceLine>>* FTraceLines;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerformanceAnalyzer)
};
