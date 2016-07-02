#ifndef SWIEE_H
#define SWIEE_H

#include <QtGui>
//#include <Q3MainWindow>
#include <QMainWindow>
#include <QString>
#include <QDir>
//Added by qt3to4:
#include <QLabel>
#include <Q3PtrList>
//#include <Q3PopupMenu>
#include <QMouseEvent>
#include <QCloseEvent>
#include <Q3ScrollView>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QTabWidget>
#include <QDockWidget>
//#include <QListWidget>
#include <QSettings>
#include <qtextcodec.h>
#define GB2312(string) QTextCodec::codecForName("gb2312")->toUnicode((string))

//add by wangyuanlong start
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include "element.h"
class Schematic;
class Q3GridLayout;
class QValidator;
class Q3VBoxLayout;
class QIntValidator;
class QRegExpValidator;

//add by wangyuanlong end


class SwieeDoc;
class Schematic;
class SimMessage;
class MouseActions;
class SearchDialog;
class QLabel;
class QAction;
class QPrinter;
class QLineEdit;
class QComboBox;
class QTabWidget;
class Q3ListView;
class Q3ListViewItem;
class OctaveWindow;
class MessageDock;

typedef bool (Schematic::*pToggleFunc) ();
typedef void (MouseActions::*pMouseFunc) (Schematic*, QMouseEvent*);
typedef void (MouseActions::*pMouseFunc2) (Schematic*, QMouseEvent*, float, float);

class SwieeApp : public QMainWindow {
  Q_OBJECT
public:
  SwieeApp();
 ~SwieeApp();
  QSettings *swieeSettings;
  bool closeAllFiles();
  static int testFile(const QString&);
  bool gotoPage(const QString&);   // to load a document
  SwieeDoc *getDoc(int No=-1);
  SwieeDoc* findDoc (QString, int * Pos = 0);
  QString fileType (const QString&);

  QString ProjName;   // name of the project, that is open
  //QList<QString> swieePathList; // the swiee path list for subcircuits and spice files
  QHash<QString,QString> schNameHash; // QHash for the schematic files lookup
  QStringList spiceExtensions; // List of extensions used for spice files
  QString getSpiceFileFilter (void); // generate file filter string for spice files
  QHash<QString,QString> spiceNameHash; // QHash for the spice files lookup

  QLineEdit *editText;  // for edit component properties on schematic
  SearchDialog *SearchDia;  // global in order to keep values

  // current mouse methods
  void (MouseActions::*MouseMoveAction) (Schematic*, QMouseEvent*);
  void (MouseActions::*MousePressAction) (Schematic*, QMouseEvent*, float, float);
  void (MouseActions::*MouseDoubleClickAction) (Schematic*, QMouseEvent*);
  void (MouseActions::*MouseReleaseAction) (Schematic*, QMouseEvent*);

protected:
  void closeEvent(QCloseEvent*);

public slots:
  void slotFileNew();     // generate a new schematic in the view TabBar
  void slotTextNew();     // generate a new text editor in the view TabBar
  void slotFileOpen();    // open a document
  void slotFileSave();    // save a document
  void slotFileSaveAs();  // save a document under a different filename
  void slotFileSaveAll(); // save all open documents
  void slotFileClose();   // close the actual file
  void slotFileExamples();   // show the examples in a file browser
  void slotHelpTutorial();   // Open a pdf tutorial
  void slotHelpReport();   // Open a pdf report
  void slotHelpTechnical();   // Open a pdf technical document
  void slotFileClose (int); // close the file with given index
  void slotSymbolEdit();  // edit the symbol for the schematic
  void slotFileSettings();// open dialog to change file settings
  void slotFilePrint();   // print the current file
  void slotFilePrintFit();// Print and fit to page
  void slotFileQuit();    // exits the application
  void slotEditCut();     // put marked object into clipboard and delete it
  void slotEditCopy();    // put the marked object into the clipboard
  void slotApplSettings();// open dialog to change application settings
  void slotRefreshSchPath(); // refresh the schematic path hash

  void slotIntoHierarchy();
  void slotPopHierarchy();

  void slotShowAll();
  void slotShowOne();
  void slotZoomOut(); // Zoom out by 2

  void slotToPage();
  void slotSelectComponent(QListWidgetItem*);

  void slotEditElement();
  void slotPowerMatching();
  void slot2PortMatching();

