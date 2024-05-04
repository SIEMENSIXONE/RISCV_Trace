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
CreateProjectPanel::SetPathPanel::SetPathPanel(string& _val, CreateProjectPanel& _parent)
{
    parent = &_parent;
    val = &_val;
    chooseFileButton = new TextButton("Choose...");
    chooseFileButton->addListener(this);
    addAndMakeVisible(chooseFileButton);
}
//
CreateProjectPanel::SetPathPanel::~SetPathPanel()
{
    chooseFileButton->removeListener(this);
    //
    delete(chooseFileButton);
}
//
void CreateProjectPanel::SetPathPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);   // clear the background

    g.setColour(juce::Colours::ghostwhite);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    string text;
    //
    if (*val == "") text = "Choose...";
    else text = *val;
    //
    g.drawText(text, getLocalBounds(),
        juce::Justification::left, true);   // draw some placeholder text
}
//
void CreateProjectPanel::SetPathPanel::resized()
{
    int val = 5;
    chooseFileButton->setBounds(getWidth() - getWidth()/val, 0, getWidth() / val, getHeight());
}
//
void CreateProjectPanel::SetPathPanel::setPath(const string& filepath) {
    *val = filepath;
}
//
void CreateProjectPanel::SetPathPanel::chooseProjectFile() {
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.c;*.cpp;*.txt");
    auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != File{}) {
                setPath((file.getFullPathName().toStdString()));
                parent->refresh();
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
}
//
CreateProjectPanel::TitlePanel::TitlePanel(const string& _text) {
    text = _text;
}
//
CreateProjectPanel::TitlePanel::~TitlePanel() {
}
//
void CreateProjectPanel::TitlePanel::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText(text, getLocalBounds(),
        juce::Justification::left, true);   // draw some placeholder text
}
//
void CreateProjectPanel::TitlePanel::resized()
{}
//
CreateProjectPanel::CreateProjectPanel()
{
    spacerPanel = new TitlePanel("");
    titleTracePanel = new TitlePanel("Choose trace file:");
    titleCodePanel = new TitlePanel("Choose code file:");
    titleObjdumpPanel = new TitlePanel("Choose objdump file:");
    //
    addAndMakeVisible(spacerPanel);
    addAndMakeVisible(titleTracePanel);
    addAndMakeVisible(titleCodePanel);
    addAndMakeVisible(titleObjdumpPanel);
    //
    setTracePathPanel = new SetPathPanel(tracePath, *this);
    setCodePathPanel = new SetPathPanel(codePath, *this);
    setObjdumpPathPanel = new SetPathPanel(objdumpPath, *this);
    //
    addAndMakeVisible(setTracePathPanel);
    addAndMakeVisible(setCodePathPanel);
    addAndMakeVisible(setObjdumpPathPanel);
    //
    saveProjectButton = new TextButton("Save...");
    saveProjectButton->setEnabled(false);
    addAndMakeVisible(saveProjectButton);
    saveProjectButton->addListener(this);
    //ColourIds { buttonColourId = 0x1000100 , buttonOnColourId = 0x1000101 , textColourOffId = 0x1000102 , textColourOnId = 0x1000103 }
    saveProjectButton->setColour(TextButton::ColourIds::buttonColourId, juce::Colours::lightgrey);
    //saveProjectButton->setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgrey);
    //saveProjectButton->setColour(TextButton::ColourIds::textColourOffId, juce::Colours::lightgrey);
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("CreateProjectWindow", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}
//
void CreateProjectPanel::resized()
{
    Rectangle<int> area = Rectangle<int>(0, 0, getWidth(), getHeight());
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    Grid grid;
    grid.templateRows = { Track(Fr(2)), Track(Fr(1)), Track(Fr(2)), Track(Fr(1)), Track(Fr(2)), Track(Fr(1)), Track(Fr(2)), Track(Fr(1)), Track(Fr(2)) };
    grid.templateColumns = { Track(Fr(1)) };
    grid.items = { GridItem(titleTracePanel),GridItem(setTracePathPanel), GridItem(titleCodePanel) ,
        GridItem(setCodePathPanel),GridItem(titleObjdumpPanel), GridItem(setObjdumpPathPanel), GridItem(spacerPanel), GridItem(saveProjectButton), GridItem(spacerPanel)};
    grid.performLayout(area);
}
//
void CreateProjectPanel::refresh() {
    resized();
    repaint();
    //
    if ((tracePath != "") && (codePath != "") && (objdumpPath != "")) {
        saveProjectButton->setEnabled(true);
    }
}
//
void CreateProjectPanel::saveProject() {
    chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.traceproj");
    auto chooserFlags = FileBrowserComponent::saveMode;
    chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != File{})
            {
                TProjectParser::Project project;
                project.code = codePath;
                project.trace = tracePath;
                project.objdump = objdumpPath;
                TProjectParser::saveProjectToFile(project, file.getFullPathName().toStdString());
                getParentComponent()->setVisible(false);
            }
        });
}
//
void CreateProjectPanel::buttonClicked(Button* button) {
    if (button = saveProjectButton) {
        saveProject();
    }
}
