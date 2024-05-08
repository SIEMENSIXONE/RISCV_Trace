#pragma once

#include <JuceHeader.h>
#include "ProjectParser.hpp"
#include "ObjdumpParser.hpp"
#include "TraceComponent.h"
#include "CodeComponent.h"
#include "CreateProjectWindow.h"
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
    class PerformanceAnalyzer : public juce::Component
    {
    public:
        //
        PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, vector<string>>&, map<string, vector<string>>&, MainComponent&);
        ~PerformanceAnalyzer() override;
        void paint(juce::Graphics&) override;
        void resized() override;
        void tableSetSelectedRow(const string&);
        //
    private:
        //
        class ProfileTable : public Component, public TableListBoxModel {
        public:
            //
            ProfileTable(vector<array<std::string, 6>>& _data, MainComponent &);
            ~ProfileTable() override;
            void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
            void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override;
            void sortOrderChanged(int newSortColumnId, bool isForwards) override;
            int getNumRows() override;
            void resized() override;
            void setSelectedRow(int);
            void setSelectedRow(const string&);
            void clearSelection();
            //
        private:
            //
            class MyLookAndFeel : public LookAndFeel_V4
            {
            public:
                MyLookAndFeel();
                //
                void drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header, const String& columnName, int /*columnId*/, int width, int height, bool isMouseOver, bool isMouseDown, int columnFlags) override;
            };
            //
            void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
            void selectedRowsChanged(int lastRowSelected) override;
            //
            MainComponent *mainComponent;
            TableListBox box;
            MyLookAndFeel myLookAndFeel;
            vector<array<std::string, 6>>* data;
            string selectedFunc = "";
            int selectedRow = -1;

        };
        //
        MainComponent* mainComponent;
        //
        vector<TraceParser::TraceLineStruct>* lines = nullptr;
        map<string, vector<string>>* funcAddrMap = nullptr;
        map<string, vector<string>>* callingMap = nullptr;
        map<string, vector<string>>* callersMap = nullptr;
        //
        map<string, int>* timesCalledMap;
        map<string, int>* execTimeMapTotal;
        map<string, int>* execTimeMapTotalSelf;
        map<string, int>* execTimeMapOneInstance;
        //
        ProfileTable* table;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformanceAnalyzer)
    };
    //
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
    class AnalyzerSubComponent : public Component
    {
    public:
        //
        AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, vector<string>>&, map<string, vector<string>>&, MainComponent&);
        ~AnalyzerSubComponent() override;
        void paint(Graphics&) override;
        void resized() override;
        void setSelectedFunc(const string&);
    private:
        MainComponent* mainComponent;
        PerformanceAnalyzer* performanceAnalyzer;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerSubComponent)
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
        AsSubComponent(vector<TraceParser::TraceLineStruct> &vec, MainComponent&);
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
        string defaultSearchfieldText = "Search...";
        //
        void textEditorTextChanged(TextEditor&) override;
        MainComponent* mainComponent;
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

    //
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setSelectedFunc(const string&, int callerID);
    //
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
//