  // for menu that appears by right click in content ListView
  void slotShowContentMenu(const QPoint &);
  //void slotShowContentMenu(Q3ListViewItem*, const QPoint&, int);
  void slotCMenuOpen();
  void slotCMenuCopy();
  void slotCMenuRename();
  void slotCMenuDelete();
  void slotCMenuDelGroup();
  void slotCMenuInsert();

private slots:
  void slotMenuOpenProject();
  void slotOpenProject(QListWidgetItem*);
  void slotMenuCloseProject();
  void slotSelectSubcircuit(QTreeWidgetItem*);
  void slotSelectLibComponent(QTreeWidgetItem*);
  void slotOpenContent(QTreeWidgetItem*);
  void slotSetCompView(int);
  void slotProjNewButt();
  void slotProjOpenButt();
  void slotProjDelButt();
  void slotMenuDelProject();
  void slotChangeView(QWidget*);
  void slotSimulate();
  void slotAfterSimulation(int, SimMessage*);
  void slotDCbias();
  void slotChangePage(QString&, QString&);
  void slotNextTab();
  void slotHideEdit();
signals:
  void signalKillEmAll();

public:
  MouseActions *view;
  QTabWidget *DocumentTab;
  QListWidget *CompComps;
  QTreeWidget *libTreeWidget;

  // menu appearing by right mouse button click on content listview
  QMenu *ContentMenu;

  // corresponding actions
  QAction *ActionCMenuOpen, *ActionCMenuCopy, *ActionCMenuRename, *ActionCMenuDelete, *ActionCMenuDelGroup, *ActionCMenuInsert;
  //file 菜单的原始控件
  QAction *fileNew, *textNew, *fileNewDpl, *fileOpen,*fileClose,/**recentfilesMenu,*/
		  *fileSave,*fileSaveAll,*fileSaveAs, *exportAsImage,*filePrint,*filePrintFit,
		  *fileExamples, *fileSettings, *symEdit , *applSettings, *refreshSchPath,
		  *fileQuit;
	//file 菜单的新控件	  
  QToolButton *fileNew1,*textNew1,*fileOpen1,*fileClose1,*recentfilesMenu1,
				*fileSave1,*fileSaveAll1,*fileSaveAs1,*exportAsImage1,*filePrint1,*filePrintFit1,
				*fileExamples1, *fileSettings1, *symEdit1,*applSettings1, *refreshSchPath1,
				*fileQuit1;


  QAction *activeAction;    // pointer to the action selected by the user

private:
  QPrinter  *Printer; // printer is global (to remember the user settings)

// ********* Widgets on the main area **********************************
  QDockWidget     *dock;
  QDockWidget     *menuDock;//菜单停靠栏
  QTabWidget      *menuTabView;//菜单选项卡

  QDockWidget     *QQTabDock;//菜单停靠栏
 // QTabWidget      *menuTabView;//菜单选项卡
  QToolButton *toolButton1_1,*toolButton1_2, *toolButton1_3, *toolButton1_4,*toolButton1_5, 
				*toolButton2_1, *toolButton2_2,
				*toolButton3_1, *toolButton3_2; 



  QDockWidget     *propertyDock;//属性停靠栏
  QDockWidget     *navigateDock;//导航
  QTreeWidget	  *NaviTreeWidget;




  QListWidget     *menuList;


  QTabWidget      *TabView;

  QWidget		*EleView; // added by xuliang

  QDockWidget     *octDock;
  OctaveWindow    *octave;
  MessageDock     *messageDock;

  QListWidget     *Projects;
  QTreeWidget     *Content;
  QTreeWidget     *Content1;
  QTreeWidgetItem *ConSchematics, *ConSources, *ConDisplays, *ConDatasets,
                  *ConOthers, *ConVerilog, *ConVerilogA, *ConOctave;


  QComboBox       *CompChoose;

// ********** Properties ************************************************
  Q3PtrList<QString> HierarchyHistory; // keeps track of "go into subcircuit"
  QString  SwieeFileFilter;

// ********** Methods ***************************************************
  void initView();
  void initCursorMenu();
  void initContentListView();

  void printCurrentDocument(bool);
  bool saveFile(SwieeDoc *Doc=0);
  bool saveAs();
  void openProject(const QString&, const QString&);
  bool deleteProject(const QString&, const QString&);
  void updatePortNumber(SwieeDoc*, int);
  void fillComboBox(bool);
  void switchSchematicDoc(bool);
  void switchEditMode(bool);
  void changeSchematicSymbolMode(Schematic*);
  bool deleteDirectoryContent(QDir& Dir);
  bool isTextDocument(QWidget *);
  void closeFile(int);

  void updateRecentFilesList(QString s);
  void successExportMessages(bool ok);
  void fillLibrariesTreeView (void);

public:

