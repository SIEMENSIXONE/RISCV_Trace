/*
  ==============================================================================

    SettingsWindow.cpp
    Created: 13 May 2024 11:10:33am
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SettingsWindow.h"

//==============================================================================
SettingsPanel::SettingsPanel(juce::TextButton & _saveButton)
{
    saveButton = &_saveButton;
    addAndMakeVisible(saveButton);
    saveButton->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(37, 11, 46));
    saveButton->setBounds(0, 0, 100, 100);
    //
    loadSettings();
    //
    settingsLines = new std::vector<std::unique_ptr<SettingsPanelLine>>();
    //
    settingsLines->emplace_back(new SettingsPanelLine("Trace Font Size", *this, currentSettings, 0));
    settingsLines->emplace_back(new SettingsPanelLine("Code Font Size", *this, currentSettings, 1));
    settingsLines->emplace_back(new SettingsPanelLine("Analyzer Table Font Size", *this, currentSettings, 2));
    //
    for (int i = 0; i < settingsLines->size(); i++) {
        addAndMakeVisible(*(settingsLines->at(i)));
    }
    //
    setSize(getParentWidth(), getParentHeight());
}
//
SettingsPanel::~SettingsPanel()
{
    delete(settingsLines);
}
//
void SettingsPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(94, 60, 82));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}
//
void SettingsPanel::resized()
{
    int buttonOffset = 20;
    int buttonHeight = 50;
    juce::Rectangle<int> buttonArea = juce::Rectangle<int>(getWidth()/2 - (getWidth() / 4) / 2, getHeight() - buttonHeight - buttonOffset, getWidth() / 4, buttonHeight);
    juce::Rectangle<int> linesArea = juce::Rectangle<int>(getWidth() / 2 - (getWidth()) / 2, 0, getWidth(), getHeight() - 50);
    //
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    //
    for (int i = 0; i < settingsLines->size(); i++) {
        fb.items.add(juce::FlexItem(*settingsLines->at(i)).withMinWidth((float)getWidth()).withMinHeight((float) lineHeight).withMaxHeight((float) lineHeight));
    }
    //
    fb.performLayout(linesArea);
    saveButton->setBounds(buttonArea);
}
//
void SettingsPanel::loadSettings() {
    juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    currentSettings = TSettingsParser::getSettingsFromFile(workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
void SettingsPanel::saveSettings() {
    juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    TSettingsParser::saveSettingsToFile(currentSettings, workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
SettingsPanel::SettingsPanelLine::SettingsPanelLine(const std::string& text, SettingsPanel & _settingsPanel, TSettingsParser::Settings& _settings, int index)
{
    settingsPanel = &_settingsPanel;
    settingIndex = index;
    settings = &_settings;
    title = new LineTitle(text);
    textEditor = new juce::TextEditor("Setting Editor");
    textEditor->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    textEditor->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    //
    switch (settingIndex)
    {
    case 0:
        textEditor->setText(std::to_string(settings->traceFontSize));
        break;
    case 1:
        textEditor->setText(std::to_string(settings->codeFontSize));
        break;
    case 2:
        textEditor->setText(std::to_string(settings->analyzerFontSize));
        break;
    default:
        break;
    }
    //
    textEditor->addListener(this);
    //
    addAndMakeVisible(title);
    addAndMakeVisible(textEditor);
}
//
SettingsPanel::SettingsPanelLine::~SettingsPanelLine()
{
    if (title != nullptr) delete(title);
    if (textEditor != nullptr) delete(textEditor);
}
//
void SettingsPanel::SettingsPanelLine::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("Line", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}
//
void SettingsPanel::SettingsPanelLine::resized()
{
    title->setBounds(0, 0, getWidth() / 2, getHeight());
    textEditor->setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
}
//
void SettingsPanel::SettingsPanelLine::textEditorTextChanged(juce::TextEditor& editor) {
    //
    std::string stringVal = editor.getText().toStdString();
    //
    if (stringVal.length() <= 0) return;
    //
    char* p;
    strtol(stringVal.c_str(), &p, 10);
    if (*p) {
        return;
    }
    else {
        int val = stoi(stringVal);
        //
        if ((val <= 0) || (val >= 100)) return;
        //
        switch (settingIndex)
        {
        case 0:
            settings->traceFontSize = val;
            break;
        case 1:
            settings->codeFontSize = val;
            break;
        case 2:
            settings->analyzerFontSize = val;
            break;
        default:
            break;
        }
        settingsPanel->saveSettings();
    }
    //
}
//
SettingsPanel::SettingsPanelLine::LineTitle::LineTitle(const std::string& _text)
{
    text = _text;
}
//
SettingsPanel::SettingsPanelLine::LineTitle::~LineTitle()
{
}
//
void SettingsPanel::SettingsPanelLine::LineTitle::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(37, 11, 46));   // clear the background

    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText(text, getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}
//
void SettingsPanel::SettingsPanelLine::LineTitle::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}