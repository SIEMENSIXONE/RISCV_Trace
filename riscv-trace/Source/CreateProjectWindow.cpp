/*
  ==============================================================================

    CreateProjectWindow.cpp
    Created: 27 Apr 2024 10:43:46am
    Author:  veselyaev

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CreateProjectWindow.h"
//==============================================================================
CreateProjectPanel::MyAlertWindow::MyAlertWindow(const String& title,
    const String& message,
    MessageBoxIconType iconType,
    Component* comp)
    :AlertWindow(title, message, iconType, comp)
{
    setColour(AlertWindow::ColourIds::backgroundColourId, juce::Colour(37, 11, 46));
    setColour(AlertWindow::ColourIds::outlineColourId, juce::Colours::white);
}
//
CreateProjectPanel::MyAlertWindow::~MyAlertWindow() {}
//
CreateProjectPanel::SetPathPanel::SetPathPanel(vector<string>& _val, CreateProjectPanel& _parent, TSettingsParser::Settings& _settings, bool multipleFilesFlag)
{
    parent = &_parent;
    vals = &_val;
    settings = &_settings;
    //
    textField = new TextEditor();
    textField->setReadOnly(true);
    textField->setMultiLine(true, false);
    textField->setText("");
    textField->setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(37, 11, 46));
    textField->getLookAndFeel().setColour(ScrollBar::thumbColourId, Colour(187, 148, 174));
    textField->setFont((float) settings->interfaceFontSize);
    //
    addAndMakeVisible(textField);
    //
    if (multipleFilesFlag) {
        chooseFilesButton = new TextButton("...");
        chooseFilesButton->addListener(this);
        chooseFilesButton->setColour(TextButton::ColourIds::buttonColourId, Colour(187, 148, 174));
        chooseFilesButton->setColour(TextButton::ColourIds::textColourOffId, juce::Colour(37, 11, 46));
        addAndMakeVisible(chooseFilesButton);
    }
    else {
        chooseFileButton = new TextButton("...");
        chooseFileButton->addListener(this);
        chooseFileButton->setColour(TextButton::ColourIds::buttonColourId, Colour(187, 148, 174));
        chooseFileButton->setColour(TextButton::ColourIds::textColourOffId, juce::Colour(37, 11, 46));
        addAndMakeVisible(chooseFileButton);
    }
}
//
CreateProjectPanel::SetPathPanel::~SetPathPanel()
{
    //
    if (textField != nullptr) {
        delete(textField);
    }
    //
    if (chooseFileButton != nullptr) {
        chooseFileButton->removeListener(this);
        delete(chooseFileButton);
    }
    //
    if (chooseFilesButton != nullptr) {
        chooseFilesButton->removeListener(this);
        delete(chooseFilesButton);
    }
}
//
void CreateProjectPanel::SetPathPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(94, 60, 82));
}
//
void CreateProjectPanel::SetPathPanel::resized()
{
    int space = 15;
    int buttonHeight = 28;
    int buttonWidth = 90;
    //
    if (textField != nullptr) textField->setBounds(0, 0, getWidth() - buttonWidth - space, getHeight());
    if (chooseFileButton != nullptr) chooseFileButton->setBounds(getWidth() - buttonWidth, 0, buttonWidth, buttonHeight);
    if (chooseFilesButton != nullptr) chooseFilesButton->setBounds(getWidth() - buttonWidth, 0, buttonWidth, buttonHeight);
}
//
void CreateProjectPanel::SetPathPanel::addPath(const string& filepath) {
    vals->push_back(filepath);
    //
    string text = "";
    int pathsNum = (int)vals->size();
    //
    if (pathsNum == 0) text = "No files choosen";
    else {
        for (int i = 0; i < pathsNum; i++) {
            text += vals->at(i);
            if (i < pathsNum - 1) text += ",\n";
        }
    }
    //
    textField->setText(text);
    parent->refresh();
}
//
void CreateProjectPanel::SetPathPanel::chooseProjectFile() {
    chooser = std::make_unique<FileChooser>(ChooseFileText, File(defaultFilepath), "*.c;*.h;*.txt");
    auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != File{}) {
                addPath((file.getFullPathName().toStdString()));
                parent->refresh();
            }
        });
}
//
void CreateProjectPanel::SetPathPanel::chooseProjectFiles() {
    chooser = std::make_unique<FileChooser>(ChooseFileText, File(defaultFilepath), "*.c;*.h;*.txt");
    auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
        {
            Array< File > files = fc.getResults();
            for (File file : files) {
                if (file != File{}) {
                    addPath((file.getFullPathName().toStdString()));
                    parent->refresh();
                }
            }
        });
}
//
void CreateProjectPanel::SetPathPanel::buttonClicked(Button * button)
{
    if (button == chooseFileButton)
    {
        chooseProjectFile();
    }
    //
    if (button == chooseFilesButton)
    {
        chooseProjectFiles();
    }
}
//
CreateProjectPanel::TitlePanel::TitlePanel(const string& _text, TSettingsParser::Settings& _settings) {
    text = _text;
    settings = &_settings;
}
//
CreateProjectPanel::TitlePanel::~TitlePanel() {
}
//
void CreateProjectPanel::TitlePanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(94, 60, 82));   // clear the background
    g.setColour(juce::Colours::white);
    g.setFont((float) settings->interfaceFontSize);
    g.drawText(text, getLocalBounds(),
        juce::Justification::left, true);   // draw some placeholder text
}
//
void CreateProjectPanel::TitlePanel::resized()
{}
//
CreateProjectPanel::CreateProjectPanel(TSettingsParser::Settings& _settings)
{
    settings = &_settings;
    setLang(settings->lang);
    //
    codePaths = new vector<string>();
    tracePath = new vector<string>();
    objdumpPath = new vector<string>();
    //
    spacerPanel = new TitlePanel("", *settings);
    titleTracePanel = new TitlePanel(chooseTraceFileText.toStdString(), *settings);
    titleCodePanel = new TitlePanel(chooseCodeFilesText.toStdString(), *settings);
    titleObjdumpPanel = new TitlePanel(chooseObjdumpFileText.toStdString(), *settings);
    //
    addAndMakeVisible(spacerPanel);
    addAndMakeVisible(titleTracePanel);
    addAndMakeVisible(titleCodePanel);
    addAndMakeVisible(titleObjdumpPanel);
    //
    setTracePathPanel = new SetPathPanel(*tracePath, *this, *settings);
    setCodePathPanel = new SetPathPanel(*codePaths, *this, *settings, true);
    setObjdumpPathPanel = new SetPathPanel(*objdumpPath, *this, *settings);
    //
    addAndMakeVisible(setTracePathPanel);
    addAndMakeVisible(setCodePathPanel);
    addAndMakeVisible(setObjdumpPathPanel);
    //
    saveProjectButton = new TextButton(saveButtonText);
    saveProjectButton->setEnabled(false);
    addAndMakeVisible(saveProjectButton);
    saveProjectButton->addListener(this);
    //
    saveProjectButton->setColour(TextButton::ColourIds::buttonColourId, Colour(187, 148, 174));
    saveProjectButton->setColour(TextButton::ColourIds::textColourOnId, juce::Colours::white);
    //
    setSize(getParentWidth(), getParentHeight());
}
//
CreateProjectPanel::~CreateProjectPanel()
{
    saveProjectButton->removeListener(this);
    delete(saveProjectButton);
    //
    delete(setTracePathPanel);
    delete(setCodePathPanel);
    delete(setObjdumpPathPanel);
    //
    delete(spacerPanel);
    delete(titleTracePanel);
    delete(titleCodePanel);
    delete(titleObjdumpPanel);
    //
}
//
void CreateProjectPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(94, 60, 82));
}
//
void CreateProjectPanel::resized()
{
    int border = 15;
    int width = getWidth();
    int height = getHeight();
    //
    Rectangle<int> area = Rectangle<int>(border, border, width - 2 * border, height - 2 * border);
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    Grid grid;
    grid.templateRows = { Track(Fr(4)), Track(Fr(4)), Track(Fr(4)), Track(Fr(4)), Track(Fr(4)), Track(Fr(20)), Track(Fr(4)), Track(Fr(3)), Track(Fr(4)) };
    grid.templateColumns = { Track(Fr(1)) };
    grid.items = { GridItem(titleTracePanel),GridItem(setTracePathPanel),GridItem(titleObjdumpPanel), GridItem(setObjdumpPathPanel), GridItem(titleCodePanel) ,
        GridItem(setCodePathPanel), GridItem(spacerPanel), GridItem(saveProjectButton).withWidth((float) width / 5).withJustifySelf(GridItem::JustifySelf::center), GridItem(spacerPanel)};
    grid.performLayout(area);
}
//
void CreateProjectPanel::refresh() {
    resized();
    repaint();
    //
    if ((tracePath->size() == 1) && (codePaths->size() > 0) && (objdumpPath->size() == 1)) {
        saveProjectButton->setEnabled(true);
    }
}
//
void CreateProjectPanel::saveProject() {
    chooser = std::make_unique<FileChooser>(ChooseFileText, File(defaultSaveFilepath), "*.JSON");
    auto chooserFlags = FileBrowserComponent::saveMode;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != File{})
            {
                TProjectParser::Project project;
                project.code = *codePaths;
                project.trace = tracePath->at(0);
                project.objdump = objdumpPath->at(0);
                TProjectParser::saveProjectToFile(project, file.getFullPathName().toStdString());
                showCongratsWindow();
                getParentComponent()->setVisible(false);
            }
        });
}
//
void CreateProjectPanel::buttonClicked(Button* button) {
    if (button == saveProjectButton) {
        saveProject();
    }
}
//
void CreateProjectPanel::showCongratsWindow()
{
    MyAlertWindow* processWnd = new MyAlertWindow(SuccessText,
        SuccessSubText,
        MessageBoxIconType::InfoIcon, this);
    processWnd->addButton("Ok", 0);
    processWnd->enterModalState(true, nullptr, true);
    MessageManager::callAsync([this, processWnd]() {});
}
