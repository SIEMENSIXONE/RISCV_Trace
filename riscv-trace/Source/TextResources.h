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
    //String((std::wstring(L"����� �� �������")).c_str())
static String currentLang = "ENG";
//
//Window titles
static String MainWindowTitle;
static String NewProjectWindowTitle;
static String SettingsWindowTitle;
static String AboutWindowTitle;
static String UsageWindowTitle;
static String GraphWindowTitle;
//
static String WelcomeText;
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
static String tableTabText;
static String graphTabText;
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
//Failed to open file window
    static String FailText;
    static String FailSubText;
//
// New project created window
    static String SuccessText;
    static String SuccessSubText;
    //
// Project creation window
    static String chooseTraceFileText;
    static String chooseObjdumpFileText;
    static String chooseCodeFilesText;
    static String saveButtonText;
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
        //
        WelcomeText = String((std::wstring(L"�������� ����� ������ ��� �������� ������������!")).c_str());
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
            FileNewButtonText = String((std::wstring(L"�����")).c_str());
            FileOpenButtonText = String((std::wstring(L"�������..")).c_str());
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
        tableTabText = String((std::wstring(L"�������")).c_str());
        graphTabText = String((std::wstring(L"����")).c_str());
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
            LoadingText = String((std::wstring(L"��������...")).c_str());
            LoadingSubText = String((std::wstring(L"����������, ���������, ���� ������ ����������.")).c_str());
            //
            //Failed to open file window
            FailText = String((std::wstring(L"�� ������� ������� ������...")).c_str());
            FailSubText = String((std::wstring(L"��������� ���� ��������� ��� ����.")).c_str());
            //
            // New project created window
            SuccessText = String((std::wstring(L"����� ������ ������� ������!")).c_str());
            SuccessSubText = String((std::wstring(L"�������� ��� � ������� ����->�������.")).c_str());
            //
            chooseTraceFileText = String((std::wstring(L"�������� ���� � �������:")).c_str());
            chooseObjdumpFileText = String((std::wstring(L"�������� ���� ����������:")).c_str());
            chooseCodeFilesText = String((std::wstring(L"�������� ����� � �������� �����:")).c_str());
            saveButtonText = String((std::wstring(L"���������...")).c_str());
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
        //
        WelcomeText = String((std::wstring(L"Create new project or open existing one!")).c_str());
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
        tableTabText = String((std::wstring(L"Table")).c_str());
        graphTabText = String((std::wstring(L"Graph")).c_str());
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
        LoadingText = String((std::wstring(L"Loading...")).c_str());
        LoadingSubText = String((std::wstring(L"Please, wait for project to load.")).c_str());
        //
        //Failed to open file window
        FailText = String((std::wstring(L"Failed to open project file...")).c_str());
        FailSubText = String((std::wstring(L"File is wrong format or empty.")).c_str());
        //
        // New project created window
        SuccessText = String((std::wstring(L"New project successfully created!")).c_str());
        SuccessSubText = String((std::wstring(L"You can open it using File->Open.")).c_str());
        //
        chooseTraceFileText = String((std::wstring(L"Choose trace file:")).c_str());
        chooseObjdumpFileText = String((std::wstring(L"Choose objdump file:")).c_str());
        chooseCodeFilesText = String((std::wstring(L"Choose code files:")).c_str());
        saveButtonText = String((std::wstring(L"Save...")).c_str());
        //
        ChooseFileText = String((std::wstring(L"Choose file...")).c_str());
        //
        NoGraphviz = String((std::wstring(L"The graph display function is only available when graphviz is installed.")).c_str());
    }
}
#endif