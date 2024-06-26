/*
  ==============================================================================

    AboutWindow.cpp
    Created: 13 May 2024 4:16:13pm
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AboutWindow.h"

//==============================================================================
AboutPanel::AboutPanel(TSettingsParser::Settings& _settings)
{
    settings = &_settings;
    //
    textField = new juce::TextEditor();
    textField->setFont((float) settings->interfaceFontSize);
    textField->setReadOnly(true);
    textField->setMultiLine(true);
    setLang(settings->lang);
    textField->setText(HelpAboutText);
    textField->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(37, 11, 46));
    textField->getLookAndFeel().setColour(ScrollBar::thumbColourId, Colour(187, 148, 174));
    //
    addAndMakeVisible(textField);
    setSize(getParentWidth(), getParentHeight());
}

AboutPanel::~AboutPanel()
{
    delete(textField);
}

void AboutPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(94, 60, 82));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont ((float) settings->interfaceFontSize);
    g.drawText("text", getLocalBounds(),
                juce::Justification::centred, true);
}

void AboutPanel::resized()
{
    int offset = 50;
    textField->setBounds(offset, offset, getWidth() - 2 * offset, getHeight() - 2 * offset);
}
