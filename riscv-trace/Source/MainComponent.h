#pragma once

#include <JuceHeader.h>
#include "ProjectParser.hpp"
#include "ObjdumpParser.hpp"
#include "TraceComponent.h"
#include "CodeComponent.h"
#include "CreateProjectWindow.h"
#include "SettingsWindow.h"
#include "AboutWindow.h"
#include "UsageWindow.h"
#include <array>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
using namespace juce;
//
class MainComponent  : public Component, public MenuBarModel, public ComponentListener, public Button::Listener
{
public:
    //==============================================================================
    //
    class PerformanceAnalyzer : public juce::Component
    {
    public:
        //
        PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, set<string>>&, map<string, vector<string>>&, map<string, pair<string, string>>&, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent&);
        ~PerformanceAnalyzer() override;
        void paint(juce::Graphics&) override;
        void resized() override;
        string getGraphCode(map<string, juce::Colour>& _funcColoursMap);
        void tableSetSelectedRow(const string&);
        //
        void setFontSize(const int);
    private:
        //
        class ProfileTable : public Component, public TableListBoxModel {
        public:
            //
            ProfileTable(vector<array<std::string, 6>>& _data, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent &);
            ~ProfileTable() override;
            void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
            void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override;
            void sortOrderChanged(int newSortColumnId, bool isForwards) override;
            int getNumRows() override;
            void resized() override;
            void setSelectedRow(int);
            void setSelectedRow(const string&);
            void clearSelection();
            void refreshRowsColoursMap();
            //
            void setFontSize(const int);
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
            vector<array<std::string, 6>> *data;
            string selectedFunc = "";
            map<string, juce::Colour> funcColoursMap;
            map<string, juce::Colour> funcColoursTempMap;
            map<int, juce::Colour> rowsColoursMap;
            map<int, juce::Colour> rowsColoursTempMap;
            int selectedRow = -1;
            //
            int fontSize = 12;

        };
        //
        class ProfileGraphPanel : public Component, public Button::Listener {
        public:
            ProfileGraphPanel(const string&);
            ~ProfileGraphPanel() override;
            void paint(juce::Graphics&) override;
            void resized() override;
            void openSeparateGraphWindow();
        private:
            void buttonClicked(Button* button) override;
            void mouseDoubleClick(const MouseEvent& event) override;
            void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
            //
            class SeparateGraphPanel : public Component {
            public:
                SeparateGraphPanel(Image&);
                ~SeparateGraphPanel() override;
                void paint(juce::Graphics&) override;
                void resized() override;
                //
                int zoomStep = 100;
                int minZoom = 100;
                int maxZoom = 5000;
            private:
                class ImageComponent : public Component {
                public:
                    ImageComponent(SeparateGraphPanel&, Image&);
                    ~ImageComponent() override;
                    void paint(juce::Graphics&) override;
                    void resized() override;
                private:
                    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
                    //
                    SeparateGraphPanel* parent;
                    Image* picture;
                };
                //
                void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;
                //
                ImageComponent *imageComponent;
                Viewport *viewport;
            };
            //
            class SeparateGraphPanelWindow : public juce::DocumentWindow
            {
            public:
                SeparateGraphPanelWindow(juce::String name, Image& _image)
                    : DocumentWindow(name,
                        juce::Desktop::getInstance().getDefaultLookAndFeel()
                        .findColour(juce::ResizableWindow::backgroundColourId),
                        DocumentWindow::allButtons)
                {
                    setUsingNativeTitleBar(true);
                    setContentOwned(new SeparateGraphPanel(_image), true);
                    setResizable(true, true);
                    setFullScreen(false);
                    centreWithSize(getWidth() / 2, getHeight() / 2);
                    setResizeLimits(1280, 720, 1920, 1080);
                    setVisible(true);
                }
                //
                void closeButtonPressed() override
                {
                    setVisible(false);
                }
                //
            private:
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SeparateGraphPanelWindow)
            };
            //
            TextButton* openSeparateWindowButton = nullptr;
            //
            string pictureFileath;
            Image picture;
            SeparateGraphPanelWindow* separateWindow = nullptr;
            //
            int zoomStep = 150;
            int minZoom = 100;
            int maxZoom = 5000;
        };
        //
        struct MyTabbedComponent final : public TabbedComponent
        {
            MyTabbedComponent(ProfileTable&, Viewport&);
            void updateTabNames();
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyTabbedComponent)
        };
        //
        MainComponent* mainComponent;
        //
        vector<TraceParser::TraceLineStruct>* lines = nullptr;
        map<string, vector<string>>* funcAddrMap = nullptr;
        map<string, set<string>>* callingMap = nullptr;
        map<string, vector<string>>* callersMap = nullptr;
        map<string, pair<string, string>>* addrCallingCalledMap = nullptr;
        //
        map<string, int>* timesCalledMap;
        map< pair<string, string>, int>* timesCalledByMap; // caller - called - times
        map<string, long>* execTimeMapTotal;
        map<string, long>* execTimeMapTotalSelf;
        map<string, long>* execTimeMapOneInstance;
        //
        long totalTime = 0;
        //
        MyTabbedComponent* tabs = nullptr;
        ProfileTable* table = nullptr;
        ProfileGraphPanel* graph = nullptr;
        Viewport* graphViewport = nullptr;
        //
        File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        string graphTXTFilepath = parentDirecory.getFullPathName().toStdString() + "/Resources/graph.txt";
        string graphPNGFilepath = parentDirecory.getFullPathName().toStdString() + "/Resources/graph.png";
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformanceAnalyzer)
    };
    //
    class TitlePanel: public Component
    {
    public:
        TitlePanel(juce::String&);
        ~TitlePanel() override;
        void paint (Graphics&) override;
        void resized() override;
    private:
        juce::String *text;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
    //
    class CodeSubComponent : public Component
    {
    public:
        CodeSubComponent(const std::vector<std::string>&, map<string, string>, MainComponent&);
        ~CodeSubComponent() override;
        void paint(Graphics&) override;
        void resized() override;
        void selectFunc(const string&);
        //
        void setFontSize(const int);
    private:
        struct MyTabbedComponent final : public TabbedComponent
        {
            MyTabbedComponent(vector<CodeComponent*>&);
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyTabbedComponent)
        };
        MainComponent* mainComponent;
        MyTabbedComponent *tabs;
        //CodeComponent* CodeWindow;
        vector<CodeComponent*> *codeWindows;
        //
        int borderThickness = 2;
        //
        TitlePanel *titlePanel;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeSubComponent)
    };
    //
    class AnalyzerSubComponent : public Component
    {
    public:
        //
        AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, set<string>>&, map<string, vector<string>>&, map<string, pair<string, string>>&, map<string, juce::Colour> &, map<string, juce::Colour>&, MainComponent&);
        ~AnalyzerSubComponent() override;
        void paint(Graphics&) override;
        void resized() override;
        void setSelectedFunc(const string&);
        //
        void setFontSize(const int);
    private:
        MainComponent* mainComponent;
        PerformanceAnalyzer* performanceAnalyzer;
        int borderThickness = 2;
        //
        TitlePanel* titlePanel;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerSubComponent)
    };
    //
    class AsSubComponent: public Component, public TextEditor::Listener
    {
    public:
        class ScrollableWindow: public Component, public ScrollBar::Listener
        {
        public:
            ScrollableWindow(vector<TraceParser::TraceLineStruct>& vec, map<string, string>& addrFuncMap, map<string, juce::Colour>&, map<string, juce::Colour>&);
            ~ScrollableWindow();
            void paint (Graphics&) override;
            void resized() override;
            void scrollToFunc(const string &, int);
            void clearSelection();
            int getNumberOfOccurances(const string &);
            map<string, juce::Colour> getFuncColoursMap();
            //
            void setFontSize(const int);
            //
            int scrollBarWidth = 10;
        private:
            void scrollBarMoved(juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
            //
            TraceComponent* TraceWindow;
            ScrollBar* scrollBar = nullptr;
            //
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollableWindow)
        };
        //
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
                TitlePanel(String&);
                ~TitlePanel() override;
                void paint(Graphics&) override;
                void resized() override;
                void setText(const string&);
                string getText();
            private:
                String *text;
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
        AsSubComponent(vector<TraceParser::TraceLineStruct> &vec, map<string, string> & addrFuncMap, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent&);
        ~AsSubComponent() override;
        void paint (Graphics&) override;
        void resized() override;
        //
        void selectFuncInCombobox(const string&);
        int getFuncOccuranceNumber(const string&);
        void setSelectedFunc(const string&);
        string getCurrentSelectedFunction();
        int getCurrentSelectedOccurance();
        void incrCurrentSelectedOccurance();
        void decrCurrentSelectedOccurance();
        map<string, juce::Colour> getFuncColoursMap();
        //
        void setFontSize(const int);
    private:
        class MyTextEditor : public TextEditor
        {
        public:
            MyTextEditor(MainComponent &);
            ~MyTextEditor() override;
            //
        private:
            void returnPressed() override;
            //
            MainComponent* mainComponent;
        };
        //
        MainComponent* mainComponent;
        OccurancesPanel* occurancesPanel;
        MyTextEditor* searchField;
        ComboBox* functionsComboBox;
        ScrollableWindow* scrollableWindow;
        string selectedFunction = "";
        int selectedFuncOccurance = -1;
        int totalOccurances = -1;
        map<string, int> *funcOccurances;
        map<string, int> *comboBoxItemID;
        //
        LookAndFeel_V4 lf;
        //
        TitlePanel *titlePanel;
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
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setSelectedFunc(const string&, int callerID);
    //
    TSettingsParser::Settings currentSettings;
    //
private:
    void loadSettings();
    void createProjectFile();
    void openProjectFile(const string);
    void chooseProjectFile();
    void saveProject();
    void closeProjectFile();
    void openSettingsWindow();
    void openAboutWindow();
    void openUsageWindow();
    void updateCurrentSettings();
    void setFontSizes();
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/) override;
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
    void showAlertWindow();
    //
    void buttonClicked(Button*) override;
    //
    void componentVisibilityChanged(Component& component) override;
    //
    //==============================================================================
    TProjectParser::Project project;
    //
    TextButton *saveSettingsButton = nullptr;
    //
    MenuBarComponent *menuBar = nullptr;
    //
    AsSubComponent *asPanel = nullptr;
    CodeSubComponent *codePanel = nullptr;
    AnalyzerSubComponent* analyzerPanel = nullptr;
    PlaceholderSubComponent *placeholderPanel = nullptr;
    //
    CreateProjectWindow* createProjWindow = nullptr;
    SettingsWindow* settingsWindow = nullptr;
    AboutWindow* aboutWindow = nullptr;
    UsageWindow* usageWindow = nullptr;
    //
    StretchableLayoutManager verticalLayout;
    std::unique_ptr<StretchableLayoutResizerBar> verticalDividerBarLeft;
    std::unique_ptr<StretchableLayoutResizerBar> verticalDividerBarRight;
    //
    ScopedMessageBox messageBox;
    //
    std::unique_ptr<juce::FileChooser> chooser;
    File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
    String defaultFilepath = parentDirecory.getFullPathName() + "/Projects/";
    //
    LookAndFeel_V4 lf;
    //
    int menuHeight = 20;
    bool projectOpened = false;
    //
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
//
