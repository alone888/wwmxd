#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <QtGui>

#include "main.h"
#include "swiee.h"
#include "dialogs/vtabbeddockwidget.h"
#include "octave_window.h"
//add by wangyuanlong
#include "schematic.h"
#include "element.h"
#include "mouseactions.h"
#include <Q3HBox>
#include <Q3VBox>
#include <QLayout>
#include <Q3HGroupBox>
#include <QValidator>
#include <QTabWidget>
#include <QFileDialog>
#include <Q3ListView>

//class MouseActions;
/**
* @brief SwieeApp::initActions Initializes all QActions of the application
*/
void SwieeApp::initActions()
{
	activeAction = 0;   // no active action

	// note: first argument of QAction() for backward compatibility Qt < 3.2
	//QResource::registerResource ("qucs.rcc");
	fileNew = new QAction(QIcon((":/bitmaps/filenew.png")), GB2312("新建电路[&n]"), this); //tr("&New"), this);
	fileNew->setShortcut(Qt::CTRL+Qt::Key_N);
	fileNew->setStatusTip(tr("Creates a new document"));
	fileNew->setWhatsThis(tr("New\n\nCreates a new schematic or data display document"));
	connect(fileNew1, SIGNAL(clicked()), SLOT(slotFileNew()));




	textNew = new QAction(QIcon((":/bitmaps/textnew.png")), GB2312("新建文档[&T]"), this);
	textNew->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_V);
	textNew->setStatusTip(tr("Creates a new text document"));
	textNew->setWhatsThis(tr("New Text\n\nCreates a new text document"));
	connect(textNew, SIGNAL(activated()), SLOT(slotTextNew()));
	connect(textNew1, SIGNAL(clicked()), SLOT(slotTextNew()));
	


	fileOpen = new QAction(QIcon((":/bitmaps/fileopen.png")),	GB2312("打开文档[&O]"), this);
	fileOpen->setShortcut(Qt::CTRL+Qt::Key_O);
	fileOpen->setStatusTip(tr("Opens an existing document"));
	fileOpen->setWhatsThis(tr("Open File\n\nOpens an existing document"));
	connect(fileOpen, SIGNAL(activated()), SLOT(slotFileOpen()));
	connect(fileOpen1, SIGNAL(clicked()), SLOT(slotFileOpen()));

	fileSave = new QAction(QIcon((":/bitmaps/filesave.png")),	tr("&Save"), this);
	fileSave->setShortcut(Qt::CTRL+Qt::Key_S);
	fileSave->setStatusTip(tr("Saves the current document"));
	fileSave->setWhatsThis(tr("Save File\n\nSaves the current document"));
	connect(fileSave, SIGNAL(activated()), SLOT(slotFileSave()));
	connect(fileSave1, SIGNAL(clicked()), SLOT(slotFileSave()));

	fileSaveAs = new QAction(tr("Save as..."), this);
	fileSaveAs->setShortcut(Qt::CTRL+Qt::Key_Minus);
	fileSaveAs->setStatusTip(
		tr("Saves the current document under a new filename"));
	fileSaveAs->setWhatsThis(
		tr("Save As\n\nSaves the current document under a new filename"));
	connect(fileSaveAs, SIGNAL(activated()), SLOT(slotFileSaveAs()));
	connect(fileSaveAs1, SIGNAL(clicked()), SLOT(slotFileSaveAs()));

	fileSaveAll = new QAction(QIcon((":/bitmaps/filesaveall.png")),	tr("Save &All"), this);
	fileSaveAll->setShortcut(Qt::CTRL+Qt::Key_Plus);
	fileSaveAll->setStatusTip(tr("Saves all open documents"));
	fileSaveAll->setWhatsThis(tr("Save All Files\n\nSaves all open documents"));
	connect(fileSaveAll, SIGNAL(activated()), SLOT(slotFileSaveAll()));
	connect(fileSaveAll1, SIGNAL(clicked()), SLOT(slotFileSaveAll()));

	fileClose = new QAction(QIcon((":/bitmaps/fileclose.png")), tr("&Close"), this);
	fileClose->setShortcut(Qt::CTRL+Qt::Key_W);
	fileClose->setStatusTip(tr("Closes the current document"));
	fileClose->setWhatsThis(tr("Close File\n\nCloses the current document"));
	connect(fileClose, SIGNAL(activated()), SLOT(slotFileClose()));
	connect(fileClose1, SIGNAL(clicked()), SLOT(slotFileClose()));

	fileExamples = new QAction(tr("&Examples"), this);
	fileExamples->setStatusTip(tr("Opens a file explorer with example documents"));
	fileExamples->setWhatsThis(
		tr("Examples\n\nOpens a file explorer with example documents"));
	connect(fileExamples, SIGNAL(activated()), SLOT(slotFileExamples()));
	connect(fileExamples1, SIGNAL(clicked()), SLOT(slotFileExamples()));


	symEdit = new QAction(tr("&Edit Circuit Symbol"), this);
	symEdit->setShortcut(Qt::Key_F9);
	symEdit->setStatusTip(tr("Edits the symbol for this schematic"));
	symEdit->setWhatsThis(
		tr("Edit Circuit Symbol\n\nEdits the symbol for this schematic"));
	connect(symEdit, SIGNAL(activated()), SLOT(slotSymbolEdit()));
	connect(symEdit1, SIGNAL(clicked()), SLOT(slotSymbolEdit()));

	fileSettings = new QAction(tr("&Document Settings..."), this);
	fileSettings->setShortcut(Qt::CTRL+Qt::Key_Period);
	fileSettings->setStatusTip(tr("Document Settings"));
	fileSettings->setWhatsThis(tr("Settings\n\nSets properties of the file"));
	connect(fileSettings, SIGNAL(activated()), SLOT(slotFileSettings()));
	connect(fileSettings1, SIGNAL(clicked()), SLOT(slotFileSettings()));

	filePrint = new QAction(QIcon((":/bitmaps/fileprint.png")), tr("&Print..."), this);
	filePrint->setShortcut(Qt::CTRL+Qt::Key_P);
	filePrint->setStatusTip(tr("Prints the current document"));
	filePrint->setWhatsThis(tr("Print File\n\nPrints the current document"));
	connect(filePrint, SIGNAL(activated()), SLOT(slotFilePrint()));
	connect(filePrint1, SIGNAL(clicked()), SLOT(slotFilePrint()));

	filePrintFit = new QAction(tr("Print Fit to Page..."), this);
	filePrintFit->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_P);
	filePrintFit->setStatusTip(tr("Print Fit to Page"));
	filePrintFit->setWhatsThis(
		tr("Print Fit to Page\n\n"
		"Print and fit content to the page size"));
	connect(filePrintFit, SIGNAL(activated()), SLOT(slotFilePrintFit()));
	connect(filePrintFit1, SIGNAL(clicked()), SLOT(slotFilePrintFit()));

	fileQuit = new QAction(tr("E&xit"), this);
	fileQuit->setShortcut(Qt::CTRL+Qt::Key_Q);
	fileQuit->setStatusTip(tr("Quits the application"));
	fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
	connect(fileQuit, SIGNAL(activated()), SLOT(slotFileQuit()));
	connect(fileQuit1, SIGNAL(clicked()), SLOT(slotFileQuit()));

	applSettings = new QAction(tr("Application Settings..."), this);
	applSettings->setShortcut(Qt::CTRL+Qt::Key_Comma);
	applSettings->setStatusTip(tr("Application Settings"));
	applSettings->setWhatsThis(
		tr("Swiee Settings\n\nSets properties of the application"));
	connect(applSettings, SIGNAL(activated()), SLOT(slotApplSettings()));
	connect(applSettings1, SIGNAL(clicked()), SLOT(slotApplSettings()));

	refreshSchPath = new QAction(tr("Refresh Search Path..."), this);
	//refreshSchPath->setShortcut(Qt::CTRL+Qt::Key_Comma);
	refreshSchPath->setStatusTip(tr("Refresh Search Path"));
	refreshSchPath->setWhatsThis(
		tr("Refresh Path\n\nRechecks the list of paths for subcircuit files."));
	connect(refreshSchPath, SIGNAL(activated()), SLOT(slotRefreshSchPath()));
	connect(refreshSchPath1, SIGNAL(clicked()), SLOT(slotRefreshSchPath()));

	alignTop = new QAction(tr("Align top"), this);
	alignTop->setShortcut(Qt::CTRL+Qt::Key_T);
	alignTop->setStatusTip(tr("Align top selected elements"));
	alignTop->setWhatsThis(
		tr("Align top\n\nAlign selected elements to their upper edge"));
	connect(alignTop, SIGNAL(activated()), SLOT(slotAlignTop()));
	connect(alignTop1, SIGNAL(clicked()), SLOT(slotAlignTop()));

	alignBottom = new QAction(tr("Align bottom"), this);
	alignBottom->setStatusTip(tr("Align bottom selected elements"));
	alignBottom->setWhatsThis(
		tr("Align bottom\n\nAlign selected elements to their lower edge"));
	connect(alignBottom, SIGNAL(activated()), SLOT(slotAlignBottom()));
	connect(alignBottom1, SIGNAL(clicked()), SLOT(slotAlignBottom()));

	alignLeft = new QAction(tr("Align left"), this);
	alignLeft->setStatusTip(tr("Align left selected elements"));
	alignLeft->setWhatsThis(
		tr("Align left\n\nAlign selected elements to their left edge"));
	connect(alignLeft, SIGNAL(activated()), SLOT(slotAlignLeft()));
	connect(alignLeft1, SIGNAL(clicked()), SLOT(slotAlignLeft()));

	alignRight = new QAction(tr("Align right"), this);
	alignRight->setStatusTip(tr("Align right selected elements"));
	alignRight->setWhatsThis(
		tr("Align right\n\nAlign selected elements to their right edge"));
	connect(alignRight, SIGNAL(activated()), SLOT(slotAlignRight()));
	connect(alignRight1, SIGNAL(clicked()), SLOT(slotAlignRight()));


	distrHor = new QAction(tr("Distribute horizontally"), this);
	distrHor->setStatusTip(tr("Distribute equally horizontally"));
	distrHor->setWhatsThis(
		tr("Distribute horizontally\n\n"
		"Distribute horizontally selected elements"));
	connect(distrHor, SIGNAL(activated()), SLOT(slotDistribHoriz()));
	connect(distrHor1, SIGNAL(clicked()), SLOT(slotDistribHoriz()));

	distrVert = new QAction(tr("Distribute vertically"), this);
	distrVert->setStatusTip(tr("Distribute equally vertically"));
	distrVert->setWhatsThis(
		tr("Distribute vertically\n\n"
		"Distribute vertically selected elements"));
	connect(distrVert, SIGNAL(activated()), SLOT(slotDistribVert()));
	connect(distrVert1, SIGNAL(clicked()), SLOT(slotDistribVert()));

	centerHor = new QAction(tr("Center horizontally"), this);
	centerHor->setStatusTip(tr("Center horizontally selected elements"));
	centerHor->setWhatsThis(
		tr("Center horizontally\n\nCenter horizontally selected elements"));
	connect(centerHor, SIGNAL(activated()), SLOT(slotCenterHorizontal()));
	connect(centerHor1, SIGNAL(clicked()), SLOT(slotCenterHorizontal()));

	centerVert = new QAction(tr("Center vertically"), this);
	centerVert->setStatusTip(tr("Center vertically selected elements"));
	centerVert->setWhatsThis(
		tr("Center vertically\n\nCenter vertically selected elements"));
	connect(centerHor, SIGNAL(activated()), SLOT(slotCenterHorizontal()));
	connect(centerHor1, SIGNAL(clicked()), SLOT(slotCenterHorizontal()));
	

	onGrid = new QAction(tr("Set on Grid"), this);
	onGrid->setShortcut(Qt::CTRL+Qt::Key_U);
	onGrid->setStatusTip(tr("Sets selected elements on grid"));
	onGrid->setWhatsThis(
		tr("Set on Grid\n\nSets selected elements on grid"));
	onGrid->setToggleAction(true);
	onGrid1->setToggleButton(true);
	connect(onGrid, SIGNAL(toggled(bool)), SLOT(slotOnGrid(bool)));

	connect(onGrid1, SIGNAL(clicked()), SLOT(slotOnGrid(bool)));

	moveText = new QAction(tr("Move Component Text"), this);
	moveText->setShortcut(Qt::CTRL+Qt::Key_K);
	moveText->setStatusTip(tr("Moves the property text of components"));
	moveText->setWhatsThis(
		tr("Move Component Text\n\nMoves the property text of components"));
	moveText->setToggleAction(true);
	moveText1->setToggleButton(true);
	connect(moveText, SIGNAL(toggled(bool)), SLOT(slotMoveText(bool)));
	connect(moveText1, SIGNAL(clicked()), SLOT(slotMoveText(bool)));

	changeProps = new QAction(tr("Replace..."), this);
	changeProps->setShortcut(Qt::Key_F7);
	changeProps->setStatusTip(tr("Replace component properties or VHDL code"));
	changeProps->setWhatsThis(
		tr("Replace\n\nChange component properties\nor\ntext in VHDL code"));
	connect(changeProps, SIGNAL(activated()), SLOT(slotChangeProps()));
	connect(changeProps1, SIGNAL(clicked()), SLOT(slotChangeProps()));

	editCut = new QAction(QIcon((":/bitmaps/editcut.png")),	tr("Cu&t"), this);
	editCut->setShortcut(Qt::CTRL+Qt::Key_X);
	editCut->setStatusTip(
		tr("Cuts out the selection and puts it into the clipboard"));
	editCut->setWhatsThis(
		tr("Cut\n\nCuts out the selection and puts it into the clipboard"));
	connect(editCut, SIGNAL(activated()), SLOT(slotEditCut()));
	connect(editCut1, SIGNAL(clicked()), SLOT(slotEditCut()));

	editCopy = new QAction(QIcon((":/bitmaps/editcopy.png")), tr("&Copy"), this);
	editCopy->setShortcut(Qt::CTRL+Qt::Key_C);
	editCopy->setStatusTip(
		tr("Copies the selection into the clipboard"));
	editCopy->setWhatsThis(
		tr("Copy\n\nCopies the selection into the clipboard"));
	connect(editCopy, SIGNAL(activated()), SLOT(slotEditCopy()));
	connect(editCopy1, SIGNAL(clicked()), SLOT(slotEditCopy()));

	editPaste = new QAction(QIcon((":/bitmaps/editpaste.png")), tr("&Paste"), this);
	editPaste->setShortcut(Qt::CTRL+Qt::Key_V);
	editPaste->setStatusTip(
		tr("Pastes the clipboard contents to the cursor position"));
	editPaste->setWhatsThis(
		tr("Paste\n\nPastes the clipboard contents to the cursor position"));
	editPaste->setToggleAction(true);
	editPaste1->setToggleButton(true);
	connect(editPaste, SIGNAL(toggled(bool)), SLOT(slotEditPaste(bool)));
	connect(editPaste1, SIGNAL(toggled(bool)), SLOT(slotEditPaste(bool)));

	editDelete = new QAction(QIcon((":/bitmaps/editdelete.png")), tr("&Delete"), this);
	editDelete->setShortcut(Qt::Key_Delete);
	editDelete->setStatusTip(tr("Deletes the selected components"));
	editDelete->setWhatsThis(tr("Delete\n\nDeletes the selected components"));
	editDelete->setToggleAction(true);
	editDelete1->setToggleButton(true);
	connect(editDelete, SIGNAL(toggled(bool)), SLOT(slotEditDelete(bool)));
	connect(editDelete1, SIGNAL(toggled(bool)), SLOT(slotEditDelete(bool)));

	editFind = new QAction(tr("Find..."), this);
	editFind->setShortcut(Qt::CTRL+Qt::Key_F);
	editFind->setStatusTip(tr("Find a piece of text"));
	editFind->setWhatsThis(tr("Find\n\nSearches for a piece of text"));
	connect(editFind, SIGNAL(activated()), SLOT(slotEditFind()));
	connect(editFind1, SIGNAL(clicked()), SLOT(slotEditFind()));

	editFindAgain = new QAction(tr("Find Again"), this);
	editFindAgain->setShortcut(Qt::Key_F3);
	editFindAgain->setStatusTip(tr("Find same text again"));
	editFindAgain->setWhatsThis(
		tr("Find\n\nSearches for the same piece of text again"));
	connect(editFindAgain, SIGNAL(activated()), SLOT(slotEditFindAgain()));
	connect(editFindAgain1, SIGNAL(clicked()), SLOT(slotEditFindAgain()));

	// to ease usage with notebooks, backspace can also be used to delete
	// currently not supported
	//mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Backspace),
	//                       editDelete, SLOT(toggle()) );

	exportAsImage = new QAction(tr("Export as image"),this);
	connect(exportAsImage,SIGNAL(activated()),SLOT(slotSaveSchematicToGraphicsFile()));
	connect(exportAsImage,SIGNAL(clicked()),SLOT(slotSaveSchematicToGraphicsFile()));

	// cursor left/right/up/down to move marker on a graph
	// 下边这四个只是快捷方式
	cursorLeft = new QShortcut(QKeySequence(Qt::Key_Left), this);
	connect(cursorLeft, SIGNAL(activated()), SLOT(slotCursorLeft()));


	cursorRight = new QShortcut(QKeySequence(Qt::Key_Right), this);
	connect(cursorRight, SIGNAL(activated()), SLOT(slotCursorRight()));


	cursorUp = new QShortcut(QKeySequence(Qt::Key_Up), this);
	connect(cursorUp, SIGNAL(activated()), SLOT(slotCursorUp()));

	cursorDown = new QShortcut(QKeySequence(Qt::Key_Down), this);
	connect(cursorDown, SIGNAL(activated()), SLOT(slotCursorDown()));


	/*
	mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Left),
	this, SLOT(slotCursorLeft()));
	mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Right),
	this, SLOT(slotCursorRight()));
	mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Up),
	this, SLOT(slotCursorUp()));
	mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Down),
	this, SLOT(slotCursorDown()));
	mainAccel->connectItem(mainAccel->insertItem(Qt::Key_Tab),
	this, SLOT(slotNextTab()));
	*/

	undo = new QAction(QIcon((":/bitmaps/undo.png")), tr("&Undo"), this);
	undo->setShortcut(Qt::CTRL+Qt::Key_Z);
	undo->setStatusTip(tr("Undoes the last command"));
	undo->setWhatsThis(tr("Undo\n\nMakes the last action undone"));
	connect(undo, SIGNAL(activated()), SLOT(slotEditUndo()));
	connect(undo1, SIGNAL(clicked()), SLOT(slotEditUndo()));

	redo = new QAction(QIcon((":/bitmaps/redo.png")), tr("&Redo"), this);
	redo->setShortcut(Qt::CTRL+Qt::Key_Y);
	redo->setStatusTip(tr("Redoes the last command"));
	redo->setWhatsThis(tr("Redo\n\nRepeats the last action once more"));
	connect(redo, SIGNAL(activated()), SLOT(slotEditRedo()));
	connect(redo1, SIGNAL(clicked()), SLOT(slotEditRedo()));

	projNew = new QAction(tr("&New Project..."), this);
	projNew->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_N);
	projNew->setStatusTip(tr("Creates a new project"));
	projNew->setWhatsThis(tr("New Project\n\nCreates a new project"));
	connect(projNew, SIGNAL(activated()), SLOT(slotProjNewButt()));
	connect(projNew1, SIGNAL(clicked()), SLOT(slotProjNewButt()));

	projOpen = new QAction(tr("&Open Project..."), this);
	projOpen->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_O);
	projOpen->setStatusTip(tr("Opens an existing project"));
	projOpen->setWhatsThis(tr("Open Project\n\nOpens an existing project"));
	connect(projOpen, SIGNAL(activated()), SLOT(slotMenuOpenProject()));
	connect(projOpen1, SIGNAL(clicked()), SLOT(slotMenuOpenProject()));

	projDel = new QAction(tr("&Delete Project..."), this);
	projDel->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_D);
	projDel->setStatusTip(tr("Deletes an existing project"));
	projDel->setWhatsThis(tr("Delete Project\n\nDeletes an existing project"));
	connect(projDel, SIGNAL(activated()), SLOT(slotMenuDelProject()));
	connect(projDel1, SIGNAL(clicked()), SLOT(slotMenuDelProject()));

	projClose = new QAction(tr("&Close Project"), this);
	projClose->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_W);
	projClose->setStatusTip(tr("Closes the current project"));
	projClose->setWhatsThis(tr("Close Project\n\nCloses the current project"));
	connect(projClose, SIGNAL(activated()), SLOT(slotMenuCloseProject()));
	connect(projClose1, SIGNAL(clicked()), SLOT(slotMenuCloseProject()));

	addToProj = new QAction(tr("&Add Files to Project..."), this);
	addToProj->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_A);
	addToProj->setStatusTip(tr("Copies files to project directory"));
	addToProj->setWhatsThis(
		tr("Add Files to Project\n\nCopies files to project directory"));
	connect(addToProj, SIGNAL(activated()), SLOT(slotAddToProject()));
	connect(addToProj1, SIGNAL(clicked()), SLOT(slotAddToProject()));

	createLib = new QAction(tr("Create &Library..."), this);
	createLib->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_L);
	createLib->setStatusTip(tr("Create Library from Subcircuits"));
	createLib->setWhatsThis(
		tr("Create Library\n\nCreate Library from Subcircuits"));
	connect(createLib, SIGNAL(activated()), SLOT(slotCreateLib()));
	connect(createLib1, SIGNAL(clicked()), SLOT(slotCreateLib()));

	createPkg = new QAction(tr("Create &Package..."), this);
	createPkg->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_Z);
	createPkg->setStatusTip(tr("Create compressed Package from Projects"));
	createPkg->setWhatsThis(
		tr("Create Package\n\nCreate compressed Package from complete Projects"));
	connect(createPkg, SIGNAL(activated()), SLOT(slotCreatePackage()));
	connect(createPkg1, SIGNAL(clicked()), SLOT(slotCreatePackage()));

	extractPkg = new QAction(tr("E&xtract Package..."),  this);
	extractPkg->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_X);
	extractPkg->setStatusTip(tr("Install Content of a Package"));
	extractPkg->setWhatsThis(
		tr("Extract Package\n\nInstall Content of a Package"));
	connect(extractPkg, SIGNAL(activated()), SLOT(slotExtractPackage()));
	connect(extractPkg1, SIGNAL(clicked()), SLOT(slotExtractPackage()));

	importData = new QAction(tr("&Import/Export Data..."), this);
	importData->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_I);
	importData->setStatusTip(tr("Convert data file"));
	importData->setWhatsThis(
		tr("Import/Export Data\n\nConvert data file to various file formats"));
	connect(importData, SIGNAL(activated()), SLOT(slotImportData()));
	connect(importData1, SIGNAL(clicked()), SLOT(slotImportData()));

	graph2csv = new QAction(tr("Export to &CSV..."), this);
	graph2csv->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_C);
	graph2csv->setStatusTip(tr("Convert graph data to CSV file"));
	graph2csv->setWhatsThis(
		tr("Export to CSV\n\nConvert graph data to CSV file"));
	connect(graph2csv, SIGNAL(activated()), SLOT(slotExportGraphAsCsv()));
	connect(graph2csv1, SIGNAL(clicked()), SLOT(slotExportGraphAsCsv()));

	buildModule = new QAction(tr("Build Verilog-A module..."), this);
	buildModule->setStatusTip(tr("Run admsXml and C++ compiler"));
	buildModule->setWhatsThis(tr("Build Verilog-A module\nRuns amdsXml and C++ compiler"));
	connect(buildModule, SIGNAL(activated()), SLOT(slotBuildModule()));
	connect(buildModule1, SIGNAL(clicked()), SLOT(slotBuildModule()));

	loadModule = new QAction(tr("Load Verilog-A module..."), this);
	loadModule->setStatusTip(tr("Select Verilog-A symbols to be loaded"));
	loadModule->setWhatsThis(tr("Load Verilog-A module\nLet the user select and load symbols"));
	connect(loadModule, SIGNAL(activated()), SLOT(slotLoadModule()));
	connect(loadModule1, SIGNAL(clicked()), SLOT(slotLoadModule()));

	magAll = new QAction(QIcon((":/bitmaps/viewmagfit.png")), tr("View All"), this);
	magAll->setShortcut(Qt::Key_0);
	magAll->setStatusTip(tr("Show the whole page"));
	magAll->setWhatsThis(tr("View All\n\nShows the whole page content"));
	connect(magAll, SIGNAL(activated()), SLOT(slotShowAll()));
	connect(magAll1, SIGNAL(clicked()), SLOT(slotShowAll()));

	magOne = new QAction(QIcon((":/bitmaps/viewmag1.png")), tr("View 1:1"), this);
	magOne->setShortcut(Qt::Key_1);
	magOne->setStatusTip(tr("Views without magnification"));
	magOne->setWhatsThis(tr("View 1:1\n\nShows the page content without magnification"));
	connect(magOne, SIGNAL(activated()), SLOT(slotShowOne()));
	connect(magOne1, SIGNAL(clicked()), SLOT(slotShowOne()));

	magPlus = new QAction(QIcon((":/bitmaps/viewmag+.png")),	tr("Zoom in"), this);
	magPlus->setShortcut(Qt::Key_Plus);
	magPlus->setStatusTip(tr("Zooms into the current view"));
	magPlus->setWhatsThis(tr("Zoom in\n\nZooms the current view"));
	magPlus->setToggleAction(true);
	magPlus1->setToggleButton(true);
	connect(magPlus, SIGNAL(toggled(bool)), SLOT(slotZoomIn(bool)));
	connect(magPlus1, SIGNAL(toggled(bool)), SLOT(slotZoomIn(bool)));

	magMinus = new QAction(QIcon((":/bitmaps/viewmag-.png")), tr("Zoom out"), this);
	magMinus->setShortcut(Qt::Key_Minus);
	magMinus->setStatusTip(tr("Zooms out the current view"));
	magMinus->setWhatsThis(tr("Zoom out\n\nZooms out the current view"));
	connect(magMinus, SIGNAL(activated()), SLOT(slotZoomOut()));
	connect(magMinus1, SIGNAL(clicked()), SLOT(slotZoomOut()));

	//按下Esc键
	QAction *escape = new QAction(this);
	escape->setShortcut(Qt::Key_Escape);
	connect(escape, SIGNAL(activated()), SLOT(slotEscape()));
	this->addAction(escape);

	select = new QAction(QIcon((":/bitmaps/pointer.png")), tr("Select"), this);
	select->setStatusTip(tr("Activate select mode"));
	select->setWhatsThis(tr("Select\n\nActivates select mode"));
	select->setToggleAction(true);
	select1->setToggleButton(true);
	connect(select, SIGNAL(toggled(bool)), SLOT(slotSelect(bool)));
	connect(select1, SIGNAL(toggled(bool)), SLOT(slotSelect(bool)));

	selectAll = new QAction(tr("Select All"), this);
	selectAll->setShortcut(Qt::CTRL+Qt::Key_A);
	selectAll->setStatusTip(tr("Selects all elements"));
	selectAll->setWhatsThis(
		tr("Select All\n\nSelects all elements of the document"));
	connect(selectAll, SIGNAL(activated()), SLOT(slotSelectAll()));
	connect(selectAll1, SIGNAL(clicked()), SLOT(slotSelectAll()));

	selectMarker = new QAction(tr("Select Markers"), this);
	selectMarker->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_M);
	selectMarker->setStatusTip(tr("Selects all markers"));
	selectMarker->setWhatsThis(
		tr("Select Markers\n\nSelects all diagram markers of the document"));
	connect(selectMarker, SIGNAL(activated()), SLOT(slotSelectMarker()));
	connect(selectMarker1, SIGNAL(clicked()), SLOT(slotSelectMarker()));

	editRotate = new QAction(QIcon((":/bitmaps/rotate_ccw.png")), tr("Rotate"), this);
	editRotate->setShortcut(Qt::CTRL+Qt::Key_R);
	editRotate->setStatusTip(tr("Rotates the selected component by 90\x00B0"));
	editRotate->setWhatsThis(
		tr("Rotate\n\nRotates the selected component by 90\x00B0 counter-clockwise"));
	editRotate->setToggleAction(true);
	editRotate1->setToggleButton(true);
	connect(editRotate, SIGNAL(toggled(bool)), SLOT(slotEditRotate(bool)));
	connect(editRotate1, SIGNAL(toggled(bool)), SLOT(slotEditRotate(bool)));

	editMirror = new QAction(QIcon((":/bitmaps/mirror.png")), tr("Mirror about X Axis"), this);
	editMirror->setShortcut(Qt::CTRL+Qt::Key_J);
	editMirror->setStatusTip(tr("Mirrors the selected item about X Axis"));
	editMirror->setWhatsThis(
		tr("Mirror about X Axis\n\nMirrors the selected item about X Axis"));
	editMirror->setToggleAction(true);
	editMirror1->setToggleButton(true);
	connect(editMirror, SIGNAL(toggled(bool)), SLOT(slotEditMirrorX(bool)));
	connect(editMirror1, SIGNAL(toggled(bool)), SLOT(slotEditMirrorX(bool)));

	editMirrorY = new QAction(QIcon((":/bitmaps/mirrory.png")), tr("Mirror about Y Axis"), this);
	editMirrorY->setShortcut(Qt::CTRL+Qt::Key_M);
	editMirrorY->setStatusTip(tr("Mirrors the selected item about Y Axis"));
	editMirrorY->setWhatsThis(
		tr("Mirror about Y Axis\n\nMirrors the selected item about Y Axis"));
	editMirrorY->setToggleAction(true);
	editMirrorY1->setToggleButton(true);
	connect(editMirrorY, SIGNAL(toggled(bool)), SLOT(slotEditMirrorY(bool)));
	connect(editMirrorY1, SIGNAL(toggled(bool)), SLOT(slotEditMirrorY(bool)));

	intoH = new QAction(QIcon((":/bitmaps/bottom.png")), tr("Go into Subcircuit"), this);
	intoH->setShortcut(Qt::CTRL+Qt::Key_I);
	intoH->setStatusTip(tr("Goes inside the selected subcircuit"));
	intoH->setWhatsThis(
		tr("Go into Subcircuit\n\nGoes inside the selected subcircuit"));
	connect(intoH, SIGNAL(activated()), SLOT(slotIntoHierarchy()));
	connect(intoH1, SIGNAL(clicked()), SLOT(slotIntoHierarchy()));

	popH = new QAction(QIcon((":/bitmaps/top.png")), tr("Pop out"), this);
	popH->setShortcut(Qt::CTRL+Qt::Key_H);
	popH->setStatusTip(tr("Pop outside subcircuit"));
	popH->setWhatsThis(
		tr("Pop out\n\nGoes up one hierarchy level, i.e. leaves subcircuit"));
	connect(popH, SIGNAL(activated()), SLOT(slotPopHierarchy()));
	connect(popH1, SIGNAL(clicked()), SLOT(slotPopHierarchy()));
	popH->setEnabled(false);  // only enabled if useful !!!!

	editActivate = new QAction(QIcon((":/bitmaps/deactiv.png")),	tr("Deactivate/Activate"), this);
	editActivate->setShortcut(Qt::CTRL+Qt::Key_D);
	editActivate->setStatusTip(tr("Deactivate/Activate selected components"));
	editActivate->setWhatsThis(
		tr("Deactivate/Activate\n\nDeactivate/Activate the selected components"));
	editActivate->setToggleAction(true);
	editActivate1->setToggleButton(true);
	connect(editActivate, SIGNAL(toggled(bool)), SLOT(slotEditActivate(bool)));
	connect(editActivate1, SIGNAL(toggled(bool)), SLOT(slotEditActivate(bool)));

	insEquation = new QAction(QIcon((":/bitmaps/equation.png")),	tr("Insert Equation"), this);
	insEquation->setShortcut(Qt::CTRL+Qt::Key_Less);
	insEquation->setStatusTip(tr("Inserts an equation"));
	insEquation->setWhatsThis(
		tr("Insert Equation\n\nInserts a user defined equation"));
	insEquation->setToggleAction(true);
	insEquation1->setToggleButton(true);
	connect(insEquation, SIGNAL(toggled(bool)), SLOT(slotInsertEquation(bool)));
	connect(insEquation1, SIGNAL(toggled(bool)), SLOT(slotInsertEquation(bool)));

	insGround = new QAction(QIcon((":/bitmaps/ground.png")), tr("Insert Ground"), this);
	insGround->setShortcut(Qt::CTRL+Qt::Key_G);
	insGround->setStatusTip(tr("Inserts a ground symbol"));
	insGround->setWhatsThis(tr("Insert Ground\n\nInserts a ground symbol"));
	insGround->setToggleAction(true);
	insGround1->setToggleButton(true);
	connect(insGround, SIGNAL(toggled(bool)), SLOT(slotInsertGround(bool)));
	connect(insGround1, SIGNAL(toggled(bool)), SLOT(slotInsertGround(bool)));

	insPort = new QAction(QIcon((":/bitmaps/port.png")),	tr("Insert Port"), this);
	insPort->setStatusTip(tr("Inserts a port symbol"));
	insPort->setWhatsThis(tr("Insert Port\n\nInserts a port symbol"));
	insPort->setToggleAction(true);
	insPort1->setToggleButton(true);
	connect(insPort, SIGNAL(toggled(bool)), SLOT(slotInsertPort(bool)));
	connect(insPort1, SIGNAL(toggled(bool)), SLOT(slotInsertPort(bool)));

	insWire = new QAction(QIcon((":/bitmaps/wire.png")),	tr("Wire"), this);
	insWire->setShortcut(Qt::CTRL+Qt::Key_E);
	insWire->setStatusTip(tr("Inserts a wire"));
	insWire->setWhatsThis(tr("Wire\n\nInserts a wire"));
	insWire->setToggleAction(true);
	insWire1->setToggleButton(true);
	connect(insWire, SIGNAL(toggled(bool)), SLOT(slotSetWire(bool)));
	connect(insWire1, SIGNAL(toggled(bool)), SLOT(slotSetWire(bool)));

	insLabel = new QAction(QIcon((":/bitmaps/nodename.png")), tr("Wire Label"), this);
	insLabel->setShortcut(Qt::CTRL+Qt::Key_L);
	insLabel->setStatusTip(tr("Inserts a wire or pin label"));
	insLabel->setWhatsThis(tr("Wire Label\n\nInserts a wire or pin label"));
	insLabel->setToggleAction(true);
	insLabel1->setToggleButton(true);
	connect(insLabel, SIGNAL(toggled(bool)), SLOT(slotInsertLabel(bool)));
	connect(insLabel1, SIGNAL(toggled(bool)), SLOT(slotInsertLabel(bool)));

	insEntity = new QAction(tr("VHDL entity"), this);
	insEntity->setShortcut(Qt::CTRL+Qt::Key_Space);
	insEntity->setStatusTip(tr("Inserts skeleton of VHDL entity"));
	insEntity->setWhatsThis(
		tr("VHDL entity\n\nInserts the skeleton of a VHDL entity"));
	connect(insEntity, SIGNAL(activated()), SLOT(slotInsertEntity()));
	connect(insEntity1, SIGNAL(clicked()), SLOT(slotInsertEntity()));

	callEditor = new QAction(tr("Text Editor"), this);
	callEditor->setShortcut(Qt::CTRL+Qt::Key_1);
	callEditor->setStatusTip(tr("Starts the Swiee text editor"));
	callEditor->setWhatsThis(tr("Text editor\n\nStarts the Swiee text editor"));
	connect(callEditor, SIGNAL(activated()), SLOT(slotCallEditor()));
	connect(callEditor1, SIGNAL(clicked()), SLOT(slotCallEditor()));

	callFilter = new QAction(tr("Filter synthesis"), this);
	callFilter->setShortcut(Qt::CTRL+Qt::Key_2);
	callFilter->setStatusTip(tr("Starts SwieeFilter"));
	callFilter->setWhatsThis(tr("Filter synthesis\n\nStarts SwieeFilter"));
	connect(callFilter, SIGNAL(activated()), SLOT(slotCallFilter()));
	connect(callFilter1, SIGNAL(clicked()), SLOT(slotCallFilter()));

	callLine = new QAction(tr("Line calculation"), this);
	callLine->setShortcut(Qt::CTRL+Qt::Key_3);
	callLine->setStatusTip(tr("Starts SwieeTrans"));
	callLine->setWhatsThis(
		tr("Line calculation\n\nStarts transmission line calculator"));
	connect(callLine, SIGNAL(activated()), SLOT(slotCallLine()));
	connect(callLine1, SIGNAL(clicked()), SLOT(slotCallLine()));

	callLib = new QAction(tr("Component Library"), this);
	callLib->setShortcut(Qt::CTRL+Qt::Key_4);
	callLib->setStatusTip(tr("Starts SwieeLib"));
	callLib->setWhatsThis(
		tr("Component Library\n\nStarts component library program"));
	connect(callLib, SIGNAL(activated()), SLOT(slotCallLibrary()));
	connect(callLib1, SIGNAL(clicked()), SLOT(slotCallLibrary()));

	callMatch = new QAction(tr("Matching Circuit"), this);
	callMatch->setShortcut(Qt::CTRL+Qt::Key_5);
	callMatch->setStatusTip(tr("Creates Matching Circuit"));
	callMatch->setWhatsThis(
		tr("Matching Circuit\n\nDialog for Creating Matching Circuit"));
	connect(callMatch, SIGNAL(activated()), SLOT(slotCallMatch()));
	connect(callMatch1, SIGNAL(clicked()), SLOT(slotCallMatch()));

	callAtt = new QAction(tr("Attenuator synthesis"), this);
	callAtt->setShortcut(Qt::CTRL+Qt::Key_6);
	callAtt->setStatusTip(tr("Starts SwieeAttenuator"));
	callAtt->setWhatsThis(
		tr("Attenuator synthesis\n\nStarts attenuator calculation program"));
	connect(callAtt, SIGNAL(activated()), SLOT(slotCallAtt()));
	connect(callAtt1, SIGNAL(clicked()), SLOT(slotCallAtt()));

	callRes = new QAction(tr("Resistor color codes"), this);
	callRes->setShortcut(Qt::CTRL+Qt::Key_7);
	callRes->setStatusTip(tr("Starts Swiee resistor color codes"));
	callRes->setWhatsThis(
		tr("Resistor color codes\n\nStarts standard resistor color code computation program"));
	connect(callRes, SIGNAL(activated()), SLOT(slotCallRes()));
	connect(callRes1, SIGNAL(clicked()), SLOT(slotCallRes()));

	simulate = new QAction(QIcon((":/bitmaps/gear.png")), tr("Simulate"), this);
	simulate->setShortcut(Qt::Key_F2);
	simulate->setStatusTip(tr("Simulates the current schematic"));
	simulate->setWhatsThis(tr("Simulate\n\nSimulates the current schematic"));
	connect(simulate, SIGNAL(activated()), SLOT(slotSimulate()));
	connect(simulate1, SIGNAL(clicked()), SLOT(slotSimulate()));

	dpl_sch = new QAction(QIcon((":/bitmaps/rebuild.png")), tr("View Data Display/Schematic"), this);
	dpl_sch->setShortcut(Qt::Key_F4);
	dpl_sch->setStatusTip(tr("Changes to data display or schematic page"));
	dpl_sch->setWhatsThis(
		tr("View Data Display/Schematic\n\n")+
		tr("Changes to data display or schematic page"));
	connect(dpl_sch, SIGNAL(activated()), SLOT(slotToPage()));
	connect(dpl_sch1, SIGNAL(clicked()), SLOT(slotToPage()));

	dcbias = new QAction(tr("Calculate DC bias"), this);
	dcbias->setShortcut(Qt::Key_F8);
	dcbias->setStatusTip(tr("Calculates DC bias and shows it"));
	dcbias->setWhatsThis(
		tr("Calculate DC bias\n\nCalculates DC bias and shows it"));
	connect(dcbias, SIGNAL(activated()), SLOT(slotDCbias()));
	connect(dcbias1, SIGNAL(clicked()), SLOT(slotDCbias()));

	setMarker = new QAction(QIcon((":/bitmaps/marker.png")),	tr("Set Marker on Graph"), this);
	setMarker->setShortcut(Qt::CTRL+Qt::Key_B);
	setMarker->setStatusTip(tr("Sets a marker on a diagram's graph"));
	setMarker->setWhatsThis(
		tr("Set Marker\n\nSets a marker on a diagram's graph"));
	setMarker->setToggleAction(true);
	setMarker1->setToggleButton(true);
	connect(setMarker, SIGNAL(toggled(bool)), SLOT(slotSetMarker(bool)));
	connect(setMarker1, SIGNAL(toggled(bool)), SLOT(slotSetMarker(bool)));

	showMsg = new QAction(tr("Show Last Messages"), this);
	showMsg->setShortcut(Qt::Key_F5);
	showMsg->setStatusTip(tr("Shows last simulation messages"));
	showMsg->setWhatsThis(
		tr("Show Last Messages\n\nShows the messages of the last simulation"));
	connect(showMsg, SIGNAL(activated()), SLOT(slotShowLastMsg()));
	connect(showMsg1, SIGNAL(clicked()), SLOT(slotShowLastMsg()));

	showNet = new QAction(tr("Show Last Netlist"), this);
	showNet->setShortcut(Qt::Key_F6);
	showNet->setStatusTip(tr("Shows last simulation netlist"));
	showNet->setWhatsThis(
		tr("Show Last Netlist\n\nShows the netlist of the last simulation"));
	connect(showNet, SIGNAL(activated()), SLOT(slotShowLastNetlist()));
	connect(showNet1, SIGNAL(clicked()), SLOT(slotShowLastNetlist()));

	viewToolBar = new QAction(tr("Tool&bar"), this);
	viewToolBar->setToggleAction(true);
	viewToolBar1->setToggleButton(true);
	viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
	viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
	connect(viewToolBar, SIGNAL(toggled(bool)), SLOT(slotViewToolBar(bool)));
	connect(viewToolBar1, SIGNAL(toggled(bool)), SLOT(slotViewToolBar(bool)));

	viewStatusBar = new QAction(tr("&Statusbar"), this);
	viewStatusBar->setToggleAction(true);
	viewStatusBar1->setToggleButton(true);
	viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
	viewStatusBar->setWhatsThis(
		tr("Statusbar\n\nEnables/disables the statusbar"));
	connect(viewStatusBar, SIGNAL(toggled(bool)), SLOT(slotViewStatusBar(bool)));
	connect(viewStatusBar1, SIGNAL(toggled(bool)), SLOT(slotViewStatusBar(bool)));

	viewBrowseDock = new QAction(tr("&Dock Window"), this);
	viewBrowseDock->setToggleAction(true);
	viewBrowseDock1->setToggleButton(true);
	viewBrowseDock->setStatusTip(tr("Enables/disables the browse dock window"));
	viewBrowseDock->setWhatsThis(
		tr("Browse Window\n\nEnables/disables the browse dock window"));
	connect(viewBrowseDock, SIGNAL(toggled(bool)), SLOT(slotViewBrowseDock(bool)));
	connect(viewBrowseDock1, SIGNAL(toggled(bool)), SLOT(slotViewBrowseDock(bool)));

	viewOctaveDock = new QAction(tr("&Octave Window"), this);
	viewOctaveDock->setToggleAction(true);
	viewOctaveDock1->setToggleButton(true);
	viewOctaveDock->setStatusTip(tr("Shows/hides the Octave dock window"));
	viewOctaveDock->setWhatsThis(
		tr("Octave Window\n\nShows/hides the Octave dock window"));
	connect(viewOctaveDock, SIGNAL(toggled(bool)), SLOT(slotViewOctaveDock(bool)));
	connect(viewOctaveDock1, SIGNAL(toggled(bool)), SLOT(slotViewOctaveDock(bool)));

	helpIndex = new QAction(tr("Help Index..."), this);
	helpIndex->setShortcut(Qt::Key_F1);
	helpIndex->setStatusTip(tr("Index of Swiee Help"));
	helpIndex->setWhatsThis(tr("Help Index\n\nIndex of intern Swiee help"));
	connect(helpIndex, SIGNAL(activated()), SLOT(slotHelpIndex()));
	connect(helpIndex1, SIGNAL(clicked()), SLOT(slotHelpIndex()));

	helpGetStart = new QAction(tr("Getting Started..."), this);
	helpGetStart->setStatusTip(tr("Getting Started with Swiee"));
	helpGetStart->setWhatsThis(
		tr("Getting Started\n\nShort introduction into Swiee"));
	connect(helpGetStart, SIGNAL(activated()), SLOT(slotGettingStarted()));
	connect(helpGetStart1, SIGNAL(clicked()), SLOT(slotGettingStarted()));

	helpAboutApp = new QAction(tr("&About Swiee..."), this);
	helpAboutApp->setStatusTip(tr("About the application"));
	helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
	connect(helpAboutApp, SIGNAL(activated()), SLOT(slotHelpAbout()));
	connect(helpAboutApp1, SIGNAL(clicked()), SLOT(slotHelpAbout()));

	helpAboutQt = new QAction(tr("About Qt..."), this);
	helpAboutQt->setStatusTip(tr("About Qt"));
	helpAboutQt->setWhatsThis(tr("About Qt\n\nAbout Qt by Trolltech"));
	connect(helpAboutQt, SIGNAL(activated()), SLOT(slotHelpAboutQt()));
	connect(helpAboutQt1, SIGNAL(clicked()), SLOT(slotHelpAboutQt()));
}

