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
#include "ProjectInfoWindow.h"
#include <array>
//
//==============================================================================
using namespace juce;
//
class MainComponent : public Component, public MenuBarModel, public ComponentListener, public Button::Listener
{
public:
	//==============================================================================
	//
	class MyStretchableLayoutResizerBar : public Component
	{
	public:
		//
		class TopResizerBar : public Component
		{
		public:
			TopResizerBar(MyStretchableLayoutResizerBar&);
			~TopResizerBar() override;
			void paint(Graphics&) override;
			void resized() override;
		private:
			void mouseEnter(const MouseEvent& event) override;
			void mouseExit(const MouseEvent& event) override;
			void mouseDown(const MouseEvent&) override;
			void mouseUp(const MouseEvent&) override;
			void mouseDrag(const MouseEvent&) override;
			//
			int mouseDownPos;
			MyStretchableLayoutResizerBar* resizerBar = nullptr;
			//
			Colour currentColour;
			Colour defaultColour;
			Colour mouseOverColour;
		};
		//
		MyStretchableLayoutResizerBar(StretchableLayoutManager* layoutToUse, int itemIndexInLayout, bool isBarVertical, MainComponent& _mainComponent);
		~MyStretchableLayoutResizerBar() override;
		virtual void hasBeenMoved();
		struct JUCE_API  LookAndFeelMethods
		{
			virtual ~LookAndFeelMethods() = default;

