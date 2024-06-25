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
MyLookAndFeel::MyLookAndFeel(TSettingsParser::Settings& _settings) {
    settings = _settings;
}
//
Font MyLookAndFeel::getComboBoxFont(ComboBox&){
    return((float) settings.interfaceFontSize);
}
//
Font MyLookAndFeel::getPopupMenuFont() {
    return((float)settings.interfaceFontSize);
}
//
SettingsPanel::SettingsPanel(juce::TextButton & _saveButton, juce::String lang)
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
    setLang(currentSettings.lang);
    //
    myLookAndFeel = new MyLookAndFeel(currentSettings);
    //
    settingsLines->emplace_back(new SettingsPanelLine(SettingsTraceFontSizeText, *this, currentSettings, 0));
    settingsLines->emplace_back(new SettingsPanelLine(SettingsCodeFontSizeText, *this, currentSettings, 1));
    settingsLines->emplace_back(new SettingsPanelLine(SettingsTableFontSizeText, *this, currentSettings, 2));
    settingsLines->emplace_back(new SettingsPanelLine(SettingsInterfaceFontSizeText, *this, currentSettings, 3));
    //
    langComboBox = new ComboBox("LanguageComboBox");
    langComboBox->setLookAndFeel(myLookAndFeel);
    langComboBox->setText(ChooseLangText);
    langComboBox->setJustificationType(Justification::centred);
    langComboBox->setColour(ComboBox::ColourIds::backgroundColourId, Colour(37, 11, 46));
    langComboBox->setEditableText(false);
    //
    langComboBox->addItem(EnglishLangTitle, 1);
    langComboBox->addItem(RussianLangTitle, 2);
    //
    langComboBox->onChange = [this] {
        if (langComboBox->getSelectedId() == 1) { 
            currentSettings.lang = "ENG"; 
            saveSettings();
        }
        if (langComboBox->getSelectedId() == 2) { 
            currentSettings.lang = "RUS"; 
            saveSettings();
        }
        };
    //
    addAndMakeVisible(langComboBox);
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
    delete(langComboBox);
    //
    if (myLookAndFeel != nullptr) delete(myLookAndFeel);
}
//
void SettingsPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(94, 60, 82));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    //
    setLang(currentSettings.lang);
    for (int i = 0; i < settingsLines->size(); i++) {
        settingsLines->at(i)->resized();
        settingsLines->at(i)->repaint();
    }
}
//
void SettingsPanel::resized()
{
    int offset = 30;
    int offsetDiffProportion = 3;
    int buttonOffset = 20;
    int buttonHeight = 50;
    int comboBoxHeight = lineHeight;
    juce::Rectangle<int> buttonArea = juce::Rectangle<int>(getWidth()/2 - (getWidth() / 4) / 2, getHeight() - buttonHeight - buttonOffset, getWidth() / 4, buttonHeight);
    juce::Rectangle<int> comboBoxArea = juce::Rectangle<int>(offset, offset, getWidth() - 2 * offset, comboBoxHeight);
    juce::Rectangle<int> linesArea = juce::Rectangle<int>(offsetDiffProportion * offset, comboBoxHeight + offset + offset, getWidth() - 2 * offsetDiffProportion * offset, getHeight() - buttonHeight - comboBoxHeight);
    //
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    //
    for (int i = 0; i < settingsLines->size(); i++) {
        fb.items.add(juce::FlexItem(*settingsLines->at(i)).withMinWidth((float)getWidth() - 2 * offsetDiffProportion * offset).withMinHeight((float) lineHeight).withMaxHeight((float) lineHeight));
    }
    //
    langComboBox->setBounds(comboBoxArea);
    fb.performLayout(linesArea);
    saveButton->setBounds(buttonArea);
}
//
void SettingsPanel::loadSettings() {
    juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
    currentSettings = TSettingsParser::getSettingsFromFile(workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
void SettingsPanel::saveSettings() {
    juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
    TSettingsParser::saveSettingsToFile(currentSettings, workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
SettingsPanel::SettingsPanelLine::SettingsPanelLine(juce::String& text, SettingsPanel & _settingsPanel, TSettingsParser::Settings& _settings, int index)
{
    settingsPanel = &_settingsPanel;
    settingIndex = index;
    settings = &_settings;
    title = new LineTitle(text, *settings);
    textEditor = new juce::TextEditor("Setting Editor");
    textEditor->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    textEditor->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    textEditor->setFont((float) settings->interfaceFontSize);
    textEditor->setJustification(Justification::Flags::centred);
    textEditor->setInputRestrictions(2, *(new String("0123456789")));
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
    case 3:      
        textEditor->setText(std::to_string(settings->interfaceFontSize));
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
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    //
    g.setColour(juce::Colours::white);
    g.drawText("Line", getLocalBounds(),
        juce::Justification::centred, true);
    //
    title->resized();
    textEditor->resized();
}
//
void SettingsPanel::SettingsPanelLine::resized()
{
    int titleWidth = getWidth() * 9 / 10;
    int textFieldWidth = getWidth() - titleWidth;
    //
    title->setBounds(0, 0, titleWidth, getHeight());
    textEditor->setBounds(titleWidth, 0, textFieldWidth, getHeight());
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
        case 3:
            settings->interfaceFontSize = val;
            break;
        default:
            break;
        }
        settingsPanel->saveSettings();
    }
    //
}
//
SettingsPanel::SettingsPanelLine::LineTitle::LineTitle(juce::String& _text, TSettingsParser::Settings& _settings)
{
    text = &_text;
    settings = &_settings;
    //
    fontSize = (float) settings->interfaceFontSize;
}
//
SettingsPanel::SettingsPanelLine::LineTitle::~LineTitle()
{
}
//
void SettingsPanel::SettingsPanelLine::LineTitle::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(37, 11, 46));
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);
    g.setFont(fontSize);
    g.drawText(*text, getLocalBounds(), juce::Justification::centred, true);
}
//
void SettingsPanel::SettingsPanelLine::LineTitle::resized()
{
    repaint();

}