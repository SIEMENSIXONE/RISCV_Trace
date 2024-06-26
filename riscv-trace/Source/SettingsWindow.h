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
#include "TextResources.h"

//==============================================================================
/*
*/
class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel(TSettingsParser::Settings&);
    Font getComboBoxFont(ComboBox&) override;
    Font getPopupMenuFont() override;
private:
    TSettingsParser::Settings settings;
};
//
class SettingsPanel  : public juce::Component
{
public:
    SettingsPanel(juce::TextButton &, juce::String);
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
        SettingsPanelLine(juce::String &, SettingsPanel&, TSettingsParser::Settings &, int);
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
            LineTitle(juce::String&, TSettingsParser::Settings&);
            ~LineTitle() override;
            //
            void paint(juce::Graphics&) override;
            void resized() override;
        private:
            juce::String *text;
            TSettingsParser::Settings* settings = nullptr;
            float fontSize = 5;
        };
        //
        LineTitle *title = nullptr;
        juce::TextEditor *textEditor = nullptr;
        //
        int settingIndex = -1;
        TSettingsParser::Settings *settings = nullptr;
        SettingsPanel *settingsPanel = nullptr;
    };
    MyLookAndFeel* myLookAndFeel = nullptr;
    //
    vector<std::unique_ptr<SettingsPanelLine>> *settingsLines;
    ComboBox* langComboBox;
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
    SettingsWindow(juce::String name, juce::TextButton &saveButton, juce::String lang)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new SettingsPanel(saveButton, lang), true);
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