			virtual void drawStretchableLayoutResizerBar(Graphics&, int w, int h,
				bool isVerticalBar, bool isMouseOver, bool isMouseDragging) = 0;
		};
		void paint(Graphics&) override;
		void setPosition(int);
	private:
		StretchableLayoutManager* layout;
		MainComponent* mainComponent;
		int itemIndex, mouseDownPos;
		bool isVertical;
		int newPos = 0;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyStretchableLayoutResizerBar)
	};
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
		class MyViewport : public Component,
			private ComponentListener,
			private ScrollBar::Listener
		{
		public:
			//
			struct MyViewportHelpers
			{
				MyViewportHelpers() = delete;

				static bool wouldScrollOnEvent(const MyViewport* vp, const MouseInputSource& src)
				{
					if (vp != nullptr)
					{
						switch (vp->getScrollOnDragMode())
						{
						case MyViewport::ScrollOnDragMode::all:           return true;
						case MyViewport::ScrollOnDragMode::nonHover:      return !src.canHover();
						case MyViewport::ScrollOnDragMode::never:         return false;
						}
					}

					return false;
				}
			};
			//
			MyViewport(const String& componentName = String());
			~MyViewport() override;
			void setViewedComponent(Component* newViewedComponent,
				bool deleteComponentWhenNoLongerNeeded = true);
			//
			Component* getViewedComponent() const noexcept { return contentComp.get(); }
			//
			void setViewPosition(int xPixelsOffset, int yPixelsOffset);
			void setViewPosition(Point<int> newPosition);
			void setViewPositionProportionately(double proportionX, double proportionY);
			//
			bool autoScroll(int mouseX, int mouseY, int distanceFromEdge, int maximumSpeed);
			Point<int> getViewPosition() const noexcept { return lastVisibleArea.getPosition(); }
			Rectangle<int> getViewArea() const noexcept { return lastVisibleArea; }
			int getViewPositionX() const noexcept { return lastVisibleArea.getX(); }
			int getViewPositionY() const noexcept { return lastVisibleArea.getY(); }
			int getViewWidth() const noexcept { return lastVisibleArea.getWidth(); }
			int getViewHeight() const noexcept { return lastVisibleArea.getHeight(); }
			int getMaximumVisibleWidth() const;
			int getMaximumVisibleHeight() const;
			virtual void visibleAreaChanged(const Rectangle<int>& newVisibleArea);
			virtual void viewedComponentChanged(Component* newComponent);
			void setScrollBarsShown(bool showVerticalScrollbarIfNeeded,
				bool showHorizontalScrollbarIfNeeded,
				bool allowVerticalScrollingWithoutScrollbar = false,
				bool allowHorizontalScrollingWithoutScrollbar = false);
			void setScrollBarPosition(bool verticalScrollbarOnRight,
				bool horizontalScrollbarAtBottom);
			bool isVerticalScrollbarOnTheRight() const noexcept { return vScrollbarRight; }
			bool isHorizontalScrollbarAtBottom() const noexcept { return hScrollbarBottom; }
			bool isVerticalScrollBarShown() const noexcept { return showVScrollbar; }
			bool isHorizontalScrollBarShown() const noexcept { return showHScrollbar; }
			void setScrollBarThickness(int thickness);
			int getScrollBarThickness() const;
			void setSingleStepSizes(int stepX, int stepY);
			ScrollBar& getVerticalScrollBar() noexcept { return *verticalScrollBar; }
			ScrollBar& getHorizontalScrollBar() noexcept { return *horizontalScrollBar; }
			void recreateScrollbars();
			bool canScrollVertically() const noexcept;
			bool canScrollHorizontally() const noexcept;
			[[deprecated("Use setScrollOnDragMode instead.")]]
			void setScrollOnDragEnabled(bool shouldScrollOnDrag)
			{
				setScrollOnDragMode(shouldScrollOnDrag ? ScrollOnDragMode::all : ScrollOnDragMode::never);
			}

			[[deprecated("Use getScrollOnDragMode instead.")]]
			bool isScrollOnDragEnabled() const noexcept { return getScrollOnDragMode() == ScrollOnDragMode::all; }
			//
			enum class ScrollOnDragMode
			{
				never,          /**< Dragging will never scroll the viewport. */
				nonHover,       /**< Dragging will only scroll the viewport if the input source cannot hover. */
				all             /**< Dragging will always scroll the viewport. */
			};
			void setScrollOnDragMode(ScrollOnDragMode scrollOnDragMode);
			ScrollOnDragMode getScrollOnDragMode() const { return scrollOnDragMode; }
			bool isCurrentlyScrollingOnDrag() const noexcept;
			//==============================================================================
			/** @internal */
			void resized() override;
			/** @internal */
			void scrollBarMoved(ScrollBar*, double newRangeStart) override;
			/** @internal */
			void mouseWheelMove(const MouseEvent&, const MouseWheelDetails&) override;
			/** @internal */
			void mouseDown(const MouseEvent& e) override;
			//
			//void mouseDrag(const MouseEvent&) override;
			/** @internal */
			bool keyPressed(const KeyPress&) override;
			/** @internal */
			void componentMovedOrResized(Component&, bool wasMoved, bool wasResized) override;
			/** @internal */
			void lookAndFeelChanged() override;
			/** @internal */
			bool useMouseWheelMoveIfNeeded(const MouseEvent&, const MouseWheelDetails&);
			/** @internal */
			static bool respondsToKey(const KeyPress&);
			//
		protected:
			virtual ScrollBar* createScrollBarComponent(bool isVertical);
		private:
			//==============================================================================
			class AccessibilityIgnoredComponent : public Component
			{
			public:
				std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override
				{
					return createIgnoredAccessibilityHandler(*this);
				}
			};
			//
			std::unique_ptr<ScrollBar> verticalScrollBar, horizontalScrollBar;
			AccessibilityIgnoredComponent contentHolder;
			WeakReference<Component> contentComp;
			Rectangle<int> lastVisibleArea;
			int scrollBarThickness = 0;
			int singleStepX = 16, singleStepY = 16;
			ScrollOnDragMode scrollOnDragMode = ScrollOnDragMode::nonHover;
			bool showHScrollbar = true, showVScrollbar = true, deleteContent = true;
			bool customScrollBarThickness = false;
			bool allowScrollingWithoutScrollbarV = false, allowScrollingWithoutScrollbarH = false;
			bool vScrollbarRight = true, hScrollbarBottom = true;
			//
			struct MyDragToScrollListener;
			std::unique_ptr<MyDragToScrollListener> dragToScrollListener;
			//
			Point<int> viewportPosToCompPos(Point<int>) const;
			//
			void updateVisibleArea();
			void deleteOrRemoveContentComp();
			//
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyViewport)
		};
		//
		class ProfileTable : public Component, public TableListBoxModel {
		public:
			//
			ProfileTable(vector<array<std::string, 6>>& _data, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent&);
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
				MyLookAndFeel(MainComponent&);
				//
				void drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header, const String& columnName, int /*columnId*/, int width, int height, bool isMouseOver, bool isMouseDown, int columnFlags) override;
			private:
				MainComponent* mainComponent;
			};
			//
			void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
			void selectedRowsChanged(int lastRowSelected) override;
			//
			MainComponent* mainComponent = nullptr;
			TableListBox box;
			MyLookAndFeel* myLookAndFeel = nullptr;
			vector<array<std::string, 6>>* data;
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
		class ProfileGraphPanel : public Component {
		public:
			ProfileGraphPanel(const string&);
			~ProfileGraphPanel() override;
			void paint(juce::Graphics&) override;
			void resized() override;
			void openSeparateGraphWindow();
		private:
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
				ImageComponent* imageComponent;
				MyViewport* viewport;
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
			MyTabbedComponent(ProfileTable&, MyViewport&);
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
		MyViewport* graphMyViewport = nullptr;
		//
		File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
		string graphTXTFilepath = parentDirecory.getFullPathName().toStdString() + "/Resources/graph.txt";
		string graphPNGFilepath = parentDirecory.getFullPathName().toStdString() + "/Resources/graph.png";
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformanceAnalyzer)
	};
	//
	class TitlePanel : public Component
	{
	public:
		TitlePanel(juce::String&);
		~TitlePanel() override;
		void paint(Graphics&) override;
		void resized() override;
	private:
		juce::String* text;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
	};
	//
	class CodeSubComponent : public Component
	{
	public:
		CodeSubComponent(const std::vector<juce::File>&, map<string, string>, MainComponent&);
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
		MyTabbedComponent* tabs;
		//CodeComponent* CodeWindow;
		vector<CodeComponent*>* codeWindows;
		//
		int borderThickness = 2;
		//
		TitlePanel* titlePanel;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeSubComponent)
	};
	//
	class AnalyzerSubComponent : public Component
	{
	public:
		//
		AnalyzerSubComponent(vector<TraceParser::TraceLineStruct>&, map<string, vector<string>>&, map<string, set<string>>&, map<string, vector<string>>&, map<string, pair<string, string>>&, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent&);
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
	class AsSubComponent : public Component, public TextEditor::Listener
	{
	public:
		class ScrollableWindow : public Component, public ScrollBar::Listener
		{
		public:
			ScrollableWindow(vector<TraceParser::TraceLineStruct>& vec, map<string, string>& addrFuncMap, map<string, juce::Colour>&, map<string, juce::Colour>&);
			~ScrollableWindow();
			void paint(Graphics&) override;
			void resized() override;
			void scrollToFunc(const string&, int);
			void clearSelection();
			int getNumberOfOccurances(const string&);
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
		class OccurancesPanel : public Component, public Button::Listener
		{
		public:
			OccurancesPanel(AsSubComponent&);
			~OccurancesPanel();
			void paint(Graphics&) override;
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
				String* text;
				//
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
			};
			//
			class OccuranceNumberPanel : public Component
			{
			public:
				OccuranceNumberPanel(int);
				~OccuranceNumberPanel();
				void paint(Graphics&) override;
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
			AsSubComponent* parent;
			OccuranceNumberPanel* numPanel;
			OccuranceNumberPanel* totalPanel;
			TitlePanel* outOfPanel;
			TitlePanel* occuranceTitlePanel;
			TextButton* incrButton;
			TextButton* decrButton;
			//
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OccurancesPanel)
		};
		AsSubComponent(vector<TraceParser::TraceLineStruct>& vec, map<string, string>& addrFuncMap, map<string, juce::Colour>&, map<string, juce::Colour>&, MainComponent&);
		~AsSubComponent() override;
		void paint(Graphics&) override;
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
			MyTextEditor(MainComponent&);
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
		map<string, int>* funcOccurances;
		map<string, int>* comboBoxItemID;
		//
		LookAndFeel_V4 lf;
		//
		TitlePanel* titlePanel;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AsSubComponent)
	};
	//
	class PlaceholderSubComponent : public Component
	{
	public:
		PlaceholderSubComponent(String&);
		~PlaceholderSubComponent() override;
		void paint(Graphics&) override;
		void resized() override;
	private:
		String* sectionName;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaceholderSubComponent)
	};
	//
	class MainPlaceholderSubComponent : public Component
	{
	public:
		MainPlaceholderSubComponent(const String&);
		~MainPlaceholderSubComponent() override;
		void paint(Graphics&) override;
		void resized() override;
	private:
		String placeholderPictureFilepath;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPlaceholderSubComponent)
	};
	//
	class DarkeningComponent : public Component
	{
	public:
		DarkeningComponent();
		~DarkeningComponent() override;
		void paint(Graphics&) override;
		void resized() override;
		//
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DarkeningComponent)
	};
	//
	class MyAlertWindow : public AlertWindow, public LookAndFeel_V4
	{
	public:
		MyAlertWindow(const String& title,
			const String& message,
			MessageBoxIconType iconType,
			Component* comp, float _fontSize);
		~MyAlertWindow() override;
		Font getAlertWindowTitleFont() override;
		Font getAlertWindowMessageFont() override;
	private:
		float fontSize;
	};
	//
	MainComponent();
	~MainComponent() override;
	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void resizeRealSections();
	void setSelectedFunc(const string&, int callerID);
	void toggleSectionsVisibility(bool);
	void darkenSections(bool);
	//
	TSettingsParser::Settings currentSettings;
