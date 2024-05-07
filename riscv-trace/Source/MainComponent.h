#pragma once

#include <JuceHeader.h>
#include "ProjectParser.hpp"
#include "ObjdumpParser.hpp"
#include "TraceComponent.h"
#include "CodeComponent.h"
#include "CreateProjectWindow.h"
#include "PerformanceAnalyzer.h"
#include <array>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
using namespace juce;
//
class MainComponent  : public Component, /*public ApplicationCommandTarget,*/ public MenuBarModel
{
public:
    //==============================================================================
    class TitlePanel: public Component
    {
    public:
        TitlePanel(const std::string &);
        ~TitlePanel() override;
        void paint (Graphics&) override;
        void resized() override;
    private:
        std::string text;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
    //
    class CodeSubComponent : public Component
    {
    public:
        CodeSubComponent(const std::vector<std::string>&, map<string, string>);
        ~CodeSubComponent() override;
        void paint(Graphics&) override;
        void resized() override;
        void selectFunc(const string&);
    private:
        struct MyTabbedComponent final : public TabbedComponent
        {
            MyTabbedComponent(vector<CodeComponent*>&);
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyTabbedComponent)
        };
        MyTabbedComponent *tabs;
        //CodeComponent* CodeWindow;
        vector<CodeComponent*> *codeWindows;
        //Viewport* CodeViewport;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeSubComponent)
    };
    //
    class AsSubComponent: public Component, public TextEditor::Listener
    {
    public:
        class ScrollableWindow: public Component
        {
        public:
            ScrollableWindow(vector<TraceParser::TraceLineStruct>& vec);
            ~ScrollableWindow();
            void paint (Graphics&) override;
            void resized() override;
            void scrollToFunc(const string &, int);
            int getNumberOfOccurances(const string &);
        private:
            TraceComponent* TraceWindow;
            Viewport* TraceViewport;
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollableWindow)
        };
        class OccurancesPanel: public Component, public Button::Listener
        {
        public:
            OccurancesPanel(AsSubComponent &);
            ~OccurancesPanel();
            void paint (Graphics&) override;
            void resized() override;
            void setPanelNumbers(int, int);
        private:
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
                //
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
            };
            //
            class OccuranceNumberPanel: public Component
            {
            public:
                OccuranceNumberPanel(int);
                ~OccuranceNumberPanel();
                void paint (Graphics&) override;
                void resized() override;
                void setNumber(int);
            private:
                int currentOccurNum;
                //
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OccuranceNumberPanel)
            };
            void incrButtonClicked();
            void decrButtonClicked();
            void buttonClicked(Button* button) override;
            //
            AsSubComponent *parent;
            OccuranceNumberPanel *numPanel;
            OccuranceNumberPanel* totalPanel;
            TitlePanel* outOfPanel;
            TitlePanel* occuranceTitlePanel;
            TextButton *incrButton;
            TextButton *decrButton;
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OccurancesPanel)
        };
        AsSubComponent(vector<TraceParser::TraceLineStruct> &vec, CodeSubComponent&);
        ~AsSubComponent() override;
        void paint (Graphics&) override;
        void resized() override;

        void selectFuncInCombobox(const string&);
        int getFuncOccuranceNumber(const string&);
        void setSelectedFunc(const string&);
        string getCurrentSelectedFunction();
        int getCurrentSelectedOccurance();
        void incrCurrentSelectedOccurance();
        void decrCurrentSelectedOccurance();
    private:
        void textEditorTextChanged(TextEditor&) override;
        CodeSubComponent* codeWindow;
        OccurancesPanel* occurancesPanel;
        TextEditor* searchField;
        ComboBox* functionsComboBox;
        ScrollableWindow* scrollableWindow;
        string selectedFunction = "";
        int selectedFuncOccurance = -1;
        map<string, int> *funcOccurances;
        map<string, int> *comboBoxItemID;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AsSubComponent)
    };
    //
    class AnalyzerSubComponent : public Component
    {
    public:
        //
        AnalyzerSubComponent(vector<TraceParser::TraceLineStruct> &, map<string, vector<string>> &, map<string, vector<string>> &, map<string, vector<string>> &);
        ~AnalyzerSubComponent() override;
        void paint(Graphics&) override;
        void resized() override;
    private:
        PerformanceAnalyzer* performanceAnalyzer;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerSubComponent)
    };
    //
    class PlaceholderSubComponent: public Component
    {
    public:
        PlaceholderSubComponent();
        ~PlaceholderSubComponent() override;
        void paint (Graphics&) override;
        void resized() override;
    private:
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaceholderSubComponent)
    };
    //
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void createProjectFile();
    void openProjectFile(const string);
    void chooseProjectFile();
    void saveProject();
    void closeProjectFile();
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/) override;
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
    //
    //==============================================================================
    TProjectParser::Project project;
    //
    MenuBarComponent *menuBar = nullptr;
    TitlePanel *asPanelTitle = nullptr;
    TitlePanel *codePanelTitle = nullptr;
    TitlePanel *analyzerPanelTitle = nullptr;
    //
    AsSubComponent *asPanel = nullptr;
    CodeSubComponent *codePanel = nullptr;
    AnalyzerSubComponent* analyzerPanel = nullptr;
    PlaceholderSubComponent *placeholderPanel = nullptr;
    //
    CreateProjectWindow* createProjWindow = nullptr;
    //
    std::unique_ptr<juce::FileChooser> chooser;
    File parentDirecory = File::getCurrentWorkingDirectory()/*.getParentDirectory().getParentDirectory()*/;
    String defaultFilepath = parentDirecory.getFullPathName();
    //
    int menuHeight = 20;
    bool projectOpened = false;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