//让按钮变成window风格
void SwieeApp::setButStyle(const QString ButName,QSize butSize,QToolButton *ToolBut)
{
	ToolBut->setText(ButName);
	ToolBut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	ToolBut->setFixedSize(butSize);
	ToolBut->setIconSize(butSize);
	ToolBut->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	ToolBut->setStyleSheet("QToolButton{border-image: url(:/bitmaps/picNormal.png);}"
				"QToolButton:hover{border-image: url(:/bitmaps/picdn.png);}"
				"QToolButton:pressed{border-image: url(:/bitmaps/picPush.png);}");
}

//让菜单按钮的属性
void SwieeApp::setButTrr(QToolButton *ToolBut,
						const QKeySequence &key,
						const QString &astatusTip,
						const QString &awhatsThis)
{
	ToolBut->setShortcut(key);
	ToolBut->setToolTip(astatusTip);
	ToolBut->setStatusTip(astatusTip);	
	ToolBut->setWhatsThis(awhatsThis);

}

//让菜单按钮的属性和风格
void SwieeApp::setButPropts(QToolButton *ToolBut,
						QSize butSize,
						const QKeySequence &key,
						const QString ButName,						
						const QString &astatusTip,
						const QString &awhatsThis)
{
	setButStyle(ButName,butSize,ToolBut);
	setButTrr(ToolBut,key,astatusTip,awhatsThis);
}