private:
	void loadSettings();
	void createProjectFile();
	void openProjectFile(File);
	void openProjectInfoWindow(TProjectParser::Project&);
	void chooseProjectFile();
	void saveProject();
	void closeProjectFile();
	void openSettingsWindow();
	void openAboutWindow();
	void openUsageWindow();
	void updateCurrentSettings();
	void setFontSizes();
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override;
	void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
	void showAlertWindow();
	//
	void buttonClicked(Button*) override;
	//
	void componentVisibilityChanged(Component& component) override;
	//
	//==============================================================================
	TProjectParser::Project project;
	//
	TextButton* saveSettingsButton = nullptr;
	//
	MenuBarComponent* menuBar = nullptr;
	//
	AsSubComponent* asPanel = nullptr;
	CodeSubComponent* codePanel = nullptr;
	AnalyzerSubComponent* analyzerPanel = nullptr;
	//
	PlaceholderSubComponent* asPlaceholderPanel = nullptr;
	PlaceholderSubComponent* codePlaceholderPanel = nullptr;
	PlaceholderSubComponent* analyzerPlaceholderPanel = nullptr;
	//
	MainPlaceholderSubComponent* mainPlaceholderPanel = nullptr;
	//
	CreateProjectWindow* createProjWindow = nullptr;
	SettingsWindow* settingsWindow = nullptr;
	AboutWindow* aboutWindow = nullptr;
	UsageWindow* usageWindow = nullptr;
	ProjectInfoWindow* projectInfoWindow = nullptr;
	//
	StretchableLayoutManager verticalLayout;
	std::unique_ptr<MyStretchableLayoutResizerBar> verticalDividerBarLeft;
	std::unique_ptr<MyStretchableLayoutResizerBar> verticalDividerBarRight;
	MyStretchableLayoutResizerBar::TopResizerBar* topResizerBarLeft = nullptr;
	MyStretchableLayoutResizerBar::TopResizerBar* topResizerBarRight = nullptr;
	//
	ScopedMessageBox messageBox;
	//
	std::unique_ptr<juce::FileChooser> chooser;
	File parentDirecory = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
	String defaultFilepath = parentDirecory.getFullPathName() + "/Projects/";
	//
	LookAndFeel_V4 lf;
	//
	DarkeningComponent* darkeningComponent = nullptr;
	//
	int menuHeight = 20;
	File currentProjectFile;
	bool projectOpened = false;
	bool realSectionsVisible = true;
	//
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
//
