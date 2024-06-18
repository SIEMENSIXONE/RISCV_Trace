/*
  ==============================================================================

    UsageWindow.h
    Created: 31 May 2024 9:00:00pm
    Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TextResources.h"
#include "SettingsParser.hpp"

//==============================================================================
/*
*/
class UsagePanel  : public juce::Component
{
public:
    UsagePanel(TSettingsParser::Settings&);
    ~UsagePanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextEditor* textField;
    std::string text;
    TSettingsParser::Settings* settings;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UsagePanel)
};
//
class UsageWindow : public juce::DocumentWindow
{
public:
    UsageWindow(juce::String name, TSettingsParser::Settings& _settings)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new UsagePanel(_settings), true);
        setResizable(false, false);
        centreWithSize(getWidth(), getHeight());
        //
        setSize(500, 300);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
        setFullScreen(false);

    }
    //
    void closeButtonPressed() override
    {
        setVisible(false);
    }
    //
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UsageWindow)
};
