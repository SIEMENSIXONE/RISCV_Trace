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
static String RussianLangTitle = String((std::wstring(L"Русский")).c_str());
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
        NewProjectWindowTitle = String((std::wstring(L"Создание проекта")).c_str());
        SettingsWindowTitle = String((std::wstring(L"Настройки")).c_str());
        AboutWindowTitle = String((std::wstring(L"О программе")).c_str());
        UsageWindowTitle = String((std::wstring(L"Инструкция")).c_str());
        GraphWindowTitle = String((std::wstring(L"Граф")).c_str());
        ProjectInfoWindowTitle = String((std::wstring(L"О проекте")).c_str());
        //
        WelcomeText = String((std::wstring(L"Создайте новый проект или откройте существующий!")).c_str());
        WelcomeButtonCreateProject  = String((std::wstring(L"Создать проект")).c_str());
        WelcomeButtonOpenProject  = String((std::wstring(L"Открыть проект")).c_str());

        //
        //Section Titles
        TraceSectionTitle = String((std::wstring(L"Трасса исполнения")).c_str());
        CodeSectionTitle = String((std::wstring(L"Исходный код")).c_str());
        AnalyzerSectionTitle = String((std::wstring(L"Анализ")).c_str());
        //
        //Menu Titles
        FileButtonText = String((std::wstring(L"Файл")).c_str());
        SettingsButtonText = String((std::wstring(L"Натройки")).c_str());
        HelpButtonText = String((std::wstring(L"Помощь")).c_str());
        //
        //Menu dropdown options
            //File
            FileNewButtonText = String((std::wstring(L"Создать")).c_str());
            FileOpenButtonText = String((std::wstring(L"Открыть..")).c_str());
            FileAboutProjectButtonText = String((std::wstring(L"О проекте")).c_str());
            FileSaveButtonText = String((std::wstring(L"Сохранить...")).c_str());
            FileCloseButtonText = String((std::wstring(L"Закрыть")).c_str());
            //
            //Settings
            SettingsGeneralButtonText = String((std::wstring(L"Общее")).c_str());
            //
            //Help
            HelpAboutButtonText = String((std::wstring(L"О программе")).c_str());
            HelpUsageButtonText = String((std::wstring(L"Инструкция")).c_str());
        //
        //Trace section menu
        FunctionsDropdownText = String((std::wstring(L"Выбрать функцию...")).c_str());
        SearchFieldText = String((std::wstring(L"Поиск...")).c_str());
        OccuranceText = String((std::wstring(L"Вхождение")).c_str());
        OutOfText = String((std::wstring(L"из")).c_str());
        //
        //Analyzer section menu
        TableTabText = String((std::wstring(L"Таблица")).c_str());
        GraphTabText = String((std::wstring(L"Граф")).c_str());
        //Table
        TableNameColomnTitleText = String((std::wstring(L"Имя")).c_str());
        TableTimeColomnTitleText = String((std::wstring(L"Время")).c_str());
        TableTimePrecentColomnTitleText = String((std::wstring(L"Время\n(%)")).c_str());
        TableCalledColomnTitleText = String((std::wstring(L"Вызовы")).c_str());
        TableChildrenColomnTitleText = String((std::wstring(L"Вызванное\n(%)")).c_str());
        TableSelfColomnTitleText = String((std::wstring(L"Собственное\n(%)")).c_str());
        //
        //Settings window
            //Options
            SettingsTraceFontSizeText = String((std::wstring(L"Размер шрифта трассы")).c_str());
            SettingsCodeFontSizeText = String((std::wstring(L"Размер шрифта кода")).c_str());
            SettingsTableFontSizeText = String((std::wstring(L"Размер шрифта секции анализа")).c_str());
            SettingsInterfaceFontSizeText = String((std::wstring(L"Размер шрифта интерфейса")).c_str());
            ChooseLangText = String((std::wstring(L"Выберите язык...")).c_str());
            SaveSettingsButtonText = String((std::wstring(L"Сохранить")).c_str());
        //
        //Help window
            //Containing text
            HelpAboutText = String((std::wstring(L"Визуализатор трассы исполнения RISC-V\
        \n--------------------------------------------------------------------------\
    \nВизуализатор трассы исполнения RISC-V предназначен для профилирования программ, написанных под архитектуру RISC-V.\
    Главная цель программы - достоверное и информативное представление информации, необходимой для анализа производительности.\
    Это бесплатное, свободно распространяемое прораммное обеспечение с открытым исходным кодом. Разрешены любые модификации и любые модели использования.")).c_str());
            HelpUsageText = String((std::wstring(L"Инструкция пользователя визуализатора трассы исполнения RISC-V:\
        \n--------------------------------------------------------------------------\
        \n* Файл->Новый\
        \n* В открывшемся меню установите пути к файлам, содержащим трассу, ассемблерное представление программы, а также исходный код. Укажите путь сохранения проекта.\
        \n* Откройте созданный проект с помощью Файл->Открыть")).c_str());
            //
       //Loading window
            LoadingText = String((std::wstring(L"Загрузка проекта ")).c_str());
            LoadingSubText = String((std::wstring(L"Пожалуйста, подождите, пока проект загрузится.")).c_str());
            //
       // Project closing
            ClosingProjectText = String((std::wstring(L"Закрытие проекта ")).c_str());
            ClosingProjectSubText = String((std::wstring(L"Пожалуйста, подождите, пока проект закроется.")).c_str());
            //
            //Failed to open file window
            FailText = String((std::wstring(L"Не удалось открыть проект...")).c_str());
            FailSubText = String((std::wstring(L"Проектный файл поврежден или пуст.")).c_str());
            //
            // New project created window
            SuccessText = String((std::wstring(L"Новый проект успешно создан!")).c_str());
            SuccessSubText = String((std::wstring(L"Откройте его с помощью Файл->Открыть.")).c_str());
            //
            ChooseTraceFileText = String((std::wstring(L"Выберите файл с трассой:")).c_str());
            ChooseObjdumpFileText = String((std::wstring(L"Выберите файл ассемблера:")).c_str());
            ChooseCodeFilesText = String((std::wstring(L"Выберете файлы с исходным кодом:")).c_str());
            SaveButtonText = String((std::wstring(L"Сохранить...")).c_str());
            NoPathChoosenText = String((std::wstring(L"Путь не указан")).c_str());
            //
            ProjectFileTitleText = String((std::wstring(L"Файл проекта")).c_str());
            TraceFilesTitleText = String((std::wstring(L"Файл трассы")).c_str());
            ObjumpFilesTitleText = String((std::wstring(L"Файл дизассемблера")).c_str());
            CodeFilesTitleText = String((std::wstring(L"Файлы с исходным кодом")).c_str());
            //
            ChooseFileText = String((std::wstring(L"Выберите файл...")).c_str());
            //
            NoGraphviz = String((std::wstring(L"Функция отображения графа вызовов доступна только при установленном graphviz.")).c_str());
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