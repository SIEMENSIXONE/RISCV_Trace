/*
  ==============================================================================

    UsageWindow.h
    Created: 31 May 2024 9:00:00pm
    Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class UsagePanel  : public juce::Component
{
public:
    UsagePanel();
    ~UsagePanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextEditor* textField;
    //
    std::string text;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UsagePanel)
};
//
class UsageWindow : public juce::DocumentWindow
{
public:
    UsageWindow(juce::String name)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new UsagePanel(), true);
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
