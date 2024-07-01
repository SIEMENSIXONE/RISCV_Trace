/*
  ==============================================================================

    TextResources.h
    Created: 17 Jun 2024 9:01:38am
    Author:  veselyaev

  ==============================================================================
*/

#pragma once
#ifndef TextResources_h
#define TextResources_h
//
#include <JuceHeader.h>
using namespace juce;
//
static String currentLang = "ENG";
static String RussianLangTitle = String((std::wstring(L"�������")).c_str());
static String EnglishLangTitle = String((std::wstring(L"English")).c_str());
//
//Window titles
static String MainWindowTitle;
static String NewProjectWindowTitle;
static String SettingsWindowTitle;
static String AboutWindowTitle;
static String UsageWindowTitle;
static String GraphWindowTitle;
static String ProjectInfoWindowTitle;
//
static String WelcomeText;
static String WelcomeButtonOpenProject;
static String WelcomeButtonCreateProject;
//
//Section Titles
static String TraceSectionTitle;
static String CodeSectionTitle;
static String AnalyzerSectionTitle;
//
//Menu Titles
static String FileButtonText;
static String SettingsButtonText;
static String HelpButtonText;
//
//Menu dropdown options
    //File
    static String FileNewButtonText;
    static String FileOpenButtonText;
    static String FileAboutProjectButtonText;
    static String FileSaveButtonText;
    static String FileCloseButtonText;
    //
    //Settings
    static String SettingsGeneralButtonText;
    //
    //Help
    static String HelpAboutButtonText;
    static String HelpUsageButtonText;
//
//Trace section menu
static String FunctionsDropdownText;
static String SearchFieldText;
static String OccuranceText;
static String OutOfText;
//
//Analyzer section menu
static String TableTabText;
static String GraphTabText;
    //Table
    static String TableNameColomnTitleText;
    static String TableTimeColomnTitleText;
    static String TableTimePrecentColomnTitleText;
    static String TableCalledColomnTitleText;
    static String TableChildrenColomnTitleText;
    static String TableSelfColomnTitleText;
//
//Settings window
    //Options
    static String SettingsTraceFontSizeText;
    static String SettingsCodeFontSizeText;
    static String SettingsTableFontSizeText;
    static String SettingsInterfaceFontSizeText;
    static String ChooseLangText;
    static String SaveSettingsButtonText;
//
//Help window
    //Containing text
    static String HelpAboutText;
    static String HelpUsageText;
//
//Loading window
    static String LoadingText;
    static String LoadingSubText;
//
// Project closing
    static String ClosingProjectText;
    static String ClosingProjectSubText;
//
//Failed to open file window
    static String FailText;
    static String FailSubText;
//
// New project created window
    static String SuccessText;
    static String SuccessSubText;
    //
// Project creation window
    static String ChooseTraceFileText;
    static String ChooseObjdumpFileText;
    static String ChooseCodeFilesText;
    static String SaveButtonText;
    static String NoPathChoosenText;
// Project creation window
    static String ProjectFileTitleText;
    static String TraceFilesTitleText;
    static String ObjumpFilesTitleText;
    static String CodeFilesTitleText;