  void readProjects();
  void readProjectFiles();
  void updatePathList(void); // update the list of paths, pruning non-existing paths
  void updatePathList(QStringList);
  void updateSchNameHash(void); // maps all schematic files in the path list
  void updateSpiceNameHash(void); // maps all spice files in the path list

/* **************************************************
   *****  The following methods are located in  *****
   *****  "swiee_init.cpp".                      *****
   ************************************************** */

public slots:
  void slotShowWarnings();
  void slotResetWarnings();
  void printCursorPosition(int, int);

private slots:
  void slotViewToolBar(bool toggle);    // toggle the toolbar
  void slotViewStatusBar(bool toggle);  // toggle the statusbar
  void slotViewBrowseDock(bool toggle); // toggle the dock window
  void slotViewOctaveDock(bool); // toggle the dock window
  void slotToggleOctave(bool);
  void slotToggleDock(bool);
  void slotHelpAbout();     // shows an about dialog
  void slotHelpAboutQt();   // shows the standard about dialog for Qt

  void slotPropCellChanged(int row,int col);
  void slotShowSelectedItem(QTreeWidgetItem* treeW,int index);

private:
  void initActions();    // initializes all QActions of the application
  void initMenuBar();    // creates the menu_bar and inserts the menuitems
  void initToolBar();    // creates the toolbars
  void initStatusBar();  // setup the statusbar

  //设置菜单栏的按钮风格和属性
  void setButStyle(const QString ButName,QSize butSize,QToolButton *ToolBut);
  void setButTrr(QToolButton *ToolBut,
						const QKeySequence &key,
						const QString &astatusTip,
						const QString &awhatsThis);
  void SwieeApp::setButPropts(QToolButton *ToolBut,
						QSize butSize,
						const QKeySequence &key,
						const QString ButName,						
						const QString &astatusTip,
						const QString &awhatsThis);
  void initRibbon();
  void initMenuBarRibbon();
  void initQQTabDock();

  //属性框
  void initProptsDock();
  void initNavigateDock();


	Q3VBoxLayout *all;   // the mother of all widgets
	QValidator  *Validator, *ValRestrict, *Validator2;
	QRegExp     Expr;
	QIntValidator *ValInteger;
	QTableWidget   *PropTabWdg;
	QLineEdit   *edit, *NameEdit, *CompNameEdit;
	QCheckBox   *disp, *showName;
 //属性框-结束


  // menus contain the items of their menubar
  QMenu *fileMenu, *editMenu, *insMenu, *projMenu, *simMenu, *viewMenu,
             *helpMenu, *alignMenu, *toolMenu, *recentfilesMenu;

  // submenus for the PDF documents
  QMenu *helpTechnical, *helpReport, *helpTutorial;

  QToolBar *fileToolbar, *editToolbar, *viewToolbar, *workToolbar;

  // Shortcuts for scolling schematic / TextEdit
  // This is rather cumbersome -> Make this with a QScrollView instead??
  QShortcut *cursorUp, *cursorLeft, *cursorRight, *cursorDown;

  QLabel *WarningLabel, *PositionLabel;  // labels in status bar



/* **************************************************
   *****  The following methods are located in  *****
   *****  "swiee_actions.cpp".                   *****
   ************************************************** */

public:
	  void updateNavigateDock(Schematic *Doc);//更新导航窗
	  void updateProptsDock(Schematic *Doc,Element *Elem);//更新属性窗
	  bool ProptShowDone;
	  void getCurrentItem(QTableWidgetItem* item);

  void editFile(const QString&);
  //edit菜单的所有按钮
  QToolButton *undo1, *redo1,*editCut1,*editCopy1,*editPaste1,*editDelete1,
	  *select1,*selectAll1,*selectMarker1, *editFind1, *editFindAgain1,*changeProps1,
	   *editRotate1, *editMirror1, *editMirrorY1,*editActivate1, *intoH1, *popH1;
  //positioning菜单的所有按钮
  QToolButton  *moveText1,*onGrid1,*centerHor1, *centerVert1,
		   *alignTop1, *alignBottom1, *alignLeft1, *alignRight1,
		   *distrHor1, *distrVert1;
  //insert菜单的所有按钮
  QToolButton  *insWire1,*insLabel1,*insEquation1,*insGround1,*insPort1, *setMarker1, *insEntity1;
  //project菜单的所有按钮  12个
  QToolButton  *projNew1, *projOpen1, *addToProj1, *projDel1, *projClose1, 
			*createLib1,*createPkg1, *extractPkg1, *importData1, *graph2csv1, 
			*buildModule1,*loadModule1;
  //tools菜单的所有按钮
  QToolButton  *callEditor1, *callFilter1, *callLine1, *callLib1, *callMatch1, *callAtt1, *callRes1;


  //simulation菜单的所有原始按钮
  QToolButton  *simulate1, *dpl_sch1, *dcbias1, *showMsg1, *showNet1; 

  //view菜单的所有原始按钮  8个
  QToolButton  *magAll1, *magOne1,*magPlus1, *magMinus1,
			 *viewToolBar1, *viewStatusBar1,*viewBrowseDock1, *viewOctaveDock1;
  //help菜单的所有原始按钮
  QToolButton  *helpIndex1, *helpGetStart1, // submenus for the PDF documents QMenu *helpTechnical, *helpReport, *helpTutorial;
			*helpAboutApp1, *helpAboutQt1;
           






