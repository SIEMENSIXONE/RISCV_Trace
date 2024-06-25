/*
  ==============================================================================

    ProjectInfoWindow.cpp
    Created: 24 Jun 2024 12:42:32pm
    Author:  veselyaev

  ==============================================================================
*/

#include "ProjectInfoWindow.h"

//==============================================================================
ProjectInfoPanel::ProjectInfoPanel(TProjectParser::Project& _project, TSettingsParser::Settings& _settings)
{
    //
    settings = _settings;
    projectInfoLines = new std::vector<std::unique_ptr<ProjectInfoPanelLine>>();
    //
    setLang(settings.lang);
    //
    projectInfoLines->emplace_back(new ProjectInfoPanelLine(ProjectFileTitleText, *this, _settings, _project, 0));
    projectInfoLines->emplace_back(new ProjectInfoPanelLine(TraceFilesTitleText, *this, _settings, _project, 1));
    projectInfoLines->emplace_back(new ProjectInfoPanelLine(ObjumpFilesTitleText, *this, _settings, _project, 2));
    projectInfoLines->emplace_back(new ProjectInfoPanelLine(CodeFilesTitleText, *this, _settings, _project, 3));
    //
    for (int i = 0; i < projectInfoLines->size(); i++) {
        addAndMakeVisible(*(projectInfoLines->at(i)));
    }
    //
    setSize(getParentWidth(), getParentHeight());
}
//
ProjectInfoPanel::~ProjectInfoPanel()
{
    delete(projectInfoLines);
}
//
void ProjectInfoPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(94, 60, 82));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    //
    setLang(settings.lang);
    for (int i = 0; i < projectInfoLines->size(); i++) {
        projectInfoLines->at(i)->resized();
        projectInfoLines->at(i)->repaint();
    }
}
//
void ProjectInfoPanel::resized()
{
    int offset = 15;

    juce::Rectangle<int> linesArea = juce::Rectangle<int>(offset, offset, getWidth() - 2 * offset, getHeight());
    //
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    //
    int lineHeight = (getHeight() - (offset * 2)) / projectInfoLines->size();
    //
    for (int i = 0; i < projectInfoLines->size(); i++) {
        fb.items.add(juce::FlexItem(*projectInfoLines->at(i)).withMinWidth((float)getWidth() - 2 * offset).withMinHeight((float)lineHeight).withMaxHeight((float)lineHeight));
    }
    //
    fb.performLayout(linesArea);
}
//
ProjectInfoPanel::ProjectInfoPanelLine::ProjectInfoPanelLine(juce::String& text, ProjectInfoPanel& _projectInfoPanel, TSettingsParser::Settings& _settings, TProjectParser::Project& _project, int index)
{
    projectInfoPanel = &_projectInfoPanel;
    settingIndex = index;
    title = new LineTitle(text, _settings);
    textEditor = new juce::TextEditor("Setting Editor");
    textEditor->setMultiLine(true);
    textEditor->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    textEditor->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    textEditor->setFont((float)_settings.interfaceFontSize);
    textEditor->setJustification(Justification::Flags::left);
    textEditor->setReadOnly(true);
    //
    String strAccum;
    int numberOfCodeFiles = _project.code.size();
    switch (settingIndex)
    {
    case 0:
        textEditor->setText(juce::String(_project.project.getFullPathName().toWideCharPointer()));
        break;
    case 1:
        textEditor->setText(juce::String(_project.trace.getFullPathName().toWideCharPointer()));
        break;
    case 2:
        textEditor->setText(juce::String(_project.objdump.getFullPathName().toWideCharPointer()));
        break;
    case 3:
        for (int i = 0; i < numberOfCodeFiles; i++) {
            strAccum += _project.code.at(i).getFullPathName().toWideCharPointer();
            if (i < numberOfCodeFiles - 1) strAccum += ",\n";
        }
        textEditor->setText(strAccum);
        break;
    default:
        break;
    }
    //
    addAndMakeVisible(title);
    addAndMakeVisible(textEditor);
}
//
ProjectInfoPanel::ProjectInfoPanelLine::~ProjectInfoPanelLine()
{
    if (title != nullptr) delete(title);
    if (textEditor != nullptr) delete(textEditor);
}
//
void ProjectInfoPanel::ProjectInfoPanelLine::paint(juce::Graphics& g)
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
void ProjectInfoPanel::ProjectInfoPanelLine::resized()
{
    title->setBounds(0, 0, getWidth(), getHeight() / 2);
    textEditor->setBounds(0, title->getBottom(), getWidth(), getHeight() / 2);
}
//
ProjectInfoPanel::ProjectInfoPanelLine::LineTitle::LineTitle(juce::String& _text, TSettingsParser::Settings& _settings)
{
    text = &_text;
    settings = &_settings;
    //
    fontSize = (float)settings->interfaceFontSize;
}
//
ProjectInfoPanel::ProjectInfoPanelLine::LineTitle::~LineTitle()
{
}
//
void ProjectInfoPanel::ProjectInfoPanelLine::LineTitle::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(37, 11, 46));
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);
    g.setFont(fontSize);
    g.drawText(*text, getLocalBounds(), juce::Justification::centred, true);
}
//
void ProjectInfoPanel::ProjectInfoPanelLine::LineTitle::resized()
{
    repaint();
}