//Chooser
static String ChooseFileText;;
// No graphviz installed text
static String NoGraphviz;
//
static void setLang(String lang) {
    if (lang == String("ENG")) currentLang = lang;
    if (lang == String("RUS")) currentLang = lang;
    //
    if (currentLang == String("RUS")) {
        //
        //Window titles
        MainWindowTitle = String((std::wstring(L"RISCV Trace")).c_str());
        NewProjectWindowTitle = String((std::wstring(L"�������� �������")).c_str());
        SettingsWindowTitle = String((std::wstring(L"���������")).c_str());
        AboutWindowTitle = String((std::wstring(L"� ���������")).c_str());
        UsageWindowTitle = String((std::wstring(L"����������")).c_str());
        GraphWindowTitle = String((std::wstring(L"����")).c_str());
        ProjectInfoWindowTitle = String((std::wstring(L"� �������")).c_str());
        //
        WelcomeText = String((std::wstring(L"�������� ����� ������ ��� �������� ������������!")).c_str());
        WelcomeButtonCreateProject  = String((std::wstring(L"������� ������")).c_str());
        WelcomeButtonOpenProject  = String((std::wstring(L"������� ������")).c_str());

        //
        //Section Titles
        TraceSectionTitle = String((std::wstring(L"������ ����������")).c_str());
        CodeSectionTitle = String((std::wstring(L"�������� ���")).c_str());
        AnalyzerSectionTitle = String((std::wstring(L"������")).c_str());
        //
        //Menu Titles
        FileButtonText = String((std::wstring(L"����")).c_str());
        SettingsButtonText = String((std::wstring(L"��������")).c_str());
        HelpButtonText = String((std::wstring(L"������")).c_str());
        //
        //Menu dropdown options
            //File
            FileNewButtonText = String((std::wstring(L"�������")).c_str());
            FileOpenButtonText = String((std::wstring(L"�������..")).c_str());
            FileAboutProjectButtonText = String((std::wstring(L"� �������")).c_str());
            FileSaveButtonText = String((std::wstring(L"���������...")).c_str());
            FileCloseButtonText = String((std::wstring(L"�������")).c_str());
            //
            //Settings
            SettingsGeneralButtonText = String((std::wstring(L"�����")).c_str());
            //
            //Help
            HelpAboutButtonText = String((std::wstring(L"� ���������")).c_str());
            HelpUsageButtonText = String((std::wstring(L"����������")).c_str());
        //
        //Trace section menu
        FunctionsDropdownText = String((std::wstring(L"������� �������...")).c_str());
        SearchFieldText = String((std::wstring(L"�����...")).c_str());
        OccuranceText = String((std::wstring(L"���������")).c_str());
        OutOfText = String((std::wstring(L"��")).c_str());
        //
        //Analyzer section menu
        TableTabText = String((std::wstring(L"�������")).c_str());
        GraphTabText = String((std::wstring(L"����")).c_str());
        //Table
        TableNameColomnTitleText = String((std::wstring(L"���")).c_str());
        TableTimeColomnTitleText = String((std::wstring(L"�����")).c_str());
        TableTimePrecentColomnTitleText = String((std::wstring(L"�����\n(%)")).c_str());
        TableCalledColomnTitleText = String((std::wstring(L"������")).c_str());
        TableChildrenColomnTitleText = String((std::wstring(L"���������\n(%)")).c_str());
        TableSelfColomnTitleText = String((std::wstring(L"�����������\n(%)")).c_str());
        //
        //Settings window
            //Options
            SettingsTraceFontSizeText = String((std::wstring(L"������ ������ ������")).c_str());
            SettingsCodeFontSizeText = String((std::wstring(L"������ ������ ����")).c_str());
            SettingsTableFontSizeText = String((std::wstring(L"������ ������ ������ �������")).c_str());
            SettingsInterfaceFontSizeText = String((std::wstring(L"������ ������ ����������")).c_str());
            ChooseLangText = String((std::wstring(L"�������� ����...")).c_str());
            SaveSettingsButtonText = String((std::wstring(L"���������")).c_str());
        //
        //Help window
            //Containing text
            HelpAboutText = String((std::wstring(L"������������ ������ ���������� RISC-V\
        \n--------------------------------------------------------------------------\
    \n������������ ������ ���������� RISC-V ������������ ��� �������������� ��������, ���������� ��� ����������� RISC-V.\
    ������� ���� ��������� - ����������� � ������������� ������������� ����������, ����������� ��� ������� ������������������.\
    ��� ����������, �������� ���������������� ���������� ����������� � �������� �������� �����. ��������� ����� ����������� � ����� ������ �������������.")).c_str());
            HelpUsageText = String((std::wstring(L"���������� ������������ ������������� ������ ���������� RISC-V:\
        \n--------------------------------------------------------------------------\
        \n* ����->�����\
        \n* � ����������� ���� ���������� ���� � ������, ���������� ������, ������������ ������������� ���������, � ����� �������� ���. ������� ���� ���������� �������.\
        \n* �������� ��������� ������ � ������� ����->�������")).c_str());
            //
       //Loading window
            LoadingText = String((std::wstring(L"�������� ������� ")).c_str());
            LoadingSubText = String((std::wstring(L"����������, ���������, ���� ������ ����������.")).c_str());
            //
       // Project closing
            ClosingProjectText = String((std::wstring(L"�������� ������� ")).c_str());
            ClosingProjectSubText = String((std::wstring(L"����������, ���������, ���� ������ ���������.")).c_str());
            //
            //Failed to open file window
            FailText = String((std::wstring(L"�� ������� ������� ������...")).c_str());
            FailSubText = String((std::wstring(L"��������� ���� ��������� ��� ����.")).c_str());
            //
            // New project created window
            SuccessText = String((std::wstring(L"����� ������ ������� ������!")).c_str());
            SuccessSubText = String((std::wstring(L"�������� ��� � ������� ����->�������.")).c_str());
            //
            ChooseTraceFileText = String((std::wstring(L"�������� ���� � �������:")).c_str());
            ChooseObjdumpFileText = String((std::wstring(L"�������� ���� ����������:")).c_str());
            ChooseCodeFilesText = String((std::wstring(L"�������� ����� � �������� �����:")).c_str());
            SaveButtonText = String((std::wstring(L"���������...")).c_str());
            NoPathChoosenText = String((std::wstring(L"���� �� ������")).c_str());
            //
            ProjectFileTitleText = String((std::wstring(L"���� �������")).c_str());
            TraceFilesTitleText = String((std::wstring(L"���� ������")).c_str());
            ObjumpFilesTitleText = String((std::wstring(L"���� �������������")).c_str());
            CodeFilesTitleText = String((std::wstring(L"����� � �������� �����")).c_str());
            //
            ChooseFileText = String((std::wstring(L"�������� ����...")).c_str());
            //
            NoGraphviz = String((std::wstring(L"������� ����������� ����� ������� �������� ������ ��� ������������� graphviz.")).c_str());
    }
    else {
        //
        //Window titles
        MainWindowTitle = String((std::wstring(L"RISCV Trace")).c_str());
        NewProjectWindowTitle = String((std::wstring(L"New project")).c_str());
        SettingsWindowTitle = String((std::wstring(L"Settings")).c_str());
        AboutWindowTitle = String((std::wstring(L"About")).c_str());
        UsageWindowTitle = String((std::wstring(L"How to use")).c_str());
        GraphWindowTitle = String((std::wstring(L"Graph")).c_str());
        ProjectInfoWindowTitle = String((std::wstring(L"Project Info")).c_str());
        //
        WelcomeText = String((std::wstring(L"Create new project or open existing one!")).c_str());
        WelcomeButtonCreateProject  = String((std::wstring(L"Create project")).c_str());
        WelcomeButtonOpenProject  = String((std::wstring(L"Open project")).c_str());
        //
        //Section Titles
        TraceSectionTitle = String((std::wstring(L"Assembly Trace")).c_str());
        CodeSectionTitle = String((std::wstring(L"Code")).c_str());
        AnalyzerSectionTitle = String((std::wstring(L"Analyzer")).c_str());
        //
        //Menu Titles
        FileButtonText = String((std::wstring(L"File")).c_str());
        SettingsButtonText = String((std::wstring(L"Settings")).c_str());
        HelpButtonText = String((std::wstring(L"Help")).c_str());
        //
        //Menu dropdown options
            //File
            FileNewButtonText = String((std::wstring(L"New")).c_str());
            FileOpenButtonText = String((std::wstring(L"Open..")).c_str());
            FileAboutProjectButtonText = String((std::wstring(L"About project")).c_str());
            FileSaveButtonText = String((std::wstring(L"Save...")).c_str());
            FileCloseButtonText = String((std::wstring(L"Close")).c_str());
        //
        //Settings
        SettingsGeneralButtonText = String((std::wstring(L"General")).c_str());
        //
        //Help
        HelpAboutButtonText = String((std::wstring(L"About")).c_str());
        HelpUsageButtonText = String((std::wstring(L"Usage")).c_str());
        //
        //Trace section menu
        FunctionsDropdownText = String((std::wstring(L"Choose function...")).c_str());
        SearchFieldText = String((std::wstring(L"Search...")).c_str());
        OccuranceText = String((std::wstring(L"Occurance")).c_str());
        OutOfText = String((std::wstring(L"out of")).c_str());
        //
        //Analyzer section menu
        TableTabText = String((std::wstring(L"Table")).c_str());
        GraphTabText = String((std::wstring(L"Graph")).c_str());
        //Table
        TableNameColomnTitleText = String((std::wstring(L"Name")).c_str());
        TableTimeColomnTitleText = String((std::wstring(L"Time")).c_str());
        TableTimePrecentColomnTitleText = String((std::wstring(L"Time\n(%)")).c_str());
        TableCalledColomnTitleText = String((std::wstring(L"Called")).c_str());
        TableChildrenColomnTitleText = String((std::wstring(L"Children\n(%)")).c_str());
        TableSelfColomnTitleText = String((std::wstring(L"Self\n(%)")).c_str());
        //
        //Settings window
            //Options
            SettingsTraceFontSizeText = String((std::wstring(L"Trace Font Size")).c_str());
            SettingsCodeFontSizeText = String((std::wstring(L"Code Font Size")).c_str());
            SettingsTableFontSizeText = String((std::wstring(L"Analyzer Table Font Size")).c_str());
            SettingsInterfaceFontSizeText = String((std::wstring(L"Interface Font Size")).c_str());
            ChooseLangText = String((std::wstring(L"Choose language...")).c_str());
            SaveSettingsButtonText = String((std::wstring(L"Save")).c_str());
        //
        //Help window
            //Containing text
        HelpAboutText = String((std::wstring(L"RISC-V Trace Viewer\
        \n--------------------------------------------------------------------------\
    \nRISC-V Trace Viewer is a program for profiling software written for the RISC-V architecture.\
    The main objective of the program is a convenient and informative presentation\
    of the information necessary for performance analysis. It is an open source and free software. Any modifications and any models of use are allowed.")).c_str());
        HelpUsageText = String((std::wstring(L"RISC-V Trace profiler usage pipeline:\
        \n--------------------------------------------------------------------------\
        \n* File->New\
        \n* In opened menu set paths to trace info file, objdump file and code file. Choose where to save project file.\
        \n* Open created project file using File->Open")).c_str());
        //
        //Loading window
        LoadingText = String((std::wstring(L"Loading project ")).c_str());
        LoadingSubText = String((std::wstring(L"Please, wait for project to load.")).c_str());
        //
        // Project closing
        ClosingProjectText = String((std::wstring(L"CLosing project ")).c_str());
        ClosingProjectSubText = String((std::wstring(L"Please, wait for project to close.")).c_str());
        //
        //Failed to open file window
        FailText = String((std::wstring(L"Failed to open project file...")).c_str());
        FailSubText = String((std::wstring(L"File is wrong format or empty.")).c_str());
        //
        // New project created window
        SuccessText = String((std::wstring(L"New project successfully created!")).c_str());
        SuccessSubText = String((std::wstring(L"You can open it using File->Open.")).c_str());
        //
        ChooseTraceFileText = String((std::wstring(L"Choose trace file:")).c_str());
        ChooseObjdumpFileText = String((std::wstring(L"Choose objdump file:")).c_str());
        ChooseCodeFilesText = String((std::wstring(L"Choose code files:")).c_str());
        SaveButtonText = String((std::wstring(L"Save...")).c_str());
        NoPathChoosenText = String((std::wstring(L"No path set")).c_str());
        //
        ProjectFileTitleText = String((std::wstring(L"Project file")).c_str());
        TraceFilesTitleText = String((std::wstring(L"Trace file")).c_str());
        ObjumpFilesTitleText = String((std::wstring(L"Objdump file")).c_str());
        CodeFilesTitleText = String((std::wstring(L"Source code files")).c_str());
        //
        ChooseFileText = String((std::wstring(L"Choose file...")).c_str());
        //
        NoGraphviz = String((std::wstring(L"The graph display function is only available when graphviz is installed.")).c_str());
    }
}
#endif