void SwieeApp::initQQTabDock()
{


	QGroupBox *groupBox1 = new QGroupBox();


	QVBoxLayout *layout1 = new QVBoxLayout(groupBox1);
	layout1->setMargin(0);
	layout1->setAlignment(Qt::AlignHCenter);    
	layout1->addWidget(toolButton1_1);
	layout1->addWidget(toolButton1_2);
	layout1->addWidget(toolButton1_3);
	layout1->addWidget(toolButton1_4);
	layout1->addWidget(toolButton1_5);  
	layout1->addStretch();        

	QGroupBox *groupBox2 = new QGroupBox();

	toolButton2_1 = new QToolButton( );
	toolButton2_1->setText( tr( "pizicai" ) );
	toolButton2_1->setIcon( QPixmap( ":/images/1.png" ) );
	toolButton2_1->setIconSize( QPixmap( ":/images/1.png").size() );
	toolButton2_1->setAutoRaise( TRUE );
	toolButton2_1->setToolButtonStyle( Qt::ToolButtonTextUnderIcon);

	toolButton2_2 = new QToolButton();
	toolButton2_2->setText( tr( "feiyang" ) );
	toolButton2_2->setIcon( QPixmap( ":/images/2.png") );
	toolButton2_2->setIconSize( QPixmap( ":/images/2.png").size() );
	toolButton2_2->setAutoRaise( TRUE );
	toolButton2_2->setToolButtonStyle( Qt::ToolButtonTextUnderIcon);

	QVBoxLayout *layout2 = new QVBoxLayout(groupBox2);
	layout2->setMargin(0);
	layout2->setAlignment(Qt::AlignHCenter);    
	layout2->addWidget(toolButton2_1);
	layout2->addWidget(toolButton2_2);
	layout2->addStretch();       

	QGroupBox *groupBox3 = new QGroupBox();

	toolButton3_1 = new QToolButton( );
	toolButton3_1->setText( tr( "QQ" ) );
	toolButton3_1->setIcon( QPixmap( ":/images/3.png" ) );
	toolButton3_1->setIconSize( QPixmap( ":/images/3.png").size() );
	toolButton3_1->setAutoRaise( TRUE );
	toolButton3_1->setToolButtonStyle( Qt::ToolButtonTextUnderIcon);

	toolButton3_2 = new QToolButton();
	toolButton3_2->setText( tr( "angel" ) );
	toolButton3_2->setIcon( QPixmap( ":/images/4.png") );
	toolButton3_2->setIconSize( QPixmap( ":/images/4.png").size() );
	toolButton3_2->setAutoRaise( TRUE );
	toolButton3_2->setToolButtonStyle( Qt::ToolButtonTextUnderIcon);

	QVBoxLayout *layout3 = new QVBoxLayout(groupBox3);
	layout3->setMargin(0);
	layout3->setAlignment(Qt::AlignHCenter);    
	layout3->addWidget(toolButton3_1);
	layout3->addWidget(toolButton3_2);     
	layout3->addStretch();         

	QQTabDock = new QDockWidget(this);
	QQTabDock->setAllowedAreas(Qt::RightDockWidgetArea); //
	//把dock 放右边
	this->addDockWidget(Qt::RightDockWidgetArea, QQTabDock);
	QToolBox *test = new  QToolBox(QQTabDock);
	QQTabDock->setWidget(test);

	

	test->addItem( ( QWidget* )groupBox1 , tr("my friends" ));
	test->addItem( ( QWidget* )groupBox2 , tr("stranger" ));
	test->addItem( ( QWidget* )groupBox3 , tr("blacklist" ));

	test->setMargin(0);

	/* spacing between items in the tool bar */
	//test->setStyleSheet("  QToolBar {background: red;spacing: 3px;}");
	

	//// 加载CSS文件
	QString strPath = QCoreApplication::applicationDirPath();  
	qDebug()<<strPath;  
	QString strCssFile = strPath + "/QQ.css";  
	QFile fCss(strCssFile);  
	if( !fCss.open(QFile::ReadOnly))  
	{  
		qDebug("css File %s load false",strCssFile);  
	//        return;  
	}  
	QString strCssContent(fCss.readAll());  
	QQTabDock->setStyleSheet(strCssContent);  
	fCss.close(); 

}

