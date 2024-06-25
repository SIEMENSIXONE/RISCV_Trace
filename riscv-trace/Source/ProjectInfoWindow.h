/*
  ==============================================================================

    ProjectInfoWindow.h
    Created: 24 Jun 2024 12:42:32pm
    Author:  veselyaev

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SettingsParser.hpp"
#include "ProjectParser.hpp"
#include "TextResources.h"

//==============================================================================
/*
*/
//
class ProjectInfoPanel : public juce::Component
{
public:
    ProjectInfoPanel(TProjectParser::Project&, TSettingsParser::Settings&);
    ~ProjectInfoPanel() override;
    //
    void paint(juce::Graphics&) override;
    void resized() override;
    //
private:
    class ProjectInfoPanelLine : public juce::Component
    {
    public:
        ProjectInfoPanelLine(juce::String&, ProjectInfoPanel&, TSettingsParser::Settings& , TProjectParser::Project&, int);
        ~ProjectInfoPanelLine() override;
        //
        void paint(juce::Graphics&) override;
        void resized() override;
    private:
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
            juce::String* text;
            TSettingsParser::Settings* settings = nullptr;
            float fontSize = 5;
        };
        //
        LineTitle* title = nullptr;
        juce::TextEditor* textEditor = nullptr;
        //
        int settingIndex = -1;
        TSettingsParser::Settings* settings = nullptr;
        ProjectInfoPanel* projectInfoPanel = nullptr;
    };
    //
    vector<std::unique_ptr<ProjectInfoPanelLine>>* projectInfoLines;
    TSettingsParser::Settings settings;
    //
    //int lineHeight = 75;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectInfoPanel)
};
//
class ProjectInfoWindow : public juce::DocumentWindow
{
public:
       ProjectInfoWindow(juce::String name, TProjectParser::Project& _project, TSettingsParser::Settings& _settings)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new ProjectInfoPanel(_project, _settings), true);
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectInfoWindow)
};