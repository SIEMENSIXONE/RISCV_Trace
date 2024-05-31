/*
  ==============================================================================

    UsageWindow.cpp
    Created: 31 May 2024 9:00:00pm
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UsageWindow.h"

//==============================================================================
UsagePanel::UsagePanel()
{
    text = "RISCV Trace profiler usage pipeline:\
        \n--------------------------------------------------------------------------\
        \n* File->New\
        \n* In opened menu set paths to trace info file, objdump file and code file. Choose where to save project file.\
        \n* Open created project file";
    //
    textField = new juce::TextEditor();
    textField->setReadOnly(true);
    textField->setMultiLine(true);
    textField->setText(text);
    textField->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(37, 11, 46));
    //
    addAndMakeVisible(textField);
    setSize(getParentWidth(), getParentHeight());
}
//
UsagePanel::~UsagePanel()
{
    delete(textField);
}
//
void UsagePanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(94, 60, 82));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText("text", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}
//
void UsagePanel::resized()
{
    int offset = 50;
    textField->setBounds(offset, offset, getWidth() - 2 * offset, getHeight() - 2 * offset);
}
