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
UsagePanel::UsagePanel(TSettingsParser::Settings& _settings)
{
    settings = &_settings;
    //
    textField = new juce::TextEditor();
    textField->setFont((float) settings->interfaceFontSize);
    textField->setReadOnly(true);
    textField->setMultiLine(true);
    setLang(settings->lang);
    textField->setText(HelpUsageText);
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
    g.setFont((float) settings->interfaceFontSize);
    g.drawText("text", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}
//
void UsagePanel::resized()
{
    int offset = 50;
    textField->setBounds(offset, offset, getWidth() - 2 * offset, getHeight() - 2 * offset);
}