  //edit 的所有原始按钮 
  QAction *undo, *redo,*editCut,*editCopy,*editPaste,*editDelete,
	  *select,*selectAll,*selectMarker, *editFind, *editFindAgain,*changeProps,
	   *editRotate, *editMirror, *editMirrorY,*editActivate, *intoH, *popH;
  		  


  QAction  //positioning菜单的所有按钮
		   *moveText,*onGrid,*centerHor, *centerVert,
		   *alignTop, *alignBottom, *alignLeft, *alignRight,
		   *distrHor, *distrVert,
		   
		   //insert菜单的所有按钮
		   *insWire,*insLabel,*insEquation,*insGround, *insPort, *setMarker, *insEntity,  
			
		   //project菜单所有原始按钮
			*projNew, *projOpen, *addToProj, *projDel, *projClose, 
			*createLib,*createPkg, *extractPkg, *importData, *graph2csv, 
			*buildModule,*loadModule,

		   //tools菜单的所有原始按钮（1.9版本增加了一个按钮active filter）
           *callEditor, *callFilter, *callLine, *callLib, *callMatch, *callAtt, *callRes,
		   
			//simulation菜单的所有原始按钮
			*simulate, *dpl_sch, *dcbias, *showMsg, *showNet, 

			//view菜单的所有原始按钮  8个
			 *magAll, *magOne,*magPlus, *magMinus,
			 *viewToolBar, *viewStatusBar,*viewBrowseDock, *viewOctaveDock,
		   //help菜单的所有原始按钮
			 *helpIndex, *helpGetStart, // submenus for the PDF documents QMenu *helpTechnical, *helpReport, *helpTutorial;
			*helpAboutApp, *helpAboutQt;
           
          //*wire, 

       
         

public slots:
  void slotEditRotate(bool);  // rotate the selected items
  void slotEditMirrorX(bool); // mirror the selected items about X axis
  void slotEditMirrorY(bool); // mirror the selected items about Y axis
  void slotEditPaste(bool);   // paste the clipboard into the document
  void slotEditDelete(bool);  // delete the selected items
  void slotInsertEquation(bool);
  void slotInsertGround(bool);
  void slotInsertPort(bool);
  void slotInsertEntity();
  void slotSetWire(bool);
  void slotEscape();
  void slotSelect(bool);
  void slotEditActivate(bool);
  void slotInsertLabel(bool);
  void slotSetMarker(bool);
  void slotOnGrid(bool);      // set selected elements on grid
  void slotMoveText(bool);    // move property text of components
  void slotZoomIn(bool);
  void slotEditUndo();    // makes the last operation undone
  void slotEditRedo();    // makes the last undo undone
  void slotEditFind();    // searches for a piece of text
  void slotEditFindAgain();  // searches for the same piece of text again
  void slotAlignTop();    // align selected elements with respect to top
  void slotAlignBottom(); // align selected elements with respect to bottom
  void slotAlignLeft();   // align selected elements with respect to left
  void slotAlignRight();  // align selected elements with respect to right
  void slotDistribHoriz();// distribute horizontally selected elements
  void slotDistribVert(); // distribute vertically selected elements
  void slotCenterHorizontal();
  void slotCenterVertical();
  void slotSelectAll();
  void slotSelectMarker();
  void slotShowLastMsg();
  void slotShowLastNetlist();
  void slotCallEditor();
  void slotCallFilter();
  void slotCallLine();
  void slotCallLibrary();
  void slotCallMatch();
  void slotCallAtt();
  void slotCallRes();
  void slotHelpIndex();       // shows a HTML docu: Help Index
  void slotGettingStarted();  // shows a HTML docu: Getting started
  void slotChangeProps();
  void slotAddToProject();
  void slotApplyCompText();
  void slotOpenRecent(int num);
  void slotSaveDiagramToGraphicsFile();
  void slotSaveSchematicToGraphicsFile(bool diagram = false);

private slots:
  void slotCursorLeft();
  void slotCursorRight();
  void slotCursorUp();
  void slotCursorDown();
  void slotResizePropEdit(const QString&);
  void slotCreateLib();
  void slotImportData();
  void slotExportGraphAsCsv();
  void slotCreatePackage();
  void slotExtractPackage();
  void slotUpdateRecentFiles();
  void slotClearRecentFiles();
  void slotLoadModule();
  void slotBuildModule();
  
private:
  void showHTML(const QString&);
  bool performToggleAction(bool, QAction*, pToggleFunc, pMouseFunc, pMouseFunc2);
  friend class SaveDialog;
  QString lastExportFilename;
};

#endif /* QUCS_H */