void SwieeApp::initProptsDock()
{		
	propertyDock = new QDockWidget(this);
	propertyDock->setAllowedAreas(Qt::RightDockWidgetArea); //
	//把dock 放右边
	this->addDockWidget(Qt::RightDockWidgetArea, propertyDock);
	propertyDock->setWindowTitle(GB2312("属性"));
	
	propertyDock->setStyleSheet("background-color: rgb(240, 240, 240);");
	QWidget *dockWidgetContents = new QWidget();
       
	
	
	all = new Q3VBoxLayout(dockWidgetContents);
	   propertyDock->setWidget(dockWidgetContents);
	propertyDock->setLayout(all);


	Q3GridLayout *gp1;
	QWidget *myParent = propertyDock;
	ValInteger = new QIntValidator(1, 1000000, this);

	Expr.setPattern("[^\"=]*");  // valid expression for property 'edit'
	Validator = new QRegExpValidator(Expr, this);
	Expr.setPattern("[^\"]*");   // valid expression for property 'edit'
	Validator2 = new QRegExpValidator(Expr, this);
	Expr.setPattern("[\\w_]+");  // valid expression for property 'NameEdit'
	ValRestrict = new QRegExpValidator(Expr, this);

	//checkSim  = 0;  editSim  = 0;  comboType  = 0;  checkParam = 0;
	//editStart = 0;  editStop = 0;  editNumber = 0;

	//Property *pp = 0;   // last property not to put in ListView
	
	
	// no simulation component
	gp1 = new Q3GridLayout(0, 9,2,5,5);
    all->addLayout(gp1);

	// ...........................................................
	//gp1->addMultiCellWidget(new QLabel(Comp->Description, myParent), 0,0,0,1);
	gp1->addMultiCellWidget(new QLabel(GB2312("属性"), myParent), 0,0,0,1);

	Q3HBox *h5 = new Q3HBox(myParent);
	h5->setSpacing(5);
	gp1->addWidget(h5, 1,0);
	//new QLabel(tr("Name:"), h5);
	//CompNameEdit = new QLineEdit(h5);
	//CompNameEdit->setValidator(ValRestrict);
	//connect(CompNameEdit, SIGNAL(returnPressed()), SLOT(slotButtOK()));

	//showName = new QCheckBox(tr("display in schematic"), myParent);
	//gp1->addWidget(showName, 1,1);
	
	Q3HGroupBox *PropertyBox = new Q3HGroupBox(tr("Properties"), myParent);
	gp1->addMultiCellWidget(PropertyBox, 2,2,0,1);
	//开始增加属性列表

	PropTabWdg = new QTableWidget(PropertyBox);
	
	PropTabWdg->setColumnCount(4);//设置列数
	//PropTabWdg->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
	
	
	//设置表头内容
	QStringList header;
	header<<GB2312("显示")<<GB2312("名称")<<GB2312("值")<<GB2312("描述");
	PropTabWdg->setHorizontalHeaderLabels(header);
	PropTabWdg->horizontalHeader()->setStretchLastSection(true); //就是这个地方 
	//PropTabWdg->horizontalHeader()->setResizeMode(QHeaderView::Stretch); //水平表头自适应填充
	//PropTabWdg->verticalHeader()->setResizeMode(QHeaderView::Stretch);//垂直表头自适应填充
	PropTabWdg->verticalHeader()->setVisible(false);//不显示垂直表头


	//设置列宽行高
	PropTabWdg->setColumnWidth(0,35);//设置第一列的列宽
	PropTabWdg->setColumnWidth(1,80);//设置第二列的列宽
	PropTabWdg->setColumnWidth(2,80);//设置第三列的列宽
	//PropTabWdg->setColumnWidth(2,40);//设置第三列的列宽

	//PropTabWdg->setRowHeight()
	PropTabWdg->verticalHeader()->setDefaultSectionSize(20);

	//设置表头字体加粗
	QFont font = PropTabWdg->horizontalHeader()->font();
	//font.setBold(true);
	PropTabWdg->horizontalHeader()->setFont(font);

	//PropTabWdg->insertRow(0); //插入新行
	//QTableWidgetItem *item = new QTableWidgetItem();
	//QTableWidgetItem *item1 = new QTableWidgetItem();
	//QTableWidgetItem *item2 = new QTableWidgetItem();
	//QTableWidgetItem *item3 = new QTableWidgetItem();

	//connect(PropTabWdg, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));
	//connect(PropTabWdg, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(getCurrentItem(QTableWidgetItem*)));

	connect(PropTabWdg, SIGNAL(cellChanged(int,int)), this, SLOT(slotPropCellChanged(int, int)));
	//连接属性框单元格更改的信号的slot
 

}
void SwieeApp::getCurrentItem(QTableWidgetItem* item)
{
	QString s;
	Component *pc = (Component*)view->focusElement;
}

