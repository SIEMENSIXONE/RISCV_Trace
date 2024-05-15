/*
  ==============================================================================

    SettingsWindow.h
    Created: 13 May 2024 11:10:33am
    Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SettingsParser.hpp"

//==============================================================================
/*
*/
class SettingsPanel  : public juce::Component
{
public:
    SettingsPanel(juce::TextButton &);
    ~SettingsPanel() override;
    //
    void paint (juce::Graphics&) override;
    void resized() override;
    void loadSettings();
    void saveSettings();
    //
private:
    class SettingsPanelLine : public juce::Component, public juce::TextEditor::Listener
    {
    public:
        SettingsPanelLine(const std::string &, SettingsPanel&, TSettingsParser::Settings &, int);
        ~SettingsPanelLine() override;
        //
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
        void textEditorTextChanged(juce::TextEditor&) override;
        //
        class LineTitle : public juce::Component
        {
        public:
            LineTitle(const std::string&);
            ~LineTitle() override;
            //
            void paint(juce::Graphics&) override;
            void resized() override;
        private:
            std::string text;
        };
        //
        LineTitle *title = nullptr;
        juce::TextEditor *textEditor = nullptr;
        //
        int settingIndex = -1;
        TSettingsParser::Settings *settings = nullptr;
        SettingsPanel *settingsPanel = nullptr;
    };
    //
    vector<std::unique_ptr<SettingsPanelLine>> *settingsLines;
    juce::TextButton* saveButton = nullptr;
    TSettingsParser::Settings currentSettings;
    //
    int lineHeight = 25;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPanel)
};
//
class SettingsWindow : public juce::DocumentWindow
{
public:
    SettingsWindow(juce::String name, juce::TextButton &saveButton)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new SettingsPanel(saveButton), true);
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsWindow)
};
