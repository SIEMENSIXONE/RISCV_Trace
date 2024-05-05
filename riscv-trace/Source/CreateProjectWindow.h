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

using namespace juce;
//==============================================================================
/*
*/
class CreateProjectPanel : public Component, Button::Listener
{
public:
    class SetPathPanel : public Component, Button::Listener
    {
    public:
        SetPathPanel(string &, CreateProjectPanel &);
        ~SetPathPanel() override;
        void paint(Graphics&) override;
        void resized() override;
    private:
        void setPath(const string&);
        void chooseProjectFile();
        void buttonClicked(Button* button) override;
        File parentDirecory = File::getCurrentWorkingDirectory()/*.getParentDirectory().getParentDirectory()*/;
        String defaultFilepath = parentDirecory.getFullPathName();
        std::unique_ptr<juce::FileChooser> chooser;
        TextButton* chooseFileButton;
        string* val;
        CreateProjectPanel* parent;
        //

    };
    //
    class TitlePanel :public Component
    {
    public:
        TitlePanel(const string&);
        ~TitlePanel() override;
        void paint(Graphics&) override;
        void resized() override;
        void setText(const string&);
        string getText();
    private:
        string text;
    };
    //
    CreateProjectPanel();
    ~CreateProjectPanel() override;
    void paint(Graphics&) override;
    void resized() override;
    void refresh();
    void saveProject();
    //
    string tracePath = "";
    string codePath = "";
    string objdumpPath = "";
    //
private:
    void buttonClicked(Button* button) override;
    //
    TextButton* saveProjectButton;
    File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    String defaultFilepath = parentDirecory.getFullPathName();
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
};
//
class CreateProjectWindow  : public juce::DocumentWindow
{
public:
    CreateProjectWindow(juce::String name)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new CreateProjectPanel(), true);
        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());

        setSize(500, 300);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
        //setFullScreen(true);
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