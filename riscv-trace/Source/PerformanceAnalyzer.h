/*
  ==============================================================================

	PerformanceAnalyzer.h
	Created: 2 May 2024 12:36:39pm
	Author:  veselyaev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TraceComponent.h"

using namespace juce;
//==============================================================================
/*
*/
class PerformanceAnalyzer : public juce::Component
{
public:
	//
	PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, vector<string>>&, map<string, vector<string>>&);
	~PerformanceAnalyzer() override;
	void paint(juce::Graphics&) override;
	void resized() override;
	//
private:
	//
	class ProfileTable : public Component, public TableListBoxModel {
	public:
		//
		ProfileTable(vector<array<std::string, 6>>& _data);
		~ProfileTable() override;
		void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
		void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override;
		void sortOrderChanged(int newSortColumnId, bool isForwards) override;
		int getNumRows() override;
		void resized() override;
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
		TableListBox box;
		MyLookAndFeel myLookAndFeel;
		vector<array<std::string, 6>>* data;

	};
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
