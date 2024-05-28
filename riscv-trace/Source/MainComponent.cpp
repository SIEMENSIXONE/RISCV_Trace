#include "MainComponent.h"
//
//==============================================================================
using namespace juce;
//
MainComponent::PerformanceAnalyzer::PerformanceAnalyzer(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, set<string>>& _callingMap, map<string, vector<string>>& _callersMap, map<string, pair<string, string>>& _addrCallingCalledMap, map<string, juce::Colour>& _funcColoursMap, MainComponent& _mainComponent)
{
	mainComponent = &_mainComponent;
	lines = &_linesInfoVector;
	funcAddrMap = &_funcAddrMap;
	callingMap = &_callingMap;
	callersMap = &_callersMap;
	addrCallingCalledMap = &_addrCallingCalledMap;
	//
	timesCalledMap = new map<string, int>();
	execTimeMapTotal = new map<string, long>();
	execTimeMapTotalSelf = new map<string, long>();
	execTimeMapOneInstance = new map<string, long>();
	timesCalledByMap = new map< pair<string, string>, int>();
	//
	totalTime = (long) lines->size();
	std::vector<string> funcNameVector;
	for (map<string, vector<string>>::iterator it = funcAddrMap->begin(); it != funcAddrMap->end(); it++) funcNameVector.push_back(it->first);

	vector<string> curFuncs;
	map<string, int> timeMultipliers;
	for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
		execTimeMapTotal->insert({ *it, 0 });
		timeMultipliers.insert({ *it, 0 });
	}
	//
	string lastAddr = "";
	string lastFunc = "";
	for (vector<TraceParser::TraceLineStruct>::iterator it = lines->begin(); it != lines->end(); it++) {
		//
		if (it->func != "") {
			if ((it->isFirstLine) && (lastFunc != it->func)) (timeMultipliers.at(it->func))++;
			//
			for (vector<string>::iterator it1 = funcNameVector.begin(); it1 != funcNameVector.end(); it1++) {
				execTimeMapTotal->at(*it1) += timeMultipliers.at(*it1);
			}
			//
			if (it->isLastLine) {
				if (timeMultipliers.at(it->func) > 0) (timeMultipliers.at(it->func))--;
				if (curFuncs.size() > 0) curFuncs.pop_back();
			}
			//
			lastFunc = it->func;
		}
	}
	//
	for (vector<TraceParser::TraceLineStruct>::iterator it = lines->begin(); it != lines->end(); it++) {
		if (addrCallingCalledMap->find(it->addr) != addrCallingCalledMap->end()) {
			if (((it + 1) != lines->end()) && ((it + 1)->isFirstLine)) {
				pair<string, string> callerCalled = addrCallingCalledMap->at(it->addr);
				if (timesCalledByMap->find(callerCalled) == timesCalledByMap->end()) {
					timesCalledByMap->insert({ callerCalled, 1 });
				}
				else
				{
					(timesCalledByMap->at(callerCalled))++;
				}
			}
		}
	}
	//
	for (vector<TraceParser::TraceLineStruct>::iterator it = lines->begin(); it != lines->end(); it++) {
		if (it->isFirstLine) {
			string funcName = it->func;
			if (timesCalledMap->find(funcName) == timesCalledMap->end()) {
				timesCalledMap->insert({ funcName, 1 });
			}
			else {
				timesCalledMap->at(funcName)++;
			}
		}
		//
		if (it->func != "") {
			string funcName = it->func;
			if (execTimeMapTotalSelf->find(funcName) == execTimeMapTotalSelf->end()) {
				execTimeMapTotalSelf->insert({ funcName, 1 });
			}
			else {
				execTimeMapTotalSelf->at(funcName)++;
			}

		}
	}
	//
	for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
		string funcName = *it;
		if (execTimeMapTotalSelf->find(funcName) != execTimeMapTotalSelf->end()) {
			int fullExecTime = execTimeMapTotalSelf->at(funcName);
			int timesCalled = 1;
			if (timesCalledMap->find(funcName) != timesCalledMap->end()) {
				if (timesCalledMap->at(funcName) > 1) timesCalled = timesCalledMap->at(funcName);
			}
			//
			execTimeMapOneInstance->insert({ funcName, fullExecTime / timesCalled });
		}
	}
	//
	const int columnsNum = 6;
	vector<array<std::string, columnsNum>>* tableData = new vector<array<std::string, columnsNum>>();
	//
	for (vector<string>::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
		string funcName = *it;
		array<std::string, columnsNum> newRow;
		newRow.fill("-");
		//
		//name
		newRow.at(0) = funcName;
		//
		//time
		int totalExecTime = 0;
		if (execTimeMapTotal->find(funcName) != execTimeMapTotal->end()) totalExecTime = execTimeMapTotal->at(funcName);
		newRow.at(1) = "-";
		if (totalExecTime != 0) newRow.at(1) = to_string(totalExecTime);
		//
		//time %
		newRow.at(2) = "-";
		if (totalExecTime != 0) newRow.at(2) = to_string((totalExecTime * 100) / totalTime);
		//red = 255, 0, 0
		//green = 0, 255, 0
		long maxSelfTime = 0;
		//
		for (map<string, long>::iterator it1 = execTimeMapTotalSelf->begin(); it1 != execTimeMapTotalSelf->end(); it1++) if (it1->second > maxSelfTime) maxSelfTime = it1->second;
		//
		double selfTime = 0;
		if (execTimeMapTotalSelf->find(funcName) != execTimeMapTotalSelf->end()) selfTime = execTimeMapTotalSelf->at(funcName);
		double precentage = selfTime / maxSelfTime;
		//if ((int) precentage == 0) precentage = 1;
		int maxColour = 200;
		_funcColoursMap.insert({ funcName, Colour((juce::uint8) (maxColour * precentage), (juce::uint8) (maxColour - (maxColour * precentage)),(juce::uint8) (255 * precentage / 2)) });
		//
		//called
		int timesCalled = 0;
		if (timesCalledMap->find(funcName) != timesCalledMap->end()) timesCalled = timesCalledMap->at(funcName);
		newRow.at(3) = "-";
		if (timesCalled != 0) newRow.at(3) = to_string(timesCalled);
		//
		//self
		if (execTimeMapTotalSelf->find(funcName) != execTimeMapTotalSelf->end()) {

			// �� ������ �������
			newRow.at(4) = to_string(((totalExecTime - execTimeMapTotalSelf->at(funcName)) * 100) / totalTime);
			newRow.at(5) = to_string((execTimeMapTotalSelf->at(funcName) * 100) / totalTime);


			//// �� ������������ ������� �������
			//newRow.at(4) = to_string(((totalExecTime - execTimeMapTotalSelf->at(funcName)) * 100) / totalExecTime);
			//newRow.at(5) = to_string((execTimeMapTotalSelf->at(funcName) * 100) / totalExecTime);
		}
		//
		if (totalExecTime != 0) tableData->push_back(newRow);
	}
	//
	table = new ProfileTable(*tableData, _funcColoursMap, *mainComponent);
	table->setBounds(0, 0, getWidth(), getHeight());
	//
	std::ofstream file;
	file.open(graphTXTFilepath, std::ios::out);
	//
	if (file.is_open()) {
		file << getGraphCode(_funcColoursMap);
	}
	//
	file.close();
	//
	std::string command = "dot -Tpng -o " + graphPNGFilepath + " " + graphTXTFilepath;
	const char* c = command.c_str();
	std::system(c);
	//
	graph = new ProfileGraphPanel(graphPNGFilepath);
	//
	graphViewport = new Viewport("graphViewport");
	graphViewport->setViewedComponent(graph, false);
	graphViewport->setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
	graphViewport->setScrollBarsShown(true, true);
	graphViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(187, 148, 174));
	graphViewport->getHorizontalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(187, 148, 174));
	graphViewport->setScrollBarThickness(10);
	graphViewport->setBounds(0, 0, getWidth(), getHeight());
	//
	tabs = new MyTabbedComponent(*table, *graphViewport);
	addAndMakeVisible(tabs);
	tabs->setBounds(0, 0, getWidth(), getHeight());
}
//
MainComponent::PerformanceAnalyzer::~PerformanceAnalyzer()
{
	if (table != nullptr) delete(table);
	if (graphViewport != nullptr) delete(graphViewport);
	if (graph != nullptr) delete(graph);
	if (tabs != nullptr) delete(tabs);
	delete(timesCalledMap);
	delete(execTimeMapTotalSelf);
}
//
void MainComponent::PerformanceAnalyzer::paint(juce::Graphics& g)
{
	g.fillAll(Colour(94, 60, 82));
	//
	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
	g.drawText("PerformanceAnalyzer", getLocalBounds(),
		juce::Justification::centred, true);
}
//
void MainComponent::PerformanceAnalyzer::resized()
{
	int borderSize = 0;
	table->setBounds(borderSize, borderSize, getWidth() - 2 * borderSize, getHeight() - 2 * borderSize);
	tabs->setBounds(borderSize, borderSize, getWidth() - 2 * borderSize, getHeight() - 2 * borderSize);
	graph->setBounds(borderSize, borderSize, getWidth() - 2 * borderSize, getHeight() - 2 * borderSize);
}
//
string MainComponent::PerformanceAnalyzer::getGraphCode(map<string, juce::Colour>& _funcColoursMap) {
	string result = "";
	//
	string head = "digraph \"[calls]\" {\
        fontname = \"Helvetica,Arial,sans-serif\"\
            node[fontname = \"Helvetica,Arial,sans-serif\"]\
            edge[fontname = \"Helvetica,Arial,sans-serif\"]\
            node[style = filled fillcolor = \"#f8f8f8\"]\n";
	//
	string tail = "}";
	//
	string nodes = "";
	//
	string edges = "";
	//
	map<string, int> nodeNumMap;
	//
	int ctr = 1;
	set<string> funcNameVector;
	for (map< pair<string, string>, int>::iterator it = timesCalledByMap->begin(); it != timesCalledByMap->end(); it++) if (execTimeMapTotal->find(it->first.first) != execTimeMapTotal->end()) funcNameVector.insert(it->first.first);
	for (map< pair<string, string>, int>::iterator it = timesCalledByMap->begin(); it != timesCalledByMap->end(); it++) if (execTimeMapTotal->find(it->first.second) != execTimeMapTotal->end()) funcNameVector.insert(it->first.second);
	//
	for (set<string> ::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) nodeNumMap.insert({ *it, ctr++ });
	//
	for (set<string> ::iterator it = funcNameVector.begin(); it != funcNameVector.end(); it++) {
		string func = *it;
		string nodeNumber = to_string(nodeNumMap.at(func));
		juce::Colour funcColour = juce::Colours::grey;
		//
		if (_funcColoursMap.find(func) != _funcColoursMap.end()) funcColour = _funcColoursMap.at(func);
		//
		string funcColourString = (funcColour.toDisplayString(false)).toStdString();
		//
		double totalPrecentage = ((double)execTimeMapTotal->at(func) * 100) / (double)totalTime;
		std::stringstream streamTotal;
		streamTotal << std::fixed << std::setprecision(2) << totalPrecentage;
		std::string totalPrecentageString = streamTotal.str();
		//
		double selfPrecentage = ((double)execTimeMapTotalSelf->at(func) * 100) / (double)totalTime;
		std::stringstream streamSelf;
		streamSelf << std::fixed << std::setprecision(2) << selfPrecentage;
		std::string selfPrecentageString = streamSelf.str();
		//
		//double childPrecentage = (((double)totalTime - (double) execTimeMapTotalSelf->at(func)) * 100) / (double)totalTime;
		//std::stringstream streamChild;
		//streamChild << std::fixed << std::setprecision(2) << childPrecentage;
		//std::string childPrecentageString = streamChild.str();
		//
		string node = "N" + nodeNumber + "[label = \"" + func
			+ "\n---------------------------------" +
			+"\nTotal: " + totalPrecentageString + "%"
			+ "\nSelf: " + selfPrecentageString + "%"
			//+ "\nChildren: " + childPrecentageString + "%"
			+ "\" id = \"node" + nodeNumber + "\" fontsize = 18 shape = box tooltip = \"" + func + "\"  color = \"#ffffff\" fillcolor = \"#" + funcColourString + "\"]";
		nodes += node + "\n";
	}
	//
	for (map<string, set<string>> ::iterator it = callingMap->begin(); it != callingMap->end(); it++) {
		string func = it->first;
		if (nodeNumMap.find(func) != nodeNumMap.end()) {
			string nodeNumber = to_string(nodeNumMap.at(func));
			//
			set<string> calls = it->second;
			for (set<string> ::iterator iter = calls.begin(); iter != calls.end(); iter++) {
				string calledFunc = *iter;
				string nodeNumberCalled = "";
				if (nodeNumMap.find(calledFunc) != nodeNumMap.end()) {
					nodeNumberCalled = to_string(nodeNumMap.at(calledFunc));
					pair<string, string> callerCalled = make_pair(func, calledFunc);
					int tmp = 0;
					if (timesCalledByMap->find(callerCalled) != timesCalledByMap->end()) tmp = timesCalledByMap->at(callerCalled);
					if (tmp > 0) {
						string timesCalled = to_string(tmp);
						string edge = "N" + nodeNumber + "->N" + nodeNumberCalled + "[label = \"" + timesCalled + "\" weight = 94 penwidth = 5 color = \"#5E3C52\" tooltip = \"" + func + " -> " + calledFunc + " (" + timesCalled + ")\" labeltooltip = \"" + func + " -> " + calledFunc + " (" + timesCalled + ")\"]";
						edges += edge + "\n";
					}
				}
			}
		}
	}
	//
	result = head + nodes + edges + tail;
	//
	return result;
}
//
void MainComponent::PerformanceAnalyzer::tableSetSelectedRow(const string& funcName) {
	table->setSelectedRow(funcName);
}
//
void MainComponent::PerformanceAnalyzer::setFontSize(const int size) {
	if (size < 0) return;
	//
	table->setFontSize(size);
}
//
MainComponent::PerformanceAnalyzer::ProfileGraphPanel::ProfileGraphPanel(const string& _pictureFileath) {
	pictureFileath = _pictureFileath;
	File pictureFile(pictureFileath);
	picture = ImageFileFormat::loadFrom(pictureFile);
	setSize(max(getParentWidth(), picture.getWidth()), max(getParentHeight(), picture.getHeight()));
	//
	//openSeparateWindowButton = new TextButton("Open in separate window");
	//openSeparateWindowButton->addListener(this);
	//openSeparateWindowButton->setBounds(0, 0, 80, 50);
	//addAndMakeVisible(openSeparateWindowButton);
	//
	addMouseListener(this, true);
}
//
MainComponent::PerformanceAnalyzer::ProfileGraphPanel::~ProfileGraphPanel() {
	if (separateWindow != nullptr) delete(separateWindow);
	//openSeparateWindowButton->removeListener(this);
	//if (openSeparateWindowButton != nullptr) delete(openSeparateWindowButton);
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::paint(juce::Graphics& g) {
	g.fillAll(Colours::white);
	g.setOpacity(1.0f);
	if (picture.isNull())  g.drawText("Error! Check if graphviz is installed!", getLocalBounds(), Justification::centred, true);
	else g.drawImage(picture, 0, 0, picture.getWidth(), picture.getHeight(), 0, 0, picture.getWidth(), picture.getHeight());
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::resized() {
	setSize(max(getParentWidth(), picture.getWidth()), max(getParentHeight(), picture.getHeight()));
	//if (openSeparateWindowButton != nullptr) openSeparateWindowButton->setBounds(0, offset, buttonWidth, buttonHeight);
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::openSeparateGraphWindow() {
	if (separateWindow != nullptr) delete(separateWindow);
	separateWindow = new SeparateGraphPanelWindow("Graph window", picture);
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::buttonClicked(Button* /*button*/) {

	//if (button == openSeparateWindowButton)
	//{
	//    separateWindow = new SeparateGraphPanelWindow("Graph window", picture);
	//}
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::mouseDoubleClick(const MouseEvent& /*event*/) {
	openSeparateGraphWindow();
}
//
MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::SeparateGraphPanel(Image& _image) {
	//
	imageComponent = new ImageComponent(_image);
	imageComponent->setSize(_image.getWidth(), _image.getHeight());
	//
	viewport = new Viewport("ImageViewport");
	viewport->setViewedComponent(imageComponent, false);
	viewport->setScrollBarsShown(true, true);
	viewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(187, 148, 174));
	viewport->getHorizontalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(187, 148, 174));
	viewport->setScrollBarThickness(10);
	viewport->setBounds(0, 0, getWidth(), getHeight());
	viewport->setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
	setSize(getParentWidth(), getParentHeight());
	//
	addAndMakeVisible(viewport);
}
//
MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::~SeparateGraphPanel() {
	delete(imageComponent);
	delete(viewport);
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::paint(juce::Graphics& g) {
	g.fillAll(Colours::white);
}

void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::resized() {
	viewport->setBounds(0, 0, getWidth(), getHeight());
}
//
MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::ImageComponent::ImageComponent(Image& _image) {
	picture = &_image;
	addMouseListener(this, true);
}

MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::ImageComponent::~ImageComponent() {}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::ImageComponent::paint(juce::Graphics& g) {
	g.drawImage(*picture, 0, 0, picture->getWidth(), picture->getHeight(), 0, 0, picture->getWidth(), picture->getHeight(), false);

}

void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::ImageComponent::resized() {
	setSize(picture->getWidth(), picture->getHeight());
}
//
void MainComponent::PerformanceAnalyzer::ProfileGraphPanel::SeparateGraphPanel::ImageComponent::mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel) {
	if (wheel.deltaY < 0) {
	}
	if (wheel.deltaY > 0) {
	}
}
//
MainComponent::PerformanceAnalyzer::MyTabbedComponent::MyTabbedComponent(ProfileTable& table, Viewport& graphViewport)
	: TabbedComponent(TabbedButtonBar::TabsAtTop)
{
	auto colour = Colour(37, 11, 46);
	//
	addTab("Table", colour, &table, true);
	addTab("Graph", colour, &graphViewport, true);
}
//
MainComponent::PerformanceAnalyzer::ProfileTable::ProfileTable(vector<array<std::string, 6>>& _data, map<string, juce::Colour>& _funcColoursMap, MainComponent& _mainComponent) {
	data = &_data;
	mainComponent = &_mainComponent;
	funcColoursMap = _funcColoursMap;
	rowsColoursMap;
	//
	refreshRowsColoursMap();
	//
	box.setModel(this);
	box.setColour(ListBox::backgroundColourId, Colour(94, 60, 82));
	box.setRowHeight(30);
	//
	int width = getWidth();
	if (width <= 0) width = 7;
	int singleColumnWidth = width / 7;
	//
	box.setHeaderHeight(50);
	//
	box.getHeader().addColumn("Name", 1, 2 * singleColumnWidth);
	box.getHeader().addColumn("Time", 2, singleColumnWidth);
	box.getHeader().addColumn("Time\n(%)", 3, singleColumnWidth);
	box.getHeader().addColumn("Called", 4, singleColumnWidth);
	box.getHeader().addColumn("Children\n(%)", 5, singleColumnWidth);
	box.getHeader().addColumn("Self\n(%)", 6, singleColumnWidth);
	box.getHeader().setLookAndFeel(&myLookAndFeel);
	//
	box.getVerticalScrollBar().setVisible(true);
	box.getVerticalScrollBar().setAutoHide(false);
	box.getVerticalScrollBar().setColour(Slider::ColourIds::thumbColourId, Colour(187, 148, 174));
	//
	addAndMakeVisible(box);
	//
}
//
MainComponent::PerformanceAnalyzer::ProfileTable::~ProfileTable() {
	box.getHeader().setLookAndFeel(nullptr);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::paintRowBackground(Graphics& g, int /*rowNumber*/, int width, int height, bool rowIsSelected)
{
	//
	g.setColour(juce::Colours::white);
	if (rowIsSelected) g.setColour(juce::Colours::lightpink);
	g.fillRect(0, 0, width, height);
	//!!!
	box.getHorizontalScrollBar().setVisible(false);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	juce::String fontTypeface = "Courier New";
	juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::bold;
	juce::Font font(fontTypeface, (float)fontSize, fontStyle);
	g.setFont(font);
	g.setColour(juce::Colours::black);
	std::string val = (data->at(rowNumber)).at(columnId - 1);
	if ((rowsColoursMap.size() != 0) && (rowsColoursMap.find(rowNumber) != rowsColoursMap.end())) g.setColour(rowsColoursMap.at(rowNumber));
	g.drawText(val, 2, 0, width - 4, height, Justification::centred, true);
	g.setColour(juce::Colours::white);
	g.fillRect(width - 1, 0, 1, height);
	g.setColour(juce::Colours::white);
	if ((rowsColoursMap.size() != 0) && (rowsColoursMap.find(rowNumber) != rowsColoursMap.end())) g.setColour(rowsColoursMap.at(rowNumber));
	g.drawRect(0, 0, width, height);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::sortOrderChanged(int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
		bool changesFlag = true;
		while (changesFlag) {
			changesFlag = false;
			for (int i = 0; i < data->size() - 1; i++) {
				if (!isForwards)
				{
					if (newSortColumnId == 1) {
						string val1 = data->at(i)[newSortColumnId - 1];
						string val2 = data->at(i + 1)[newSortColumnId - 1];
						//
						if (val1 < val2) {
							array<std::string, 6> tmp = data->at(i);
							data->at(i) = data->at(i + 1);
							data->at(i + 1) = tmp;
							//
							changesFlag = true;
						}
					}
					else
					{
						string val1 = data->at(i)[newSortColumnId - 1];
						string val2 = data->at(i + 1)[newSortColumnId - 1];
						int compVal1 = -1;
						int compVal2 = -1;
						if (val1 != "-") compVal1 = stoi(val1);
						if (val2 != "-") compVal2 = stoi(val2);
						if (compVal1 < compVal2) {
							array<std::string, 6> tmp = data->at(i);
							data->at(i) = data->at(i + 1);
							data->at(i + 1) = tmp;
							//
							changesFlag = true;
						}
					}
				}
				else
				{
					if (newSortColumnId == 1) {
						string val1 = data->at(i)[newSortColumnId - 1];
						string val2 = data->at(i + 1)[newSortColumnId - 1];
						//
						if (val1 > val2) {
							array<std::string, 6> tmp = data->at(i);
							data->at(i) = data->at(i + 1);
							data->at(i + 1) = tmp;
							//
							changesFlag = true;
						}
					}
					else
					{
						string val1 = data->at(i)[newSortColumnId - 1];
						string val2 = data->at(i + 1)[newSortColumnId - 1];
						int compVal1 = -1;
						int compVal2 = -1;
						if (val1 != "-") compVal1 = stoi(val1);
						if (val2 != "-") compVal2 = stoi(val2);
						if (compVal1 > compVal2) {
							array<std::string, 6> tmp = data->at(i);
							data->at(i) = data->at(i + 1);
							data->at(i + 1) = tmp;
							//
							changesFlag = true;
						}
					}
				}
			}
		}
		//
		box.updateContent();
		refreshRowsColoursMap();
		setSelectedRow(selectedFunc);
	}
}
//
int MainComponent::PerformanceAnalyzer::ProfileTable::getNumRows() {
	return (int)data->size();
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::resized() {
	//
	box.setBounds(getLocalBounds());
	int width = getWidth();
	int singleColumnWidth = width / 7;
	//
	for (int i = 0; i < box.getHeader().getNumColumns(true); i++) {
		int columnId = box.getHeader().getColumnIdOfIndex(i, true);
		if (columnId == 1) box.getHeader().setColumnWidth(columnId, 2 * singleColumnWidth);
		else box.getHeader().setColumnWidth(columnId, singleColumnWidth);
	}
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setSelectedRow(int index) {
	SparseSet<int> sparseSet;
	sparseSet.addRange(Range<int>(index, index + 1));
	selectedRow = index;
	box.setSelectedRows(sparseSet);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setSelectedRow(const string& name) {
	for (int i = 0; i < data->size(); i++) {
		array<std::string, 6> row = data->at(i);
		if (row.at(0) == name) {
			selectedFunc = name;
			setSelectedRow(i);
		}
	}
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::clearSelection() {
	SparseSet<int> sparseSet;
	sparseSet.addRange(Range<int>(0, 0));
	box.setSelectedRows(sparseSet);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::refreshRowsColoursMap() {
	rowsColoursMap = *(new map<int, juce::Colour>());
	int rowCtr = 0;
	for (vector<array<std::string, 6>> ::iterator it = data->begin(); it != data->end(); it++) {
		string funcName = it->at(0);
		if ((funcColoursMap.size() != 0) && (funcColoursMap.find(funcName) != funcColoursMap.end())) {
			rowsColoursMap.insert({ rowCtr, funcColoursMap.at(funcName) });
		}
		rowCtr++;
	}
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::setFontSize(const int size) {
	if (size < 0) return;
	//
	fontSize = size;
}
//
MainComponent::PerformanceAnalyzer::ProfileTable::MyLookAndFeel::MyLookAndFeel()
{
	setColour(juce::Slider::thumbColourId, Colour(37, 11, 46));
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::MyLookAndFeel::drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header,
	const String& columnName, int /*columnId*/,
	int width, int height, bool isMouseOver, bool isMouseDown,
	int columnFlags)
{
	g.setColour(Colour(37, 11, 46));
	g.fillRect(0, 0, width, height);
	auto highlightColour = header.findColour(TableHeaderComponent::highlightColourId);

	if (isMouseDown)
		g.fillAll(highlightColour);
	else if (isMouseOver)
		g.fillAll(highlightColour.withMultipliedAlpha(0.625f));

	Rectangle<int> area(width, height);
	area.reduce(4, 0);

	if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
	{
		Path sortArrow;
		sortArrow.addTriangle(0.0f, 0.0f,
			0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
			1.0f, 0.0f);

		g.setColour(juce::Colours::hotpink);
		g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced(2).toFloat(), true));
	}
	//
	g.setColour(juce::Colours::white);
	/*   juce::String fontTypeface = "Courier New";
	   float headerFontSize = (float)(15);
	   juce::Font::FontStyleFlags fontStyle = juce::Font::FontStyleFlags::plain;
	   Font font(fontTypeface, headerFontSize, fontStyle);*/
	g.setFont(16.0f);
	//
	g.drawFittedText(columnName, area, Justification::centred, 1);
	g.setColour(juce::Colours::white);
	g.drawRect(0, 0, width, height);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::cellClicked(int rowNumber, int /*columnId*/, const MouseEvent&) {
	setSelectedRow(rowNumber);
	array<std::string, 6> row = data->at(rowNumber);
	selectedFunc = row.at(0);
}
//
void MainComponent::PerformanceAnalyzer::ProfileTable::selectedRowsChanged(int lastRowSelected) {
	mainComponent->setSelectedFunc(data->at(lastRowSelected).at(0), 3);
}
//
MainComponent::TitlePanel::TitlePanel(const std::string& str) {
	text = str;
}
//
MainComponent::TitlePanel::~TitlePanel() {

}
//
void MainComponent::TitlePanel::paint(Graphics& g)
{
	g.fillAll(Colour(187, 148, 174));   // clear the background
	g.setColour(Colour(37, 11, 46));
	g.drawRect(getLocalBounds(), 2);   // draw an outline around the component
	g.setColour(Colours::white);
	juce::Font font("Candara", 25.0f, juce::Font::FontStyleFlags::plain);
	g.setFont(font);
	//g.setFont (14.0f);
	g.drawText(text, getLocalBounds(), Justification::centred, true);   // draw some placeholder text
}
//
void MainComponent::TitlePanel::resized()
{
}
//
MainComponent::AsSubComponent::ScrollableWindow::ScrollableWindow(vector<TraceParser::TraceLineStruct>& vec, map<string, string>& addrFuncMap, map<string, juce::Colour>& funcColours) {
	scrollBar = new juce::ScrollBar(true);
	scrollBar->setRangeLimits((double)0, (double)vec.size());
	scrollBar->setSingleStepSize(1);
	scrollBar->setCurrentRangeStart(0);
	scrollBar->addListener(this);
	addAndMakeVisible(scrollBar);
	//
	TraceWindow = new TraceComponent(vec, addrFuncMap, funcColours, *scrollBar);
	addAndMakeVisible(TraceWindow);
	//
	resized();
}
//
MainComponent::AsSubComponent::ScrollableWindow::~ScrollableWindow() {
	delete(TraceWindow);
	delete(scrollBar);
}
//
void MainComponent::AsSubComponent::ScrollableWindow::resized() {
	int width = getWidth();
	int height = getHeight();
	//
	juce::Rectangle<int> mainArea(0, 0, width - scrollBarWidth, height);
	juce::Rectangle<int> sliderArea(width - scrollBarWidth, 0, scrollBarWidth, height);
	//
	TraceWindow->setBounds(mainArea);
	if (scrollBar != nullptr) scrollBar->setBounds(sliderArea);
}
//
void MainComponent::AsSubComponent::ScrollableWindow::paint(Graphics& g) {
	g.fillAll(Colour(94, 60, 82));
}
//
void MainComponent::AsSubComponent::ScrollableWindow::scrollToFunc(const string& funcName, int occuranceNum) {
	vector<int> linesVector = TraceWindow->getFuncLines(funcName);
	int index = 0;
	if (linesVector.size() != 0) {
		if (!(((occuranceNum) < 0) || ((occuranceNum) > linesVector.size()))) index = occuranceNum;
		int line = linesVector.at(index);
		//TraceWindow->setTopLine(line);
		TraceWindow->setSelectedLine(line);
	}
}
//
void MainComponent::AsSubComponent::ScrollableWindow::clearSelection() {
	TraceWindow->clearSelections();
}
//
int MainComponent::AsSubComponent::ScrollableWindow::getNumberOfOccurances(const string& funcName) {
	vector<int> linesVector = TraceWindow->getFuncLines(funcName);
	return (int)linesVector.size();
}
//
map<string, juce::Colour> MainComponent::AsSubComponent::ScrollableWindow::getFuncColoursMap() {
	return TraceWindow->getFuncColoursMap();
}
//
void MainComponent::AsSubComponent::ScrollableWindow::setFontSize(const int size) {
	if (size < 0) return;
	//
	TraceWindow->setFontSize(size);
}
//
void MainComponent::AsSubComponent::ScrollableWindow::scrollBarMoved(juce::ScrollBar* /*scrollBarThatHasMoved*/, double newRangeStart) {
	TraceWindow->setSelectedLine((int)newRangeStart);
}
//
MainComponent::AsSubComponent::OccurancesPanel::OccurancesPanel(AsSubComponent& asComp) {
	parent = &asComp;
	numPanel = new OccuranceNumberPanel(parent->getCurrentSelectedOccurance());
	totalPanel = new OccuranceNumberPanel(-1);
	//
	outOfPanel = new TitlePanel("out of");
	occuranceTitlePanel = new TitlePanel("Occurance");
	//
	decrButton = new TextButton("<");
	incrButton = new TextButton(">");
	//
	decrButton->setColour(TextButton::ColourIds::buttonColourId, Colour(37, 11, 46));
	incrButton->setColour(TextButton::ColourIds::buttonColourId, Colour(37, 11, 46));
	//
	decrButton->addListener(this);
	incrButton->addListener(this);
	//
	addAndMakeVisible(numPanel);
	addAndMakeVisible(totalPanel);
	addAndMakeVisible(decrButton);
	addAndMakeVisible(incrButton);
	//
	addAndMakeVisible(occuranceTitlePanel);
	addAndMakeVisible(outOfPanel);
}
//
MainComponent::AsSubComponent::OccurancesPanel::~OccurancesPanel() {
	incrButton->removeListener(this);
	decrButton->removeListener(this);
	delete(numPanel);
	delete(totalPanel);
	delete(incrButton);
	delete(decrButton);
	delete(occuranceTitlePanel);
	delete(outOfPanel);
}
//
//
void MainComponent::AsSubComponent::OccurancesPanel::paint(Graphics& g) {
	g.fillAll(Colour(94, 60, 82));
}
//
void MainComponent::AsSubComponent::OccurancesPanel::resized() {
	Rectangle<int> area = Rectangle<int>(0, 0, getWidth(), getHeight());
	using Track = Grid::TrackInfo;
	using Fr = Grid::Fr;
	Grid grid;
	grid.templateRows = { Track(Fr(1)) };
	grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(4)), Track(Fr(1)), Track(Fr(4)), Track(Fr(1)) };
	grid.items = { GridItem(decrButton), GridItem(incrButton), GridItem(occuranceTitlePanel), GridItem(numPanel), GridItem(outOfPanel), GridItem(totalPanel) };
	grid.performLayout(area);
}
//
MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::TitlePanel(const string& _text) {
	text = _text;
}
//
MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::~TitlePanel() {
}
//
void MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::paint(juce::Graphics& g)
{
	g.fillAll(Colour(94, 60, 82));   // clear the background

	g.setColour(Colour(94, 60, 82));
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
	g.drawText(text, getLocalBounds(),
		juce::Justification::centred, true);   // draw some placeholder text
}
//
void MainComponent::AsSubComponent::OccurancesPanel::TitlePanel::resized()
{}
//
MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::OccuranceNumberPanel(int num) {
	currentOccurNum = num;
}
//
MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::~OccuranceNumberPanel() {

}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::paint(Graphics& g) {
	g.fillAll(juce::Colours::white);
	g.setColour(juce::Colours::black);
	g.drawRect(getLocalBounds(), 1);
	g.setColour(juce::Colours::black);
	juce::Font font("Courier New", 15.0f, juce::Font::FontStyleFlags::plain);
	g.setFont(font);
	string text;
	if (currentOccurNum >= 0) {
		text = to_string(currentOccurNum + 1);
	}
	else {
		text = "-";
	}
	g.drawText(text, getLocalBounds(), juce::Justification::centred, true);

}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::resized() {
}
//
void MainComponent::AsSubComponent::OccurancesPanel::OccuranceNumberPanel::setNumber(int num) {
	currentOccurNum = num;
	repaint();
	resized();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::setPanelNumbers(int num, int max) {
	numPanel->setNumber(num);
	if (max <= 0) numPanel->setNumber(-1);
	totalPanel->setNumber(max - 1);
	repaint();
	resized();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::incrButtonClicked() {
	parent->incrCurrentSelectedOccurance();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::decrButtonClicked() {
	parent->decrCurrentSelectedOccurance();
}
//
void MainComponent::AsSubComponent::OccurancesPanel::buttonClicked(Button* button) {

	if (button == incrButton)
	{
		incrButtonClicked();
	}
	if (button == decrButton) {
		decrButtonClicked();
	}
}
//
MainComponent::AsSubComponent::AsSubComponent(vector<TraceParser::TraceLineStruct>& vec, map<string, string>& addrFuncMap, map<string, juce::Colour>& funcColoursMap, MainComponent& _mainComponent) {
	scrollableWindow = new ScrollableWindow(vec, addrFuncMap, funcColoursMap);
	addAndMakeVisible(scrollableWindow);
	//
	mainComponent = &_mainComponent;
	//
	occurancesPanel = new OccurancesPanel(*this);
	addAndMakeVisible(occurancesPanel);
	//
	functionsComboBox = new ComboBox("FunctionsComboBox");
	functionsComboBox->setText("Choose function...");
	functionsComboBox->setJustificationType(Justification::centred);
	functionsComboBox->setColour(ComboBox::ColourIds::backgroundColourId, Colour(37, 11, 46));
	functionsComboBox->setEditableText(false);
	//
	//
	searchField = new TextEditor("searchField");
	juce::Font font("Courier New", 20.0f, juce::Font::FontStyleFlags::plain);
	searchField->setFont(font);
	searchField->setMultiLine(false);
	searchField->addListener(this);
	//
	searchField->setColour(TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
	searchField->setColour(TextEditor::ColourIds::textColourId, juce::Colours::black);
	searchField->setColour(TextEditor::ColourIds::highlightColourId, juce::Colours::hotpink);
	searchField->setColour(TextEditor::ColourIds::highlightedTextColourId, juce::Colours::lightpink);
	searchField->setColour(TextEditor::ColourIds::outlineColourId, juce::Colours::black);
	searchField->setColour(TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::hotpink);
	searchField->setColour(TextEditor::ColourIds::shadowColourId, juce::Colours::grey);
	//
	searchField->setText(defaultSearchfieldText);

	addAndMakeVisible(searchField);
	//
	funcOccurances = new map<string, int>();
	set<string> funcNames;
	for (int i = 0; i < (&vec)->size(); i++) {
		funcNames.insert((&vec)->at(i).func);
	}
	//
	comboBoxItemID = new map<string, int>();
	int ctr = 1;
	for (set<string>::iterator it = funcNames.begin(); it != funcNames.end(); it++) {
		string funcName = *it;
		int occurancesNum = scrollableWindow->getNumberOfOccurances(funcName);

		if (funcName != "") {
			funcOccurances->insert({ funcName, occurancesNum });
			//
			pair<string, int> tmp = make_pair(funcName, ctr++);
			comboBoxItemID->insert(tmp);
			functionsComboBox->addItem(tmp.first, tmp.second);
		}

	}
	//
	lf;
	lf.setColour(PopupMenu::ColourIds::backgroundColourId, Colour(37, 11, 46));
	functionsComboBox->getRootMenu()->setLookAndFeel(&lf);
	//
	functionsComboBox->onChange = [this] {
		selectedFunction = functionsComboBox->getText().toStdString();
		mainComponent->setSelectedFunc(selectedFunction, 1);
		searchField->setText(defaultSearchfieldText);
		//
		selectedFuncOccurance = 0;
		scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
		searchField->setText(defaultSearchfieldText, false);
		occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
		};
	//functionsComboBox->getItem
	//functionsComboBox->setSelectedId(1);
	addAndMakeVisible(functionsComboBox);
}
//
MainComponent::AsSubComponent::~AsSubComponent() {
	delete(scrollableWindow);
	delete(functionsComboBox);
	delete(occurancesPanel);
	delete(searchField);
}
//
void MainComponent::AsSubComponent::paint(Graphics& g) {
	g.fillAll(Colours::white);
}
//
void MainComponent::AsSubComponent::resized() {
	using Track = Grid::TrackInfo;
	using Fr = Grid::Fr;
	Grid grid;
	grid.templateRows = { Track(Fr(3)),Track(Fr(3)),Track(Fr(3)), Track(Fr(80)) };
	grid.templateColumns = { Track(Fr(1)) };
	grid.items = { GridItem(functionsComboBox),GridItem(searchField), GridItem(occurancesPanel), GridItem(scrollableWindow) };
	grid.performLayout(getLocalBounds());
}
//
void MainComponent::AsSubComponent::selectFuncInCombobox(const string& funcName) {
	if (comboBoxItemID->find(funcName) == comboBoxItemID->end()) return;
	//
	functionsComboBox->setSelectedId(comboBoxItemID->at(funcName));
}
//
int MainComponent::AsSubComponent::getFuncOccuranceNumber(const string& funcName) {
	if (funcOccurances->find(funcName) == funcOccurances->end()) return -1;
	return funcOccurances->at(funcName);
}
//
void MainComponent::AsSubComponent::setSelectedFunc(const string& funcName) {
	if (funcOccurances->find(funcName) == funcOccurances->end()) {
		selectedFunction = "";
		scrollableWindow->clearSelection();
		functionsComboBox->setSelectedId(0);
		functionsComboBox->setText("<No entries>");
		occurancesPanel->setPanelNumbers(0, 0);
		return;
	}
	else {
		selectedFunction = funcName;
		selectedFuncOccurance = 0;
		scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
		functionsComboBox->setSelectedId(comboBoxItemID->at(selectedFunction), juce::NotificationType::dontSendNotification);
		searchField->setText(defaultSearchfieldText, false);
		occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
		return;
	}
}
//
string MainComponent::AsSubComponent::getCurrentSelectedFunction() {
	return selectedFunction;
}
//
int MainComponent::AsSubComponent::getCurrentSelectedOccurance() {
	return selectedFuncOccurance;
}
//
void MainComponent::AsSubComponent::incrCurrentSelectedOccurance() {
	selectedFuncOccurance = (selectedFuncOccurance + 1) % getFuncOccuranceNumber(getCurrentSelectedFunction());
	scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
	occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
}
//
void MainComponent::AsSubComponent::decrCurrentSelectedOccurance() {
	selectedFuncOccurance = (selectedFuncOccurance - 1 + getFuncOccuranceNumber(getCurrentSelectedFunction())) % getFuncOccuranceNumber(getCurrentSelectedFunction());
	scrollableWindow->scrollToFunc(selectedFunction, selectedFuncOccurance);
	occurancesPanel->setPanelNumbers(selectedFuncOccurance, scrollableWindow->getNumberOfOccurances(selectedFunction));
}
//
map<string, juce::Colour> MainComponent::AsSubComponent::getFuncColoursMap() {
	return scrollableWindow->getFuncColoursMap();
}
//
void MainComponent::AsSubComponent::setFontSize(const int size) {
	if (size < 0) return;
	//
	scrollableWindow->setFontSize(size);
}
//
void MainComponent::AsSubComponent::textEditorTextChanged(TextEditor& editor) {
	mainComponent->setSelectedFunc(editor.getText().toStdString(), 1);
}
//
MainComponent::CodeSubComponent::CodeSubComponent(const std::vector<std::string>& filepaths, map<string, string> map, MainComponent& _mainComponent) {
	mainComponent = &_mainComponent;
	//
	std::vector<std::string> filesContentVector;
	std::vector<std::string> filesNamesVector;
	std::vector<std::string> filepathsVector = filepaths;
	//
	for (std::vector<std::string>::iterator it = filepathsVector.begin(); it != filepathsVector.end(); it++) {
		string content;
		std::getline(std::ifstream(*it), content, '\0');
		filesContentVector.push_back(content);
		filesNamesVector.push_back(it->substr(it->find_last_of("/\\") + 1));
	}
	//
	codeWindows = new vector<CodeComponent*>();
	//
	for (int i = 0; i < min(filesContentVector.size(), filesNamesVector.size()); i++) {
		codeWindows->push_back(new CodeComponent(filesNamesVector.at(i), filesContentVector.at(i), map));
	}
	//
	tabs = new MyTabbedComponent(*codeWindows);
	tabs->setBounds(0, 0, getWidth(), getHeight());
	addAndMakeVisible(tabs);
}
//
MainComponent::CodeSubComponent::~CodeSubComponent() {
	//delete(CodeWindow);
	delete(codeWindows);
	delete(tabs);
}
//
void MainComponent::CodeSubComponent::paint(Graphics& g) {
	g.fillAll(Colour(94, 60, 82));
	g.setColour(Colour(37, 11, 46));
	g.drawRect(getLocalBounds(), borderThickness);
}
void MainComponent::CodeSubComponent::resized() {
	//
	for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
		(*it)->setSize(getWidth() - 2 * borderThickness, getHeight() - 2 * borderThickness);
	}
	//
	tabs->setBounds(borderThickness, borderThickness, getWidth() - 2 * borderThickness, getHeight() - 2 * borderThickness);
}
//
void MainComponent::CodeSubComponent::selectFunc(const string& funcName) {
	int tabIndex = 0;
	//
	for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
		if ((*it)->selectFunc(funcName)) tabs->setCurrentTabIndex(tabIndex);
		tabIndex++;
	}
}
//
void MainComponent::CodeSubComponent::setFontSize(const int size) {
	if (size < 0) return;
	//
	for (vector<CodeComponent*>::iterator it = codeWindows->begin(); it != codeWindows->end(); it++) {
		(*it)->setFontSize(size);
	}
}
//
MainComponent::CodeSubComponent::MyTabbedComponent::MyTabbedComponent(vector<CodeComponent*>& codeComponents)
	: TabbedComponent(TabbedButtonBar::TabsAtTop)
{
	//auto colour = findColour(ResizableWindow::backgroundColourId);
	auto colour = Colour(37, 11, 46);
	//
	for (vector<CodeComponent*>::iterator it = codeComponents.begin(); it != codeComponents.end(); it++) {
		addTab((*it)->getFilename(), colour, *it, true);
	}
}
//
MainComponent::AnalyzerSubComponent::AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>& _linesInfoVector, map<string, vector<string>>& _funcAddrMap, map<string, set<string>>& _callingMap, map<string, vector<string>>& _callersMap, map<string, pair<string, string>>& _addrCallingCalledMap, map<string, juce::Colour>& _funcColoursMap, MainComponent& _mainComponent) {
	mainComponent = &_mainComponent;
	performanceAnalyzer = new PerformanceAnalyzer(_linesInfoVector, _funcAddrMap, _callingMap, _callersMap, _addrCallingCalledMap, _funcColoursMap, *mainComponent);
	addAndMakeVisible(performanceAnalyzer);
}
//
MainComponent::AnalyzerSubComponent::~AnalyzerSubComponent() {
	delete(performanceAnalyzer);
}
//
void MainComponent::AnalyzerSubComponent::paint(Graphics& g) {
	g.fillAll(juce::Colours::white);
	g.setColour(Colour(37, 11, 46));
	g.drawRect(getLocalBounds(), borderThickness);
	g.setColour(juce::Colours::white);
	g.setFont(22.0f);
	g.drawText("AnalyzerSubComponent", getLocalBounds(), juce::Justification::centred, true);
}
//
void MainComponent::AnalyzerSubComponent::resized() {
	performanceAnalyzer->setBounds(borderThickness, borderThickness, getWidth() - 2 * borderThickness, getHeight() - 2 * borderThickness);
}
//
void MainComponent::AnalyzerSubComponent::setSelectedFunc(const string& funcName) {
	performanceAnalyzer->tableSetSelectedRow(funcName);
}
//
void MainComponent::AnalyzerSubComponent::setFontSize(const int size) {
	if (size < 0) return;
	//
	performanceAnalyzer->setFontSize(size);
}
//
MainComponent::PlaceholderSubComponent::PlaceholderSubComponent() {}
//
MainComponent::PlaceholderSubComponent::~PlaceholderSubComponent() {}
//
void MainComponent::PlaceholderSubComponent::paint(Graphics& g) {
	g.fillAll(juce::Colour(187, 148, 174));
	File parentDir = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
	String pictureFileath = parentDir.getFullPathName() + "/Resources/Icons/placeholder.png";
	File pictureFile(pictureFileath);
	Image picture = ImageCache::getFromFile(pictureFile);
	g.setOpacity(1.0f);
	Rectangle<float> pictureArea((float)0, (float)(getHeight() / 4), (float)getWidth(), (float)(getHeight() / 2 - getHeight() / 4));
	Rectangle<float> textArea((float)0, (float)(getHeight() / 2), (float)getWidth(), (float)(getHeight() / 2));
	g.drawImage(picture, pictureArea, RectanglePlacement::doNotResize, false);
	//
	g.setColour(Colour(94, 60, 82));
	juce::Font font("Courier New", 55.0f, juce::Font::FontStyleFlags::bold);
	g.setFont(font);
	g.drawText("Create new project or open existing one!", textArea, juce::Justification::horizontallyCentred);
}
//
void MainComponent::PlaceholderSubComponent::resized() {}
//
MainComponent::MainComponent()
{
	currentSettings;
	loadSettings();
	//
	saveSettingsButton = new TextButton("Save");
	saveSettingsButton->addListener(this);
	//
	menuBar = new MenuBarComponent(this);
	addAndMakeVisible(menuBar);
	//
	//
	placeholderPanel = new PlaceholderSubComponent();
	addAndMakeVisible(placeholderPanel);
	//
	setSize(1440, 800);
}

MainComponent::~MainComponent()
{
	if (menuBar != nullptr) delete(menuBar);
	if (placeholderPanel != nullptr) delete(placeholderPanel);
	if (asPanelTitle != nullptr) delete(asPanelTitle);
	if (codePanelTitle != nullptr) delete(codePanelTitle);
	if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
	if (asPanel != nullptr) delete(asPanel);
	if (codePanel != nullptr) delete(codePanel);
	if (analyzerPanel != nullptr) delete(analyzerPanel);
	//
	if (createProjWindow != nullptr) delete(createProjWindow);
	if (settingsWindow != nullptr) delete(settingsWindow);
	if (aboutWindow != nullptr) delete(aboutWindow);
	//
	if (saveSettingsButton != nullptr) delete(saveSettingsButton);
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
	g.fillAll(Colour(37, 11, 46));

}
//
void MainComponent::resized()
{
	placeholderPanel->setSize(getWidth(), getHeight());
	//
	Rectangle<int> menuArea = Rectangle<int>(0, 0, getWidth(), menuHeight);
	Rectangle<int> workspaceArea = Rectangle<int>(0, menuHeight, getWidth(), getHeight() - menuHeight);
	//
	menuBar->setBounds(menuArea);
	//
	using Track = Grid::TrackInfo;
	using Fr = Grid::Fr;
	//
	if (projectOpened) {
		Grid grid;
		grid.templateRows = { Track(Fr(3)), Track(Fr(40)) };
		//
		int traceProportion = 3;
		int codeProportion = 4;
		int analyzerProportion = 3;
		//
		grid.templateColumns = { Track(Fr(traceProportion)), Track(Fr(codeProportion)), Track(Fr(analyzerProportion)) };
		grid.items = { GridItem(asPanelTitle), GridItem(codePanelTitle), GridItem(analyzerPanelTitle), GridItem(asPanel), GridItem(codePanel), GridItem(analyzerPanel) };
		grid.performLayout(workspaceArea);
	}
	else {
		placeholderPanel->setBounds(workspaceArea);
	}
}
//
void MainComponent::setSelectedFunc(const string& funcName, int callerID) {
	if (callerID != 1) asPanel->setSelectedFunc(funcName);
	if (callerID != 2) codePanel->selectFunc(funcName);
	if (callerID != 3) analyzerPanel->setSelectedFunc(funcName);
}
//
void MainComponent::loadSettings() {
	juce::File workingDirectory = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
	currentSettings = TSettingsParser::getSettingsFromFile(workingDirectory.getFullPathName().toStdString() + "/config.JSON");
}
//
void MainComponent::createProjectFile() {
	createProjWindow = new CreateProjectWindow("New project");
	createProjWindow->setVisible(false);
	createProjWindow->addComponentListener(this);
	createProjWindow->setVisible(true);
}
//
void MainComponent::openProjectFile(const string filepath) {
	//
	AlertWindow* processWnd = new AlertWindow("Loading",
		"Please, wait for project to load...",
		MessageBoxIconType::NoIcon);
	processWnd->enterModalState(true, nullptr, true);
	//
	/*Flag = */MessageManager::callAsync([this, processWnd, filepath]()
		{
			TProjectParser::Project newProject = TProjectParser::getProjectFromFile(filepath);
			if ((newProject.trace != "") && (newProject.objdump != "") && (newProject.code.size() != 0)) {
				project = newProject;
				//
				if (asPanelTitle != nullptr) delete(asPanelTitle);
				if (codePanelTitle != nullptr) delete(codePanelTitle);
				if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
				if (asPanel != nullptr) delete(asPanel);
				if (codePanel != nullptr) delete(codePanel);
				if (analyzerPanel != nullptr) delete(analyzerPanel);
				//
				asPanelTitle = new TitlePanel("Assembly trace");
				codePanelTitle = new TitlePanel("Code");
				analyzerPanelTitle = new TitlePanel("Analyzer");
				//
				addAndMakeVisible(asPanelTitle);
				addAndMakeVisible(codePanelTitle);
				addAndMakeVisible(analyzerPanelTitle);
				//
				//Objdump parsing
				TObjdumpParser* parser = new TObjdumpParser();
				parser->parseFile(project.objdump);
				map<string, string> firstFuncAddrMap = parser->getFirstFuncAddrMap();
				map<string, string> addrFuncMap = parser->getAddrFuncMap();
				map<string, vector<string>> funcAddrMap = parser->getFuncAddrMap();
				//
				map<string, string> lastFuncAddrMap;
				for (map<string, vector<string>>::iterator it = funcAddrMap.begin(); it != funcAddrMap.end(); it++) {
					lastFuncAddrMap.insert({ (it->second).at((it->second).size() - 1), it->first });
				}
				//
				map<string, set<string>> callingMap = parser->getCallingMap();
				map<string, vector<string>> callersMap = parser->getCallersMap();
				map<string, pair<string, string>> addrCallerCalled = parser->getAddrCallerCalled();
				map<string, vector<string>> retFuncAddrs = parser->getRetFuncAddrMap();
				//
				//Trace parsing
				vector<TraceParser::TraceLineStruct>* vec = new vector<TraceParser::TraceLineStruct>();
				TraceParser traceParser(*vec);
				traceParser.parseTrace(project.trace);
				traceParser.addFuncAddresses(addrFuncMap);
				traceParser.markFirstLines(firstFuncAddrMap);
				traceParser.markCallLines(firstFuncAddrMap, addrCallerCalled, callingMap);
				traceParser.markLastLines(retFuncAddrs);
				//
				//
				map<string, juce::Colour> funcColoursMap;
				codePanel = new CodeSubComponent(project.code, firstFuncAddrMap, *this);
				analyzerPanel = new AnalyzerSubComponent(*vec, funcAddrMap, callingMap, callersMap, addrCallerCalled, funcColoursMap, *this);
				asPanel = new AsSubComponent(*vec, addrFuncMap, funcColoursMap, *this);
				setFontSizes();
				//
				//
				addAndMakeVisible(asPanel);
				addAndMakeVisible(codePanel);
				addAndMakeVisible(analyzerPanel);
				projectOpened = true;
				resized();
			}
			else {
				showAlertWindow();
			}
			//
			if (nullptr != processWnd)
			{
				processWnd->exitModalState();
			}
		});
}
//
void MainComponent::chooseProjectFile() {
	chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.JSON");
	auto chooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
	chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
		{
			auto file = fc.getResult();
			if (file != File{}) openProjectFile((file.getFullPathName().toStdString()));
		});
}
//
void MainComponent::saveProject() {
	chooser = std::make_unique<FileChooser>(String("Chooser"), File(defaultFilepath), "*.JSON");
	auto chooserFlags = FileBrowserComponent::saveMode;
	chooser->launchAsync(chooserFlags, [this](const FileChooser& fc)
		{
			auto file = fc.getResult();
			if (file != File{})
			{
				TProjectParser::saveProjectToFile(project, file.getFullPathName().toStdString());
			}
		});
}
//
void MainComponent::closeProjectFile() {
	if (asPanelTitle != nullptr) delete(asPanelTitle);
	if (codePanelTitle != nullptr) delete(codePanelTitle);
	if (analyzerPanelTitle != nullptr) delete(analyzerPanelTitle);
	if (asPanel != nullptr) delete(asPanel);
	if (codePanel != nullptr) delete(codePanel);
	if (analyzerPanel != nullptr) delete(analyzerPanel);
	//
	asPanelTitle = nullptr;
	codePanelTitle = nullptr;
	analyzerPanelTitle = nullptr;
	asPanel = nullptr;
	codePanel = nullptr;
	analyzerPanel = nullptr;
	//
	projectOpened = false;
	//
	resized();
	repaint();
}
//
void MainComponent::openSettingsWindow() {
	if (settingsWindow != nullptr) delete(settingsWindow);
	settingsWindow = new SettingsWindow("Settings", *saveSettingsButton);
	settingsWindow->setVisible(false);
	settingsWindow->addComponentListener(this);
	settingsWindow->setVisible(true);

}
//
void MainComponent::openAboutWindow() {
	if (aboutWindow != nullptr) delete(aboutWindow);
	aboutWindow = new AboutWindow("About");
	aboutWindow->setVisible(false);
	aboutWindow->addComponentListener(this);
	aboutWindow->setVisible(true);
}
//
void MainComponent::updateCurrentSettings() {
	if (projectOpened) {
		setFontSizes();
	}
}
//
void MainComponent::setFontSizes() {
	analyzerPanel->setFontSize(currentSettings.analyzerFontSize);
	codePanel->setFontSize(currentSettings.codeFontSize);
	asPanel->setFontSize(currentSettings.traceFontSize);
	repaint();
	resized();
}
//
StringArray MainComponent::getMenuBarNames() {
	return { "File", "Settings", "Help" };
}
//
PopupMenu MainComponent::getMenuForIndex(int index, const String&) {
	PopupMenu menu;
	lf;
	lf.setColour(PopupMenu::ColourIds::backgroundColourId, Colour(37, 11, 46));
	menu.setLookAndFeel(&lf);
	if (isEnabled()) {
		//File
		if (index == 0) {
			//
			std::function<void()> createProjFunc = [this]() { createProjectFile(); };
			std::function<void()> chooseProjFunc = [this]() { chooseProjectFile(); };
			std::function<void()> saveProjFunc = [this]() { saveProject(); };
			std::function<void()> closeProjFunc = [this]() { closeProjectFile(); };
			//
			menu.addItem(String("New"), createProjFunc);
			menu.addItem(String("Open..."), chooseProjFunc);
			//
			if (projectOpened) {
				menu.addItem(String("Save..."), saveProjFunc);
				menu.addItem(String("Close"), closeProjFunc);
			}
		}
		//
		//Settings
		else if (index == 1) {
			std::function<void()> openGeneralSettings = [this]() { openSettingsWindow(); };
			//
			menu.addItem(String("General"), openGeneralSettings);
		}
		//
		//Help
		else if (index == 2) {
			std::function<void()> openAboutProgram = [this]() { openAboutWindow(); };
			//
			menu.addItem(String("About"), openAboutProgram);
		}
	}
	return menu;
}
//
void MainComponent::menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) {}
//
void  MainComponent::showWaitWindow()
{
	MessageBoxIconType icon = MessageBoxIconType::InfoIcon;
	auto options = MessageBoxOptions::makeOptionsOk(icon,
		"Opening project...",
		"Please, wait for project to load.");
	messageBox = AlertWindow::showScopedAsync(options, nullptr);
}
//
void  MainComponent::showAlertWindow()
{
	MessageBoxIconType icon = MessageBoxIconType::WarningIcon;
	auto options = MessageBoxOptions::makeOptionsOk(icon,
		"Failed to open project file...",
		"File is wrong format or empty.");
	messageBox = AlertWindow::showScopedAsync(options, nullptr);
}
//
void MainComponent::buttonClicked(Button* button) {
	if (button == saveSettingsButton) {
		loadSettings();
		updateCurrentSettings();
	}
}
//
void MainComponent::componentVisibilityChanged(Component& component) {
	if (component.isVisible()) {
		//visible
		component.setAlwaysOnTop(true);
		this->setEnabled(false);
		//
		repaint();
		resized();
	}
	else {
		//not visible
		this->setEnabled(true);
		//
		repaint();
		resized();
	}
}
