/*
  ==============================================================================

    AboutWindow.h
    Created: 13 May 2024 4:16:13pm
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
class AboutPanel  : public juce::Component
{
public:
    AboutPanel(TSettingsParser::Settings&);
    ~AboutPanel() override;
    //
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextEditor* textField;
    std::string text;
    TSettingsParser::Settings* settings;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutPanel)
};
//
class AboutWindow : public juce::DocumentWindow
{
public:
    AboutWindow(juce::String name, TSettingsParser::Settings& _settings)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new AboutPanel(_settings), true);
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutWindow)
};
