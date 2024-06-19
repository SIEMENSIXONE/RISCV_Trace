/*
  ==============================================================================

    CreateProjectWindow.h
    Created: 27 Apr 2024 10:43:46am
    Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProjectParser.hpp"
#include "TextResources.h"
#include "SettingsParser.hpp"

using namespace juce;
//==============================================================================
/*
*/
class CreateProjectPanel : public Component, Button::Listener
{
public:
    class MyAlertWindow : public AlertWindow
    {
    public:
        MyAlertWindow(const String& title,
            const String& message,
            MessageBoxIconType iconType,
            Component* comp);
        ~MyAlertWindow() override;
    };
    //
    class SetPathPanel : public Component, Button::Listener
    {
    public:
        SetPathPanel(vector<string>&, CreateProjectPanel &, TSettingsParser::Settings&, bool multipleFilesFlag = false);
        ~SetPathPanel() override;
        void paint(Graphics&) override;
        void resized() override;
    private:
        void addPath(const string&);
        void chooseProjectFile();
        void chooseProjectFiles();
        void buttonClicked(Button* button) override;
        File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        String defaultFilepath = parentDirecory.getFullPathName() +"/Resources/Files/";
        std::unique_ptr<juce::FileChooser> chooser;
        TextButton* chooseFileButton = nullptr;
        TextButton* chooseFilesButton = nullptr;
        TextEditor* textField = nullptr;
        vector<string> *vals;
        CreateProjectPanel* parent;
        //
        TSettingsParser::Settings* settings;
    };
    //
    class TitlePanel :public Component
    {
    public:
        TitlePanel(const string&, TSettingsParser::Settings&);
        ~TitlePanel() override;
        void paint(Graphics&) override;
        void resized() override;
        void setText(const string&);
        string getText();
    private:
        string text;
        TSettingsParser::Settings* settings;
    };
    //
    CreateProjectPanel(TSettingsParser::Settings&);
    ~CreateProjectPanel() override;
    void paint(Graphics&) override;
    void resized() override;
    void refresh();
    void saveProject();
    void showCongratsWindow();
    //
    File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
    String defaultSaveFilepath = parentDirecory.getFullPathName() + "/Projects/project.JSON";
    String defaultFilepath = parentDirecory.getFullPathName();
    //
    vector<string>* tracePath;
    vector<string> *codePaths;
    vector<string>* objdumpPath;
    //
    TSettingsParser::Settings* settings = nullptr;
    //
private:
    void buttonClicked(Button* button) override;
    //
    TextButton* saveProjectButton;
    std::unique_ptr<juce::FileChooser> chooser;
    //
    TitlePanel* spacerPanel;
    TitlePanel* titleTracePanel;
    SetPathPanel* setTracePathPanel;
    TitlePanel* titleCodePanel;
    SetPathPanel* setCodePathPanel;
    TitlePanel* titleObjdumpPanel;
    SetPathPanel* setObjdumpPathPanel;
    //
    ScopedMessageBox messageBox;
};
//
class CreateProjectWindow  : public juce::DocumentWindow
{
public:
    CreateProjectWindow(juce::String name, TSettingsParser::Settings& settings)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | closeButton)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new CreateProjectPanel(settings), true);
        setResizable(false, false);
        centreWithSize(getWidth(), getHeight());

        setSize(600, 400);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }
    //
    void closeButtonPressed() override
    {
        // This is called when the user tries to close this window. Here, we'll just
        // ask the app to quit when this happens, but you can change this to do
        // whatever you need.
        //JUCEApplication::getInstance()->systemRequestedQuit();
        setVisible(false);
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CreateProjectWindow)
};