void SwieeApp::slotPropCellChanged(int row, int col)
{
	if(!ProptShowDone)
	{
		return;
	}	
	
	QString s;
	Component *pc = (Component*)view->focusElement;

	if(!pc)
	{
		return;	
	}
	if(pc->Type & isComponent) {
		Component *Comp = pc;
		int row_count = 0; //获取表单行数

		/*********************************************************/
		//单独更新界面中元件名称
		/*********************************************************/
		if(PropTabWdg->item(row_count,0)->checkState() ==  Qt::Checked)
		{
			Comp->showName = true;
		}
		else
		{
			Comp->showName = false;
		}


		//检查新的名字是不是有重名的元件
		QString newName;
		Component *pc1;

		SwieeDoc *d1 = getDoc();
		Schematic *sc1 = (Schematic *)d1;
		newName = PropTabWdg->item(row_count,2)->text();
		if(newName != Comp->Name) 
		{
			for(pc1 = sc1->Components->first(); pc1!=0; pc1 = sc1->Components->next())
			{
				if(pc1->Name == newName)
						break;  // found component with the same name ?
			}
			if(!pc1)   
			{
				Comp->Name = newName;
			}
			
		}	
		row_count++;

		/*********************************************************/
		//从属性框中，更改元件实际的属性链表
		/*********************************************************/
		for(Property *p = Comp->Props.last(); p != 0; p = Comp->Props.prev()) 
		{
			//if(p == pp)  break;   // do not insert if already on first tab
			//设置对应的图标、文件名称、最后更新时间、对应的类型、文件大小 

			
			if(PropTabWdg->item(row_count,0)->checkState() ==  Qt::Checked)
			{
				p->display = true;
			}
			else
			{
				p->display = false;
			}


			p->Name = PropTabWdg->item(row_count,1)->text();
			p->Value = PropTabWdg->item(row_count,2)->text();
			p->Description = PropTabWdg->item(row_count,3)->text();

			row_count++;
		}
		SwieeDoc *d = getDoc();
		updateProptsDock((Schematic *)d,pc);
		updateNavigateDock((Schematic *)d);
		((Schematic *)d)->viewport()->update();
	}


}
void SwieeApp::updateProptsDock(Schematic *Doc,Element *Elem)
{	
	QString s;
	//PropTabWdg->clear();

	ProptShowDone = false;
	int iLen = PropTabWdg->rowCount();
	for(int i=0;i<iLen;i++)    
	{        
		PropTabWdg->removeRow(0);           
	}

	int rowID=0;
	if(!Elem)
	{
		return;
	}
	if(Elem->Type & isComponent) {
		Component *Comp = (Component*)Elem;
		/*********************************************************/
		//单独添加元件的名字，因为元件的名字不在元件的属性链表中
		/*********************************************************/

		int row_count = PropTabWdg->rowCount(); //获取表单行数
		PropTabWdg->insertRow(row_count); //插入新行
		QTableWidgetItem *item = new QTableWidgetItem();
		QTableWidgetItem *itemkong = new QTableWidgetItem();
		QTableWidgetItem *item1 = new QTableWidgetItem();
		QTableWidgetItem *item3 = new QTableWidgetItem();
		
		item->setText(GB2312("名称"));
		item1->setText(Comp->Name);
		item3->setText(GB2312("元件的名称"));

		PropTabWdg->setItem(row_count, 0, itemkong);//第一为勾选框，没有文字
		if(Comp->showName) 
		{
			PropTabWdg->item(row_count,0)->setCheckState(Qt::Checked);
		}
		else
		{
			PropTabWdg->item(row_count,0)->setCheckState(Qt::Unchecked);
		}

		PropTabWdg->setItem(row_count, 1, item);
		PropTabWdg->item(row_count, 1)->setFlags(Qt::ItemIsEnabled);
		PropTabWdg->setItem(row_count, 2, item1); 
		PropTabWdg->setItem(row_count, 3, item3);
		PropTabWdg->item(row_count, 3)->setFlags(Qt::ItemIsEnabled);

		/*********************************************************/
		//添加元件的属性
		/*********************************************************/
		for(Property *p = Comp->Props.last(); p != 0; p = Comp->Props.prev()) {
			//if(p == pp)  break;   // do not insert if already on first tab
			

			int row_count = PropTabWdg->rowCount(); //获取表单行数
			PropTabWdg->insertRow(row_count); //插入新行
			QTableWidgetItem *item = new QTableWidgetItem();
			QTableWidgetItem *itemkong = new QTableWidgetItem();
			QTableWidgetItem *item1 = new QTableWidgetItem();
			QTableWidgetItem *item3 = new QTableWidgetItem();

			//设置每一列的文字
 
			item->setText(p->Name);
			item1->setText(p->Value);
			item3->setText(p->Description);

			PropTabWdg->setItem(row_count, 0, itemkong);
			if(p->display) 
			{
				PropTabWdg->item(row_count,0)->setCheckState(Qt::Checked);
			}
			else
			{
				PropTabWdg->item(row_count,0)->setCheckState(Qt::Unchecked);
			}

			PropTabWdg->setItem(row_count, 1, item);
			PropTabWdg->item(row_count, 1)->setFlags(Qt::ItemIsEnabled);
			PropTabWdg->setItem(row_count, 2, item1); 
			PropTabWdg->setItem(row_count, 3, item3);
			PropTabWdg->item(row_count, 3)->setFlags(Qt::ItemIsEnabled);
		}
	}
	ProptShowDone = true;
	
}

void SwieeApp::initNavigateDock(){
	navigateDock = new QDockWidget(this);
	navigateDock->setAllowedAreas(Qt::RightDockWidgetArea); //
	//把dock 放右边
	this->addDockWidget(Qt::RightDockWidgetArea, navigateDock);
	navigateDock->setWindowTitle(GB2312("导航窗口"));
	
	navigateDock->setStyleSheet("background-color: rgb(240, 240, 240);");
	
	
    
	NaviTreeWidget =new QTreeWidget(navigateDock);

	   //设定头项名称  
    NaviTreeWidget->setHeaderLabels(QStringList()<<"Key"<<"Value");  
 
	
	////设定各个项  
 //   QTreeWidgetItem* A = new QTreeWidgetItem(QStringList()<<"A");  
 // 
 //   A->setCheckState(0, Qt::Checked);  
 //   QTreeWidgetItem* B = new QTreeWidgetItem(QStringList()<<"B");  
 //
 //   B->setCheckState(0, Qt::Checked);  
 //   QTreeWidgetItem* C = new QTreeWidgetItem(QStringList()<<"C");  
	// C->setCheckState(0, Qt::Checked);  

 //   NaviTreeWidget->addTopLevelItem(A);  
 //   NaviTreeWidget->addTopLevelItem(B);  
 //   NaviTreeWidget->addTopLevelItem(C);  

	QTreeWidgetItem* CompRoot = new QTreeWidgetItem(QStringList()<<GB2312("元件"));  	
	NaviTreeWidget->addTopLevelItem(CompRoot);


	navigateDock->setWidget(NaviTreeWidget);
	navigateDock->setLayout(all);

	connect(NaviTreeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
		this,SLOT(slotShowSelectedItem(QTreeWidgetItem*,int)));//单击节点事件
}
//一定要加类名字啊
void SwieeApp::slotShowSelectedItem(QTreeWidgetItem* treeW,int index)
{
	


	QString CompName;
	CompName = treeW->text(0);

	Component *pc1;

	SwieeDoc *d1 = getDoc();
	Schematic *sc1 = (Schematic *)d1;

	Element *curFocusElement;

	sc1->showAll();
	sc1->viewport()->update();


	for(pc1 = sc1->Components->first(); pc1!=0; pc1 = sc1->Components->next())
	{
		if(pc1->Name == CompName)
		{
			curFocusElement = sc1->selectElement(pc1->cx,pc1->cy,false);
			curFocusElement->isSelected = true;

			sc1->deselectElements(curFocusElement);
			
			sc1->showNoZoom();
			

			int vx,vy;
			vx = sc1->visibleWidth();
			vy = sc1->visibleHeight();

			float xScale = float(vx) / float(500.0);
			float yScale = float(vy) / float(500.0);
			if(xScale > yScale) xScale = yScale;
			xScale /= sc1->Scale;

			sc1->ViewX1 = pc1->cx - sc1->visibleWidth()/2;
			sc1->ViewY1 = pc1->cy - sc1->visibleHeight()/2;
			sc1->ViewX2 = pc1->cx + sc1->visibleWidth()/2;
			sc1->ViewY2 = pc1->cy + sc1->visibleHeight()/2;


			sc1->zoom(xScale);

			sc1->viewport()->update();
			break;  // found component with the same name ?
		}
			
	}
}
void SwieeApp::updateNavigateDock(Schematic *Doc){
	//QList<QTreeWidgetItem*> temp;
	//temp = NaviTreeWidget->findItems(GB2312("元件"),Qt::MatchContains|Qt::MatchRecursive);
	//
	//NaviTreeWidget->removeItemWidget(NaviTreeWidget->findItems(GB2312("元件"),Qt::MatchContains|Qt::MatchRecursive).at(0),0);
	NaviTreeWidget->clear();
	//QTreeWidgetItem* CompRoot = new QTreeWidgetItem(QStringList()<<GB2312("元件"));  	
	//NaviTreeWidget->addTopLevelItem(CompRoot);

	//QTreeWidgetItem* CompRoot1 = new QTreeWidgetItem(QStringList()<<GB2312("电容"));  	
	//NaviTreeWidget->addTopLevelItem(CompRoot1);

	//WYL 测试如何查找树	
	//for(int m=0;m< NaviTreeWidget->topLevelItemCount(); m++)                         //遍历所有父节点     
	//{          
	//	QTreeWidgetItem* CurTopLevItm = NaviTreeWidget->topLevelItem(m); //显示与关键字匹配的馈线         		
	//	QString TopNodText=CurTopLevItm->text(0);
	//	
	//	if (!TopNodText.compare(GB2312("元件")) )
	//	{
	//		CurTopLevItm->setBackgroundColor(0,QColor(255,0,0));
	//	} 
	//	else
	//	{
	//	}
	//
	//}

	
	// determines the name by looking for names with the same
	// prefix and increment the number
	for(Component *pc = Doc->Components->first(); pc != 0; pc = Doc->Components->next())
	{

        QTreeWidgetItem *child; 



		//pc->Description;
		QList<QTreeWidgetItem *> temp;
		temp = NaviTreeWidget->findItems(pc->Description,Qt::MatchExactly);

		if(temp.length() == 0)
		{
			QTreeWidgetItem* CompRoot = new QTreeWidgetItem(QStringList()<<pc->Description);  	
			NaviTreeWidget->addTopLevelItem(CompRoot);
			child = new QTreeWidgetItem(QStringList()<<pc->Name);
			CompRoot->addChild(child);
		}
		else
		{
			child = new QTreeWidgetItem(QStringList()<<pc->Name);
			temp[0]->addChild(child);	
		}

		/* child = new QTreeWidgetItem(QStringList()<<pc->Name);  
		CompRoot->addChild(child);*/  
	}
	 
	NaviTreeWidget->expandAll();
}






/**
* @brief SwieeApp::initRibbon Initializes all Ribbon QToolButton of the application
*/
void SwieeApp::initRibbon()
{
	QSize butSize = QSize(48,70);
	
	fileNew1 = new QToolButton();
	textNew1 = new QToolButton();
	fileOpen1 = new QToolButton();
	fileClose1 = new QToolButton();
	recentfilesMenu1 = new QToolButton();
	fileSave1 = new QToolButton();
	fileSaveAll1 = new QToolButton();
	fileSaveAs1 = new QToolButton();
	exportAsImage1 = new QToolButton();
	filePrint1 = new QToolButton();
	filePrintFit1 = new QToolButton();
	fileExamples1 = new QToolButton();
	fileSettings1 = new QToolButton();
	symEdit1 = new QToolButton();
	applSettings1 = new QToolButton();
	refreshSchPath1 = new QToolButton();
	fileQuit1 = new QToolButton();

/*************************************************************/
// file menu 添加按钮 
/*************************************************************/


	setButPropts(fileNew1,butSize,Qt::CTRL+Qt::Key_N,tr("fileNew"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(textNew1,butSize,Qt::CTRL+Qt::Key_N,tr("textNew1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileOpen1,butSize,Qt::CTRL+Qt::Key_N,tr("fileOpen1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileClose1,butSize,Qt::CTRL+Qt::Key_N,tr("fileClose1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(recentfilesMenu1,butSize,Qt::CTRL+Qt::Key_N,tr("recentfilesMenu1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

	setButPropts(fileSave1,butSize,Qt::CTRL+Qt::Key_N,tr("fileSave1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileSaveAll1,butSize,Qt::CTRL+Qt::Key_N,tr("fileSaveAll1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileSaveAs1,butSize,Qt::CTRL+Qt::Key_N,tr("fileSaveAs1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(exportAsImage1,butSize,Qt::CTRL+Qt::Key_N,tr("exportAsImage1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(filePrint1,butSize,Qt::CTRL+Qt::Key_N,tr("filePrint1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(filePrintFit1,butSize,Qt::CTRL+Qt::Key_N,tr("filePrintFit1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

	setButPropts(fileExamples1,butSize,Qt::CTRL+Qt::Key_N,tr("fileExamples1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileSettings1,butSize,Qt::CTRL+Qt::Key_N,tr("fileSettings1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(symEdit1,butSize,Qt::CTRL+Qt::Key_N,tr("symEdit1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(applSettings1,butSize,Qt::CTRL+Qt::Key_N,tr("applSettings1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));	
	setButPropts(refreshSchPath1,butSize,Qt::CTRL+Qt::Key_N,tr("refreshSchPath1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(fileQuit1,butSize,Qt::CTRL+Qt::Key_N,tr("fileQuit1"),tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
				



/*************************************************************/
// edit menu 添加按钮 
/*************************************************************/
	undo1 = new QToolButton();
	redo1 = new QToolButton();
	editCut1 = new QToolButton();
	editCopy1 = new QToolButton();
	editPaste1 = new QToolButton();
	editDelete1 = new QToolButton();

	select1 = new QToolButton();
	selectAll1 = new QToolButton();
	selectMarker1 = new QToolButton();
	editFind1 = new QToolButton();
	editFindAgain1 = new QToolButton();
	changeProps1 = new QToolButton();//replace

	editRotate1 = new QToolButton();
	editMirror1 = new QToolButton();
	editMirrorY1 = new QToolButton();
	editActivate1 = new QToolButton();
	intoH1 = new QToolButton();
	popH1 = new QToolButton();

	setButPropts(undo1,butSize,Qt::CTRL+Qt::Key_N,"undo1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(redo1,butSize,Qt::CTRL+Qt::Key_N,"redo1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editCut1,butSize,Qt::CTRL+Qt::Key_N,"editCut1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editCopy1,butSize,Qt::CTRL+Qt::Key_N,"editCopy1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editPaste1,butSize,Qt::CTRL+Qt::Key_N,"editPaste1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editDelete1,butSize,Qt::CTRL+Qt::Key_N,"editDelete1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(select1,butSize,Qt::CTRL+Qt::Key_N,"select1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	
	setButPropts(selectAll1,butSize,Qt::CTRL+Qt::Key_N,"selectAll1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(selectMarker1,butSize,Qt::CTRL+Qt::Key_N,"selectMarker1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editFind1,butSize,Qt::CTRL+Qt::Key_N,"editFind1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editFindAgain1,butSize,Qt::CTRL+Qt::Key_N,"editFindAgain1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(changeProps1,butSize,Qt::CTRL+Qt::Key_N,"changeProps1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editRotate1,butSize,Qt::CTRL+Qt::Key_N,"editRotate1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editMirror1,butSize,Qt::CTRL+Qt::Key_N,"editMirror1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));	
	setButPropts(editMirrorY1,butSize,Qt::CTRL+Qt::Key_N,"editMirrorY1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(editActivate1,butSize,Qt::CTRL+Qt::Key_N,"editActivate1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(intoH1,butSize,Qt::CTRL+Qt::Key_N,"editActivate1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(popH1,butSize,Qt::CTRL+Qt::Key_N,"editActivate1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));


	


/*************************************************************/
// position menu 添加按钮 10个
/*************************************************************/
	moveText1 = new QToolButton();
	onGrid1 = new QToolButton();
	centerHor1 = new QToolButton();
	centerVert1 = new QToolButton();
	alignTop1 = new QToolButton();
	alignBottom1 = new QToolButton();

	alignLeft1 = new QToolButton();
	alignRight1 = new QToolButton();
	distrHor1 = new QToolButton();
	distrVert1 = new QToolButton();

	setButPropts(moveText1,butSize,Qt::CTRL+Qt::Key_N,"moveText1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(onGrid1,butSize,Qt::CTRL+Qt::Key_N,"onGrid1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(centerHor1,butSize,Qt::CTRL+Qt::Key_N,"centerHor1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(centerVert1,butSize,Qt::CTRL+Qt::Key_N,"centerVert1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(alignTop1,butSize,Qt::CTRL+Qt::Key_N,"alignTop1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(alignBottom1,butSize,Qt::CTRL+Qt::Key_N,"alignBottom1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(alignLeft1,butSize,Qt::CTRL+Qt::Key_N,"alignLeft1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	
	setButPropts(alignRight1,butSize,Qt::CTRL+Qt::Key_N,"alignRight1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(distrHor1,butSize,Qt::CTRL+Qt::Key_N,"distrHor1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(distrVert1,butSize,Qt::CTRL+Qt::Key_N,"distrVert1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	


/*************************************************************/
// insert menu 添加按钮 7个
/*************************************************************/
	insWire1 = new QToolButton();
	insLabel1 = new QToolButton();
	insEquation1 = new QToolButton();
	insGround1 = new QToolButton();
	insPort1 = new QToolButton();
	setMarker1 = new QToolButton();
	insEntity1 = new QToolButton();

	setButPropts(insWire1,butSize,Qt::CTRL+Qt::Key_N,"insWire1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(insLabel1,butSize,Qt::CTRL+Qt::Key_N,"insLabel1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(insEquation1,butSize,Qt::CTRL+Qt::Key_N,"insEquation1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(insGround1,butSize,Qt::CTRL+Qt::Key_N,"insGround1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(insPort1,butSize,Qt::CTRL+Qt::Key_N,"insPort1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(setMarker1,butSize,Qt::CTRL+Qt::Key_N,"setMarker1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(insEntity1,butSize,Qt::CTRL+Qt::Key_N,"insEntity1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));


/*************************************************************/
// project menu 添加按钮 12个
/*************************************************************/
	projNew1 = new QToolButton();
	projOpen1 = new QToolButton();
	addToProj1 = new QToolButton();
	projDel1 = new QToolButton();
	projClose1 = new QToolButton();
	createLib1 = new QToolButton();

	createPkg1 = new QToolButton();
	extractPkg1 = new QToolButton();
	importData1 = new QToolButton();
	graph2csv1 = new QToolButton();
	buildModule1 = new QToolButton();
	loadModule1 = new QToolButton();

	setButPropts(projNew1,butSize,Qt::CTRL+Qt::Key_N,"projNew1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(projOpen1,butSize,Qt::CTRL+Qt::Key_N,"projOpen1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(addToProj1,butSize,Qt::CTRL+Qt::Key_N,"addToProj1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(projDel1,butSize,Qt::CTRL+Qt::Key_N,"projDel1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(projClose1,butSize,Qt::CTRL+Qt::Key_N,"projClose1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(createLib1,butSize,Qt::CTRL+Qt::Key_N,"createLib1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(createPkg1,butSize,Qt::CTRL+Qt::Key_N,"createPkg1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

	setButPropts(extractPkg1,butSize,Qt::CTRL+Qt::Key_N,"extractPkg1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(importData1,butSize,Qt::CTRL+Qt::Key_N,"importData1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(graph2csv1,butSize,Qt::CTRL+Qt::Key_N,"graph2csv1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(buildModule1,butSize,Qt::CTRL+Qt::Key_N,"buildModule1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(loadModule1,butSize,Qt::CTRL+Qt::Key_N,"loadModule1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

/*************************************************************/
// tools menu 添加按钮 
/*************************************************************/
	callEditor1 = new QToolButton();
	callFilter1 = new QToolButton();
	callLine1 = new QToolButton();
	callLib1 = new QToolButton();
	callMatch1 = new QToolButton();
	callAtt1 = new QToolButton();
	callRes1 = new QToolButton();

	setButPropts(callEditor1,butSize,Qt::CTRL+Qt::Key_N,"callEditor1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callFilter1,butSize,Qt::CTRL+Qt::Key_N,"callFilter1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callLine1,butSize,Qt::CTRL+Qt::Key_N,"callLine1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callLib1,butSize,Qt::CTRL+Qt::Key_N,"callLib1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callMatch1,butSize,Qt::CTRL+Qt::Key_N,"callMatch1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callAtt1,butSize,Qt::CTRL+Qt::Key_N,"callAtt1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(callRes1,butSize,Qt::CTRL+Qt::Key_N,"callRes1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	
/*************************************************************/
// simulation menu 添加按钮 
/*************************************************************/
	simulate1 = new QToolButton();
	dpl_sch1 = new QToolButton();
	dcbias1 = new QToolButton();
	showMsg1 = new QToolButton();
	showNet1 = new QToolButton();

	setButPropts(simulate1,butSize,Qt::CTRL+Qt::Key_N,"simulate1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(dpl_sch1,butSize,Qt::CTRL+Qt::Key_N,"dpl_sch1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(dcbias1,butSize,Qt::CTRL+Qt::Key_N,"dcbias1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(showMsg1,butSize,Qt::CTRL+Qt::Key_N,"showMsg1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(showNet1,butSize,Qt::CTRL+Qt::Key_N,"showNet1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

/*************************************************************/
// view menu 添加按钮   8个
/*************************************************************/
	magAll1 = new QToolButton();
	magOne1 = new QToolButton();
	magPlus1 = new QToolButton();
	magMinus1 = new QToolButton();
	viewToolBar1 = new QToolButton();
	viewStatusBar1 = new QToolButton();
	viewBrowseDock1 = new QToolButton();
	viewOctaveDock1 = new QToolButton();

	setButPropts(magAll1,butSize,Qt::CTRL+Qt::Key_N,"magAll1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(magOne1,butSize,Qt::CTRL+Qt::Key_N,"magOne1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(magPlus1,butSize,Qt::CTRL+Qt::Key_N,"magPlus1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(magMinus1,butSize,Qt::CTRL+Qt::Key_N,"magMinus1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(viewToolBar1,butSize,Qt::CTRL+Qt::Key_N,"viewToolBar1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(viewStatusBar1,butSize,Qt::CTRL+Qt::Key_N,"viewStatusBar1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(viewBrowseDock1,butSize,Qt::CTRL+Qt::Key_N,"viewBrowseDock1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(viewOctaveDock1,butSize,Qt::CTRL+Qt::Key_N,"viewOctaveDock1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
/*************************************************************/
// help menu 添加按钮 
/*************************************************************/
	helpIndex1 = new QToolButton();
	helpGetStart1 = new QToolButton();
	helpAboutApp1 = new QToolButton();
	helpAboutQt1 = new QToolButton();

	setButPropts(helpIndex1,butSize,Qt::CTRL+Qt::Key_N,"helpIndex1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(helpGetStart1,butSize,Qt::CTRL+Qt::Key_N,"helpGetStart1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(helpAboutApp1,butSize,Qt::CTRL+Qt::Key_N,"helpAboutApp1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));
	setButPropts(helpAboutQt1,butSize,Qt::CTRL+Qt::Key_N,"helpAboutQt1",tr("Creates a new document"),tr("New\n\nCreates a new schematic or data display document"));

}



void SwieeApp::initMenuBarRibbon()
{
	/*********************************************************************/
	//  创建菜单栏选项卡和dock
	//QDockWidget     *menuDock;//菜单停靠栏
	//QDockWidget     *propertyDock;//属性导航停靠栏
	/*********************************************************************/
	menuDock = new QDockWidget(this);
	menuDock->setAllowedAreas(Qt::TopDockWidgetArea); //
	menuTabView = new QTabWidget(menuDock);
	menuDock->setWidget(menuTabView);

	menuTabView->setTabPosition(QTabWidget::North);
	menuTabView->setFont(QFont("SimSun",9));


	//把dock 放上边
	this->addDockWidget(Qt::TopDockWidgetArea, menuDock);
	//去掉menuDock的边框
	menuDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	QWidget *wu = new QWidget;
	menuDock->setTitleBarWidget(wu);	

				QHBoxLayout *FileMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				FileMenuButtsLayout->addWidget(fileNew1);
				FileMenuButtsLayout->addWidget(textNew1);
				FileMenuButtsLayout->addWidget(fileOpen1);
				FileMenuButtsLayout->addWidget(fileClose1);
				FileMenuButtsLayout->addWidget(fileSave1);
				FileMenuButtsLayout->addWidget(fileSaveAll1);
				FileMenuButtsLayout->addWidget(fileSaveAs1);
				FileMenuButtsLayout->addWidget(filePrint1);
				FileMenuButtsLayout->addWidget(filePrintFit1);
				FileMenuButtsLayout->addWidget(fileExamples1);
				FileMenuButtsLayout->addWidget(applSettings1);
				FileMenuButtsLayout->addWidget(refreshSchPath1);
				FileMenuButtsLayout->addWidget(fileQuit1);

				FileMenuButtsLayout->setSpacing(0);
				FileMenuButtsLayout->addStretch();
				FileMenuButtsLayout->setAlignment(Qt::AlignLeft);
				FileMenuButtsLayout->setContentsMargins(0,0,0,0);
			//QWidget *FileMenuButts = new QWidget();//按钮们
			//FileMenuButts->setLayout(FileMenuButtsLayout);//把这个layout放进去butts的widget中

	
	
			////menuList = new QListWidget();
			//
			////水平box布局
			//QVBoxLayout *FileMenuLayout = new QVBoxLayout();
			//FileMenuLayout->addWidget(FileMenuButts);
			////FileMenuLayout->addWidget(menuList);
			//FileMenuLayout->setSpacing(0);

				/*************************************************************/
				// edit menu 添加按钮 10个
				/*************************************************************/

				QHBoxLayout *EditMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				EditMenuButtsLayout->addWidget(undo1);
				EditMenuButtsLayout->addWidget(redo1);
				EditMenuButtsLayout->addWidget(editCut1);
				EditMenuButtsLayout->addWidget(editCopy1);
				EditMenuButtsLayout->addWidget(editPaste1);
				EditMenuButtsLayout->addWidget(editDelete1);
				EditMenuButtsLayout->addWidget(select1);
				EditMenuButtsLayout->addWidget(selectAll1);
				EditMenuButtsLayout->addWidget(selectMarker1);
				EditMenuButtsLayout->addWidget(editFind1);
				EditMenuButtsLayout->addWidget(editFindAgain1);
				EditMenuButtsLayout->addWidget(changeProps1);
				EditMenuButtsLayout->addWidget(editRotate1);
				EditMenuButtsLayout->addWidget(editMirror1);
				EditMenuButtsLayout->addWidget(editMirrorY1);
				EditMenuButtsLayout->addWidget(editActivate1);
				EditMenuButtsLayout->addWidget(intoH1);
				EditMenuButtsLayout->addWidget(popH1);


				EditMenuButtsLayout->setSpacing(0);
				EditMenuButtsLayout->addStretch();
				EditMenuButtsLayout->setAlignment(Qt::AlignLeft);
				EditMenuButtsLayout->setContentsMargins(0,0,0,0);

				/*************************************************************/
				// position menu 添加按钮 10个
				/*************************************************************/

				QHBoxLayout *PosiMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				PosiMenuButtsLayout->addWidget(moveText1);
				PosiMenuButtsLayout->addWidget(onGrid1);
				PosiMenuButtsLayout->addWidget(centerHor1);
				PosiMenuButtsLayout->addWidget(centerVert1);
				PosiMenuButtsLayout->addWidget(alignTop1);
				PosiMenuButtsLayout->addWidget(alignBottom1);
				PosiMenuButtsLayout->addWidget(alignLeft1);
				PosiMenuButtsLayout->addWidget(alignRight1);
				PosiMenuButtsLayout->addWidget(distrHor1);
				PosiMenuButtsLayout->addWidget(distrVert1);

				PosiMenuButtsLayout->setSpacing(0);
				PosiMenuButtsLayout->addStretch();
				PosiMenuButtsLayout->setAlignment(Qt::AlignLeft);
				PosiMenuButtsLayout->setContentsMargins(0,0,0,0);
				/*************************************************************/
				// insert menu 添加按钮 10个
				/*************************************************************/

				
				QHBoxLayout *InsertMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				InsertMenuButtsLayout->addWidget(insWire1);
				InsertMenuButtsLayout->addWidget(insLabel1);
				InsertMenuButtsLayout->addWidget(insEquation1);
				InsertMenuButtsLayout->addWidget(insGround1);
				InsertMenuButtsLayout->addWidget(insPort1);
				InsertMenuButtsLayout->addWidget(setMarker1);
				InsertMenuButtsLayout->addWidget(insEntity1);

				InsertMenuButtsLayout->setSpacing(0);
				InsertMenuButtsLayout->addStretch();
				InsertMenuButtsLayout->setAlignment(Qt::AlignLeft);
				InsertMenuButtsLayout->setContentsMargins(0,0,0,0);

				/*************************************************************/
				// project menu 添加按钮 10个
				/*************************************************************/

				QHBoxLayout *ProjectMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				ProjectMenuButtsLayout->addWidget(projNew1);
				ProjectMenuButtsLayout->addWidget(projOpen1);
				ProjectMenuButtsLayout->addWidget(addToProj1);
				ProjectMenuButtsLayout->addWidget(projDel1);
				ProjectMenuButtsLayout->addWidget(projClose1);
				ProjectMenuButtsLayout->addWidget(createLib1);
				ProjectMenuButtsLayout->addWidget(createPkg1);
				ProjectMenuButtsLayout->addWidget(extractPkg1);
				ProjectMenuButtsLayout->addWidget(importData1);
				ProjectMenuButtsLayout->addWidget(graph2csv1);
				ProjectMenuButtsLayout->addWidget(buildModule1);
				ProjectMenuButtsLayout->addWidget(loadModule1);

				ProjectMenuButtsLayout->setSpacing(0);
				ProjectMenuButtsLayout->addStretch();
				ProjectMenuButtsLayout->setAlignment(Qt::AlignLeft);
				ProjectMenuButtsLayout->setContentsMargins(0,0,0,0);
				/*************************************************************/
				// tools menu 添加按钮 10个
				/*************************************************************/
				QHBoxLayout *ToolsMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				ToolsMenuButtsLayout->addWidget(callEditor1);
				ToolsMenuButtsLayout->addWidget(callFilter1);
				ToolsMenuButtsLayout->addWidget(callLine1);
				ToolsMenuButtsLayout->addWidget(callLib1);
				ToolsMenuButtsLayout->addWidget(callMatch1);
				ToolsMenuButtsLayout->addWidget(callAtt1);
				ToolsMenuButtsLayout->addWidget(callRes1);

				ToolsMenuButtsLayout->setSpacing(0);
				ToolsMenuButtsLayout->addStretch();
				ToolsMenuButtsLayout->setAlignment(Qt::AlignLeft);
				ToolsMenuButtsLayout->setContentsMargins(0,0,0,0);
				/*************************************************************/
				// simulation menu 添加按钮 10个
				/*************************************************************/
				QHBoxLayout *SimuMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				SimuMenuButtsLayout->addWidget(simulate1);
				SimuMenuButtsLayout->addWidget(dpl_sch1);
				SimuMenuButtsLayout->addWidget(dcbias1);
				SimuMenuButtsLayout->addWidget(showMsg1);
				SimuMenuButtsLayout->addWidget(showNet1);

				SimuMenuButtsLayout->setSpacing(0);
				SimuMenuButtsLayout->addStretch();
				SimuMenuButtsLayout->setAlignment(Qt::AlignLeft);
				SimuMenuButtsLayout->setContentsMargins(0,0,0,0);
				/*************************************************************/
				// view menu 添加按钮 10个
				/*************************************************************/
				QHBoxLayout *ViewMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				ViewMenuButtsLayout->addWidget(magAll1);
				ViewMenuButtsLayout->addWidget(magOne1);
				ViewMenuButtsLayout->addWidget(magPlus1);
				ViewMenuButtsLayout->addWidget(magMinus1);
				ViewMenuButtsLayout->addWidget(viewToolBar1);
				ViewMenuButtsLayout->addWidget(viewStatusBar1);
				ViewMenuButtsLayout->addWidget(viewBrowseDock1);
				ViewMenuButtsLayout->addWidget(viewOctaveDock1);

				ViewMenuButtsLayout->setSpacing(0);
				ViewMenuButtsLayout->addStretch();
				ViewMenuButtsLayout->setAlignment(Qt::AlignLeft);
				ViewMenuButtsLayout->setContentsMargins(0,0,0,0);
				/*************************************************************/
				// help menu 添加按钮 10个
				/*************************************************************/
				QHBoxLayout *HelpMenuButtsLayout = new QHBoxLayout();//水平按钮布局
				HelpMenuButtsLayout->addWidget(helpIndex1);
				HelpMenuButtsLayout->addWidget(helpGetStart1);
				HelpMenuButtsLayout->addWidget(helpAboutApp1);
				HelpMenuButtsLayout->addWidget(helpAboutQt1);

				HelpMenuButtsLayout->setSpacing(0);
				HelpMenuButtsLayout->addStretch();
				HelpMenuButtsLayout->setAlignment(Qt::AlignLeft);
				HelpMenuButtsLayout->setContentsMargins(0,0,0,0);


		QWidget *FileMenu = new QWidget();
		FileMenu->setLayout(FileMenuButtsLayout);
		FileMenu->setContentsMargins(0,0,0,0);

		QWidget *EditMenu = new QWidget();
		EditMenu->setLayout(EditMenuButtsLayout);
		EditMenu->setContentsMargins(0,0,0,0);

		QWidget *PosiMenu = new QWidget();
		PosiMenu->setLayout(PosiMenuButtsLayout);
		PosiMenu->setContentsMargins(0,0,0,0);

		QWidget *InsertMenu = new QWidget();
		InsertMenu->setLayout(InsertMenuButtsLayout);
		InsertMenu->setContentsMargins(0,0,0,0);

		QWidget *ProjectMenu = new QWidget();
		ProjectMenu->setLayout(ProjectMenuButtsLayout);
		ProjectMenu->setContentsMargins(0,0,0,0);

		QWidget *ToolsMenu = new QWidget();
		ToolsMenu->setLayout(ToolsMenuButtsLayout);
		ToolsMenu->setContentsMargins(0,0,0,0);

		QWidget *SimuMenu = new QWidget();
		SimuMenu->setLayout(SimuMenuButtsLayout);
		SimuMenu->setContentsMargins(0,0,0,0);

		QWidget *ViewMenu = new QWidget();
		ViewMenu->setLayout(ViewMenuButtsLayout);
		ViewMenu->setContentsMargins(0,0,0,0);

		QWidget *HelpMenu = new QWidget();
		HelpMenu->setLayout(HelpMenuButtsLayout);
		HelpMenu->setContentsMargins(0,0,0,0);


	menuTabView->addTab(FileMenu, tr("&File"));
	menuTabView->addTab(EditMenu, tr("&Edit"));
	menuTabView->addTab(PosiMenu, tr("P&ositioning"));
	menuTabView->addTab(InsertMenu, tr("&Insert"));
	menuTabView->addTab(ProjectMenu, tr("&Project"));
	menuTabView->addTab(ToolsMenu, tr("&Tools"));
	menuTabView->addTab(SimuMenu, tr("&Simulation"));
	menuTabView->addTab(ViewMenu, tr("&View"));
	menuTabView->addTab(HelpMenu, tr("&Help"));
	menuTabView->setContentsMargins(0,0,0,0);
	//menuDock->setWidget(menuTabView);

	//// 加载CSS文件
	QString strPath = QCoreApplication::applicationDirPath();  
	qDebug()<<strPath;  
	QString strCssFile = strPath + "/default.css";  
	QFile fCss(strCssFile);  
	if( !fCss.open(QFile::ReadOnly))  
	{  
		qDebug("css File %s load false",strCssFile);  
	//        return;  
	}  
	QString strCssContent(fCss.readAll());  
	menuDock->setStyleSheet(strCssContent);  
	fCss.close(); 
}

// ----------------------------------------------------------
void SwieeApp::initMenuBar()
{
	fileMenu = new QMenu(tr("&File"));  // menuBar entry fileMenu
	fileMenu->setFont(QFont("SimSun", 9));
	fileMenu->addAction(fileNew);
	fileMenu->addAction(textNew);
	fileMenu->addAction(fileOpen);
	fileMenu->addAction(fileClose);

	recentfilesMenu = new QMenu(tr("Open Recent"),fileMenu);
	fileMenu->addMenu(recentfilesMenu);
	slotUpdateRecentFiles();

	fileMenu->insertSeparator();
	fileMenu->addAction(fileSave);
	fileMenu->addAction(fileSaveAll);
	fileMenu->addAction(fileSaveAs);
	fileMenu->addAction(exportAsImage);
	fileMenu->addAction(filePrint);
	fileMenu->addAction(filePrintFit);
	fileMenu->insertSeparator();
	fileMenu->addAction(fileExamples);
	fileMenu->insertSeparator();
	fileMenu->addAction(fileSettings);
	fileMenu->addAction(symEdit);
	fileMenu->insertSeparator();
	fileMenu->addAction(applSettings);
	fileMenu->addAction(refreshSchPath);
	fileMenu->insertSeparator();
	fileMenu->addAction(fileQuit);


	editMenu = new QMenu(tr("&Edit"));  // menuBar entry editMenu
	editMenu->setFont(QFont("SimSun", 9));
	editMenu->addAction(undo);
	editMenu->addAction(redo);
	editMenu->insertSeparator();
	editMenu->addAction(editCut);
	editMenu->addAction(editCopy);
	editMenu->addAction(editPaste);
	editMenu->addAction(editDelete);
	editMenu->insertSeparator();
	editMenu->addAction(select);
	editMenu->addAction(selectAll);
	editMenu->addAction(selectMarker);
	editMenu->addAction(editFind);
	editMenu->addAction(editFindAgain);
	editMenu->addAction(changeProps);
	editMenu->addAction(editRotate);
	editMenu->addAction(editMirror);
	editMenu->addAction(editMirrorY);
	/*
	editMenu->addAction(editActivate);
	editMenu->insertSeparator();
	editMenu->addAction(intoH);
	editMenu->addAction(popH);
	*/

	alignMenu = new QMenu(tr("P&ositioning"));  // menuBar entry alignMenu
	alignMenu->setFont(QFont("SimSun", 9));
	alignMenu->addAction(moveText);
	alignMenu->addAction(onGrid);
	alignMenu->insertSeparator();
	alignMenu->addAction(centerHor);
	alignMenu->addAction(centerVert);
	alignMenu->insertSeparator();
	alignMenu->addAction(alignTop);
	alignMenu->addAction(alignBottom);
	alignMenu->addAction(alignLeft);
	alignMenu->addAction(alignRight);
	alignMenu->insertSeparator();
	alignMenu->addAction(distrHor);
	alignMenu->addAction(distrVert);

	insMenu = new QMenu(tr("&Insert"));  // menuBar entry insMenu
	insMenu->setFont(QFont("SimSun", 9));
	insMenu->addAction(insWire);
	insMenu->addAction(insLabel);
	/*
	insMenu->addAction(insEquation);
	insMenu->addAction(insGround);
	insMenu->addAction(insPort);
	insMenu->addAction(setMarker);
	insMenu->addAction(insEntity);
	*/  

	projMenu = new QMenu(tr("&Project"));  // menuBar entry projMenu
	projMenu->setFont(QFont("SimSun", 9));
	projMenu->addAction(projNew);
	projMenu->addAction(projOpen);
	projMenu->addAction(addToProj);
	projMenu->addAction(projClose);
	projMenu->addAction(projDel);
	/*
	projMenu->insertSeparator();
	projMenu->addAction(createLib);
	projMenu->addAction(createPkg);
	projMenu->addAction(extractPkg);
	projMenu->insertSeparator();
	projMenu->addAction(importData);
	projMenu->addAction(graph2csv);
	projMenu->insertSeparator();
	// TODO only enable if document is VA file
	projMenu->addAction(buildModule);
	projMenu->addAction(loadModule);
	*/

	toolMenu = new QMenu(tr("&Tools"));  // menuBar entry toolMenu
	toolMenu->setFont(QFont("SimSun", 9));
	toolMenu->addAction(callEditor);
	toolMenu->addAction(callFilter);
	toolMenu->addAction(callLine);
	toolMenu->addAction(callLib);
	toolMenu->addAction(callMatch);
	toolMenu->addAction(callAtt);
	toolMenu->addAction(callRes); 


	simMenu = new QMenu(tr("&Simulation"));  // menuBar entry simMenu
	simMenu->setFont(QFont("SimSun", 9));
	simMenu->addAction(simulate);

	simMenu->addAction(dpl_sch);
	simMenu->addAction(dcbias);
	simMenu->addAction(showMsg);
	simMenu->addAction(showNet);


	viewMenu = new QMenu(tr("&View"));  // menuBar entry viewMenu
	viewMenu->setFont(QFont("SimSun", 9));
	viewMenu->addAction(magAll);
	viewMenu->addAction(magOne);
	viewMenu->addAction(magPlus);
	viewMenu->addAction(magMinus);
	viewMenu->insertSeparator();
	viewMenu->setCheckable(true);
	viewMenu->addAction(viewToolBar);
	viewMenu->addAction(viewStatusBar);
	viewMenu->addAction(viewBrowseDock);
	//  viewMenu->addAction(viewOctaveDock);


	helpMenu = new QMenu(tr("&Help"));  // menuBar entry helpMenu
	helpMenu->setFont(QFont("SimSun", 9));
	helpMenu->addAction(helpIndex);
	helpMenu->addAction(helpGetStart);
	helpMenu->insertSeparator();



	//Fill submenu's with filenames of PDF documents
	QDir TechnicalDir = QDir(SwieeSettings.DocDir.replace('\\','/'));

	if(TechnicalDir.cd("technical"))
	{
		helpTechnical = new QMenu(tr("&Technical Papers"));
		helpMenu->addMenu(helpTechnical);
		TechnicalDir.setFilter(QDir::Files);
		QStringList entries = TechnicalDir.entryList();
		for(int i=0;i<entries.size();i++)
		{
			QAction* helpTechnicalActions = new QAction(entries[i], this);
			helpTechnicalActions->setObjectName ( entries[i] );
			helpTechnicalActions->setStatusTip(tr("Open ")+entries[i]);
			helpTechnicalActions->setWhatsThis(tr(entries[i]+"\n\nOpen "+entries[i]));
			connect(helpTechnicalActions, SIGNAL(activated()), SLOT(slotHelpTechnical()));
			helpTechnical->addAction(helpTechnicalActions);
		}

	}

	//Fill submenu's with filenames of PDF documents
	QDir ReportDir = QDir(SwieeSettings.DocDir.replace('\\','/'));
	if(ReportDir.cd("report"))
	{
		helpReport = new QMenu(tr("Technical &Reports"));
		helpMenu->addMenu(helpReport);
		ReportDir.setFilter(QDir::Files);
		QStringList entries = ReportDir.entryList();
		for(int i=0;i<entries.size();i++)
		{
			QAction* helpReportActions = new QAction(entries[i], this);
			helpReportActions->setObjectName ( entries[i] );
			helpReportActions->setStatusTip(tr("Open ")+entries[i]);
			helpReportActions->setWhatsThis(tr(entries[i]+"\n\nOpen "+entries[i]));
			connect(helpReportActions, SIGNAL(activated()), SLOT(slotHelpReport()));
			helpReport->addAction(helpReportActions);
		}
	}

	//Fill submenu's with filenames of PDF documents
	QDir TutorialDir = QDir(SwieeSettings.DocDir.replace('\\','/'));
	if(TutorialDir.cd("tutorial"))
	{
		helpTutorial = new QMenu(tr("T&utorials"));
		helpMenu->addMenu(helpTutorial);
		TutorialDir.setFilter(QDir::Files);
		QStringList entries = TutorialDir.entryList();
		for(int i=0;i<entries.size();i++)
		{
			QAction* helpTutorialActions = new QAction(entries[i], this);
			helpTutorialActions->setObjectName ( entries[i] );
			helpTutorialActions->setStatusTip(tr("Open ")+entries[i]);
			helpTutorialActions->setWhatsThis(tr(entries[i]+"\n\nOpen "+entries[i]));
			connect(helpTutorialActions, SIGNAL(activated()), SLOT(slotHelpTutorial()));
			helpTutorial->addAction(helpTutorialActions);
		}
	}


	helpMenu->insertSeparator();
	helpMenu->addAction(helpAboutApp);
	helpMenu->addAction(helpAboutQt);


	//menuBar()->addMenu(fileMenu);
	//menuBar()->addMenu(editMenu);
	//menuBar()->addMenu(alignMenu);
	//menuBar()->addMenu(insMenu);
	//menuBar()->addMenu(projMenu);
	//menuBar()->addMenu(toolMenu);
	//menuBar()->addMenu(simMenu);
	//menuBar()->addMenu(viewMenu);
	//menuBar()->insertSeparator();
	//menuBar()->addMenu(helpMenu);
	//menuBar()->setFont(QFont("SimSun",9)); // added by xuliang to control display character fonts
}

// ----------------------------------------------------------
void SwieeApp::initToolBar()
{
	fileToolbar = new QToolBar(tr("File"));
	this->addToolBar(fileToolbar);
	//fileToolbar->addAction(fileNew);
	fileToolbar->addAction(textNew);
	fileToolbar->addAction(fileOpen);
	fileToolbar->addAction(fileSave);
	fileToolbar->addAction(fileSaveAll);
	fileToolbar->addAction(fileClose);
	fileToolbar->addAction(filePrint);

	editToolbar = new QToolBar(tr("Edit"));
	this->addToolBar(editToolbar);
	editToolbar->addAction(editCut);
	editToolbar->addAction(editCopy);
	editToolbar->addAction(editPaste);
	editToolbar->addAction(editDelete);
	editToolbar->addAction(undo);
	editToolbar->addAction(redo);

	viewToolbar = new QToolBar(tr("View"));
	this->addToolBar(viewToolbar);
	viewToolbar->addAction(magAll);
	viewToolbar->addAction(magOne);
	viewToolbar->addAction(magPlus);
	viewToolbar->addAction(magMinus);


	workToolbar = new QToolBar(tr("Work"));
	this->addToolBar(workToolbar);
	workToolbar->addAction(select);
	//  workToolbar->addAction(editActivate);
	//  workToolbar->addAction(editMirror);
	//  workToolbar->addAction(editMirrorY);
	//  workToolbar->addAction(editRotate);
	//  workToolbar->addAction(intoH);
	//  workToolbar->addAction(popH);
	workToolbar->addAction(insWire);
	workToolbar->addAction(insLabel);
	//  workToolbar->addAction(insEquation);
	workToolbar->addAction(insGround);
	workToolbar->addAction(insPort);
	workToolbar->addAction(simulate);
	//  workToolbar->addAction(dpl_sch);
	//  workToolbar->addAction(setMarker);
	//  workToolbar->addSeparator();    

	// <<<=======================

}
// ----------------------------------------------------------
//void SwieeApp::initToolBar()
//{
//	//fileToolbar = new QToolBar(tr("File"));
//	//this->addToolBar(fileToolbar);
//	////fileToolbar->addAction(fileNew);
//	//fileToolbar->addAction(textNew);
//	//fileToolbar->addAction(fileOpen);
//	//fileToolbar->addAction(fileSave);
//	//fileToolbar->addAction(fileSaveAll);
//	//fileToolbar->addAction(fileClose);
//	//fileToolbar->addAction(filePrint);
//
//	//editToolbar = new QToolBar(tr("Edit"));
//	//this->addToolBar(editToolbar);
//	//editToolbar->addAction(editCut);
//	//editToolbar->addAction(editCopy);
//	//editToolbar->addAction(editPaste);
//	//editToolbar->addAction(editDelete);
//	//editToolbar->addAction(undo);
//	//editToolbar->addAction(redo);
//
//	//viewToolbar = new QToolBar(tr("View"));
//	//this->addToolBar(viewToolbar);
//	//viewToolbar->addAction(magAll);
//	//viewToolbar->addAction(magOne);
//	//viewToolbar->addAction(magPlus);
//	//viewToolbar->addAction(magMinus);
//
//
//	//workToolbar = new QToolBar(tr("Work"));
//	//this->addToolBar(workToolbar);
//	//workToolbar->addAction(select);
//	////  workToolbar->addAction(editActivate);
//	////  workToolbar->addAction(editMirror);
//	////  workToolbar->addAction(editMirrorY);
//	////  workToolbar->addAction(editRotate);
//	////  workToolbar->addAction(intoH);
//	////  workToolbar->addAction(popH);
//	//workToolbar->addAction(insWire);
//	//workToolbar->addAction(insLabel);
//	////  workToolbar->addAction(insEquation);
//	//workToolbar->addAction(insGround);
//	//workToolbar->addAction(insPort);
//	//workToolbar->addAction(simulate);
//	////  workToolbar->addAction(dpl_sch);
//	////  workToolbar->addAction(setMarker);
//	////  workToolbar->addSeparator();    
//
//	//// <<<=======================
//
//}

// ----------------------------------------------------------
void SwieeApp::initStatusBar()
{
	// To reserve enough space, insert the longest text and rewrite it afterwards.
	WarningLabel = new QLabel(tr("no warnings"), statusBar());
	statusBar()->addWidget(WarningLabel, 0, true);

	PositionLabel = new QLabel("0 : 0", statusBar());
	PositionLabel->setAlignment(Qt::AlignRight);
	statusBar()->addWidget(PositionLabel, 0, true);

	statusBar()->message(tr("Ready."), 2000);
}

// ----------------------------------------------------------
void SwieeApp::slotShowWarnings()
{
	static int ResultState = 0;

	if(ResultState == 0) {
		QFont f = WarningLabel->font();
		f.setWeight(QFont::DemiBold);
		WarningLabel->setFont(f);
		WarningLabel->setText(tr("Warnings in last simulation! Press F5"));
	}

	ResultState++;
	if(ResultState & 1)
		WarningLabel->setPaletteForegroundColor(Qt::red);
	else
		WarningLabel->setPaletteForegroundColor(Qt::black);

	if(ResultState < 9)
		QTimer::singleShot(500, this, SLOT(slotShowWarnings()));
	else
		ResultState = 0;
}

// ----------------------------------------------------------
void SwieeApp::slotResetWarnings()
{
	QFont f = WarningLabel->font();   // reset warning label
	f.setWeight(QFont::Normal);
	WarningLabel->setFont(f);
	WarningLabel->setPaletteForegroundColor(Qt::black);
	WarningLabel->setText(tr("no warnings"));
}

// ----------------------------------------------------------
void SwieeApp::printCursorPosition(int x, int y)
{
	PositionLabel->setText(QString::number(x)+" : "+QString::number(y));
	PositionLabel->setMinimumWidth(PositionLabel->width());
}

// ----------------------------------------------------------
// turn Toolbar on or off
void SwieeApp::slotViewToolBar(bool toggle)
{
	if (toggle== false) {
		fileToolbar->show();
		editToolbar->show();
		viewToolbar->show();
		workToolbar->show();
	}
	else {

		fileToolbar->hide();
		editToolbar->hide();
		viewToolbar->hide();
		workToolbar->hide();
	}
}

// ----------------------------------------------------------
// turn Statusbar on or off
void SwieeApp::slotViewStatusBar(bool toggle)
{
	if (toggle == false)
		statusBar()->hide();
	else
		statusBar()->show();
}

// ----------------------------------------------------------
// turn Brwose Dock Window on or off
void SwieeApp::slotViewBrowseDock(bool toggle)
{
	if (toggle == false)
		dock->hide();
	else
		dock->show();
}

// ----------------------------------------------------------
void SwieeApp::slotToggleDock(bool on)
{
	viewBrowseDock->blockSignals(true);
	viewBrowseDock->setOn(on);
	viewBrowseDock->blockSignals(false);
}

// ----------------------------------------------------------
// turn Octave Dock Window on or off
void SwieeApp::slotViewOctaveDock(bool toggle)
{
	if(toggle) {
		octDock->show();
		octave->startOctave();
	}
	else
		octDock->hide();
}

// ----------------------------------------------------------
void SwieeApp::slotToggleOctave(bool on)
{
	viewOctaveDock->blockSignals(true);
	viewOctaveDock->setOn(on);
	viewOctaveDock->blockSignals(false);
}

// ----------------------------------------------------------
void SwieeApp::slotHelpAbout()
{
	QMessageBox::about(this, tr("About..."),
		tr("Swiee Version")+" "+PACKAGE_VERSION+"\n"+
		tr("Quite Universal Circuit Simulator")+"\n"+
		tr("Copyright (C)")+" 2003-2009 "+
		tr("by Michael Margraf")+"\n"+
		tr("Copyright (C)")+" 2011-2014 "+
		tr("Swiee Team")+"\n"+
		"\nThis is free software; see the source for copying conditions."
		"\nThere is NO warranty; not even for MERCHANTABILITY or "
		"\nFITNESS FOR A PARTICULAR PURPOSE.\n\n"+
		tr("Simulator by Stefan Jahn")+"\n"+
		tr("VHDL simulator 'FreeHDL' by Edwin Naroska and Marius Vollmer")+"\n"+
		tr("Special thanks to Jens Flucke and Raimund Jacob")+"\n"+
		tr("Many thanks to Mike Brinson for correcting the VHDL output")+"\n"+
		tr("GUI improvements by Gopala Krishna A")+"\n"+
		tr("Verilog-AMS interface by Helene Parruitte")+"\n"+
		tr("Verilog-AMS dynamic loader by Guilherme Brondani Torri")+"\n\n"+
		tr("Translations:")+"\n"+
		tr("German by Stefan Jahn")+"\n"+
		tr("Polish by Dariusz Pienkowski")+"\n"+
		tr("Romanian by Radu Circa")+"\n"+
		tr("French by Vincent Habchi, F5RCS")+"\n"+
		tr("Portuguese by Luciano Franca, Helio de Sousa, Guilherme Brondani Torri")+"\n"+
		tr("Spanish by Jose L. Redrejo Rodriguez")+"\n"+
		tr("Japanese by Toyoyuki Ishikawa")+"\n"+
		tr("Italian by Giorgio Luparia and Claudio Girardi")+"\n"+
		tr("Hebrew by Dotan Nahum")+"\n"+
		tr("Swedish by Peter Landgren")+"\n"+
		tr("Turkish by Onur and Ozgur Cobanoglu")+"\n"+
		tr("Hungarian by Jozsef Bus")+"\n"+
		tr("Russian by Igor Gorbounov")+"\n"+
		tr("Czech by Marek Straka")+"\n"+
		tr("Catalan by Antoni Subirats")+"\n"+
		tr("Arabic by Chabane Noureddine")+"\n"+
		tr("Kazakh by Erbol Keshubaev"));
}

// ----------------------------------------------------------
void SwieeApp::slotHelpAboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

