#include <QtGui>
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "swieesettingsdialog.h"
#include <iostream>
#include <QGridLayout>
#include <QVBoxLayout>
#include "main.h"
#include "textdoc.h"
#include "schematic.h"

#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QLayout>
#include <QColorDialog>
#include <QFontDialog>
#include <QValidator>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>

using namespace std;

SwieeSettingsDialog::SwieeSettingsDialog(SwieeApp *parent, const char *name)
    : QDialog(parent, name)
{
    App = parent;
    setWindowTitle(tr("Edit Swiee Properties"));

    Expr.setPattern("[\\w_]+");
    Validator  = new QRegExpValidator(Expr, this);

    all = new QVBoxLayout(this); // to provide the neccessary size
    QTabWidget *t = new QTabWidget();
    all->addWidget(t);

    // ...........................................................
    // The application setings tab
    QWidget *appSettingsTab = new QWidget(t);
    QGridLayout *appSettingsGrid = new QGridLayout(appSettingsTab);

    appSettingsGrid->addWidget(new QLabel(tr("Font (set after reload):"), appSettingsTab), 0,0);
    FontButton = new QPushButton(appSettingsTab);
    connect(FontButton, SIGNAL(clicked()), SLOT(slotFontDialog()));
    appSettingsGrid->addWidget(FontButton,0,1);

    appSettingsGrid->addWidget(new QLabel(tr("Document Background Color:"), appSettingsTab) ,1,0);
    BGColorButton = new QPushButton("      ", appSettingsTab);
    connect(BGColorButton, SIGNAL(clicked()), SLOT(slotBGColorDialog()));
    appSettingsGrid->addWidget(BGColorButton,1,1);

    appSettingsGrid->addWidget(new QLabel(tr("Language (set after reload):"), appSettingsTab) ,2,0);
    LanguageCombo = new QComboBox(appSettingsTab);
    LanguageCombo->insertItem(tr("system language"));
    LanguageCombo->insertItem(tr("English")+" (en)");
    LanguageCombo->insertItem(tr("German")+" (de)");
    LanguageCombo->insertItem(tr("French")+" (fr)");
    LanguageCombo->insertItem(tr("Spanish")+" (es)");
    LanguageCombo->insertItem(tr("Italian")+" (it)");
    LanguageCombo->insertItem(tr("Polish")+" (pl)");
    LanguageCombo->insertItem(tr("Romanian")+" (ro)");
    LanguageCombo->insertItem(tr("Japanese")+" (jp)");
    LanguageCombo->insertItem(tr("Swedish")+" (sv)");
    LanguageCombo->insertItem(tr("Hungarian")+" (hu)");
    LanguageCombo->insertItem(tr("Hebrew")+" (he)");
    LanguageCombo->insertItem(tr("Portuguese-BR")+" (pt_BR)");
    LanguageCombo->insertItem(tr("Portuguese-PT")+" (pt_PT)");
    LanguageCombo->insertItem(tr("Turkish")+" (tr)");
    LanguageCombo->insertItem(tr("Ukrainian")+" (uk)");
    LanguageCombo->insertItem(tr("Russian")+" (ru)");
    LanguageCombo->insertItem(tr("Czech")+" (cs)");
    LanguageCombo->insertItem(tr("Catalan")+" (ca)");
    LanguageCombo->insertItem(tr("Arabic")+" (ar)");
    LanguageCombo->insertItem(tr("Kazakh")+" (kk)");
    appSettingsGrid->addWidget(LanguageCombo,2,1);

    val200 = new QIntValidator(0, 200, this);
    appSettingsGrid->addWidget(new QLabel(tr("maximum undo operations:"), appSettingsTab) ,3,0);
    undoNumEdit = new QLineEdit(appSettingsTab);
    undoNumEdit->setValidator(val200);
    appSettingsGrid->addWidget(undoNumEdit,3,1);

    appSettingsGrid->addWidget(new QLabel(tr("text editor:"), appSettingsTab) ,4,0);
    editorEdit = new QLineEdit(appSettingsTab);
    editorEdit->setToolTip(tr("Set to qucs, qucsedit or the path to your favorite text editor."));
    appSettingsGrid->addWidget(editorEdit,4,1);

    appSettingsGrid->addWidget(new QLabel(tr("start wiring when clicking open node:"), appSettingsTab) ,5,0);
    checkWiring = new QCheckBox(appSettingsTab);
    appSettingsGrid->addWidget(checkWiring,5,1);


    appSettingsGrid->addWidget(new QLabel(tr("Load documents from future versions ")));
    checkLoadFromFutureVersions = new QCheckBox(appSettingsTab);
    appSettingsGrid->addWidget(checkLoadFromFutureVersions,6,1);
    checkLoadFromFutureVersions->setChecked(SwieeSettings.IgnoreFutureVersion);

    t->addTab(appSettingsTab, tr("Settings"));

    // ...........................................................
    // The source code editor settings tab
    QWidget *editorTab = new QWidget(t);
    QGridLayout *editorGrid = new QGridLayout(editorTab);

    editorGrid->addMultiCellWidget(new QLabel(tr("Colors for Syntax Highlighting:"), editorTab), 0,0,0,1);

    ColorComment = new QPushButton(tr("Comment"), editorTab);
    ColorComment->setPaletteForegroundColor(SwieeSettings.Comment);
    ColorComment->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorComment, SIGNAL(clicked()), SLOT(slotColorComment()));
    editorGrid->addWidget(ColorComment,1,0);

    ColorString = new QPushButton(tr("String"), editorTab);
    ColorString->setPaletteForegroundColor(SwieeSettings.String);
    ColorString->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorString, SIGNAL(clicked()), SLOT(slotColorString()));
    editorGrid->addWidget(ColorString,1,1);

    ColorInteger = new QPushButton(tr("Integer Number"), editorTab);
    ColorInteger->setPaletteForegroundColor(SwieeSettings.Integer);
    ColorInteger->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorInteger, SIGNAL(clicked()), SLOT(slotColorInteger()));
    editorGrid->addWidget(ColorInteger,1,2);

    ColorReal = new QPushButton(tr("Real Number"), editorTab);
    ColorReal->setPaletteForegroundColor(SwieeSettings.Real);
    ColorReal->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorReal, SIGNAL(clicked()), SLOT(slotColorReal()));
    editorGrid->addWidget(ColorReal,2,0);

    ColorCharacter = new QPushButton(tr("Character"), editorTab);
    ColorCharacter->setPaletteForegroundColor(SwieeSettings.Character);
    ColorCharacter->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorCharacter, SIGNAL(clicked()), SLOT(slotColorCharacter()));
    editorGrid->addWidget(ColorCharacter,2,1);

    ColorDataType = new QPushButton(tr("Data Type"), editorTab);
    ColorDataType->setPaletteForegroundColor(SwieeSettings.Type);
    ColorDataType->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorDataType, SIGNAL(clicked()), SLOT(slotColorDataType()));
    editorGrid->addWidget(ColorDataType,2,2);

    ColorAttribute = new QPushButton(tr("Attribute"), editorTab);
    ColorAttribute->setPaletteForegroundColor(SwieeSettings.Attribute);
    ColorAttribute->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorAttribute, SIGNAL(clicked()), SLOT(slotColorAttribute()));
    editorGrid->addWidget(ColorAttribute,3,0);

    ColorDirective = new QPushButton(tr("Directive"), editorTab);
    ColorDirective->setPaletteForegroundColor(SwieeSettings.Directive);
    ColorDirective->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorDirective, SIGNAL(clicked()), SLOT(slotColorDirective()));
    editorGrid->addWidget(ColorDirective,3,1);

    ColorTask = new QPushButton(tr("Task"), editorTab);
    ColorTask->setPaletteForegroundColor(SwieeSettings.Task);
    ColorTask->setPaletteBackgroundColor(SwieeSettings.BGColor);
    connect(ColorTask, SIGNAL(clicked()), SLOT(slotColorTask()));
    editorGrid->addWidget(ColorTask,3,2);


    t->addTab(editorTab, tr("Source Code Editor"));

    // ...........................................................
    // The file types tab
    QWidget *fileTypesTab = new QWidget(t);
    QGridLayout *fileTypesGrid = new QGridLayout(fileTypesTab);

    QLabel *note = new QLabel(
        tr("Register filename extensions here in order to\nopen files with an appropriate program."));
    fileTypesGrid->addWidget(note,0,0,1,2);

    // the fileTypesTableWidget displays information on the file types
    fileTypesTableWidget = new QTableWidget(fileTypesTab);
    fileTypesTableWidget->setColumnCount(2);

    QTableWidgetItem *item1 = new QTableWidgetItem();
    QTableWidgetItem *item2 = new QTableWidgetItem();

    fileTypesTableWidget->setHorizontalHeaderItem(0, item1);
    fileTypesTableWidget->setHorizontalHeaderItem(1, item2);

    item1->setText(tr("Suffix"));
    item2->setText(tr("Program"));

    fileTypesTableWidget->horizontalHeader()->setStretchLastSection(true);
    fileTypesTableWidget->verticalHeader()->hide();
    connect(fileTypesTableWidget, SIGNAL(cellClicked(int,int)), SLOT(slotTableClicked(int,int)));
    fileTypesGrid->addWidget(fileTypesTableWidget,1,0,3,1);

    // fill listview with already registered file extensions
    QStringList::Iterator it = SwieeSettings.FileTypes.begin();
    while(it != SwieeSettings.FileTypes.end())
    {
        int row = fileTypesTableWidget->rowCount();
        fileTypesTableWidget->setRowCount(row+1);
        QTableWidgetItem *suffix = new QTableWidgetItem(QString((*it).section('/',0,0)));
        QTableWidgetItem *program = new QTableWidgetItem(QString((*it).section('/',1,1)));
        suffix->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        program->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        fileTypesTableWidget->setItem(row, 0, suffix);
        fileTypesTableWidget->setItem(row, 1, program);
        it++;
    }

    QLabel *l5 = new QLabel(tr("Suffix:"), fileTypesTab);
    fileTypesGrid->addWidget(l5,1,1);
    Input_Suffix = new QLineEdit(fileTypesTab);
    Input_Suffix->setValidator(Validator);
    fileTypesGrid->addWidget(Input_Suffix,1,2);
//  connect(Input_Suffix, SIGNAL(returnPressed()), SLOT(slotGotoProgEdit())); //not implemented

    QLabel *l6 = new QLabel(tr("Program:"), fileTypesTab);
    fileTypesGrid->addWidget(l6,2,1);
    Input_Program = new QLineEdit(fileTypesTab);
    fileTypesGrid->addWidget(Input_Program,2,2);

    QPushButton *AddButt = new QPushButton(tr("Set"));
    fileTypesGrid->addWidget(AddButt,3,1);
    connect(AddButt, SIGNAL(clicked()), SLOT(slotAddFileType()));
    QPushButton *RemoveButt = new QPushButton(tr("Remove"));
    fileTypesGrid->addWidget(RemoveButt,3,2);
    connect(RemoveButt, SIGNAL(clicked()), SLOT(slotRemoveFileType()));

    fileTypesGrid->setRowStretch(3,4);
    t->addTab(fileTypesTab, tr("File Types"));

    // ...........................................................
    // The locations tab
    QWidget *locationsTab = new QWidget(t);
    QGridLayout *locationsGrid = new QGridLayout(locationsTab);

    QLabel *note2 = new QLabel(
        tr("Edit the standard paths and external applications"));
    locationsGrid->addWidget(note2,0,0,1,2);

    locationsGrid->addWidget(new QLabel(tr("Swiee Home:"), locationsTab) ,1,0);
    homeEdit = new QLineEdit(locationsTab);
    locationsGrid->addWidget(homeEdit,1,1);
    QPushButton *HomeButt = new QPushButton("...");
    locationsGrid->addWidget(HomeButt, 1, 2);
    connect(HomeButt, SIGNAL(clicked()), SLOT(slotHomeDirBrowse()));

    locationsGrid->addWidget(new QLabel(tr("AdmsXml Path:"), locationsTab) ,2,0);
    admsXmlEdit = new QLineEdit(locationsTab);
    locationsGrid->addWidget(admsXmlEdit,2,1);
    QPushButton *AdmsXmlButt = new QPushButton("...");
    locationsGrid->addWidget(AdmsXmlButt, 2, 2);
    connect(AdmsXmlButt, SIGNAL(clicked()), SLOT(slotAdmsXmlDirBrowse()));

    locationsGrid->addWidget(new QLabel(tr("ASCO Path:"), locationsTab) ,3,0);
    ascoEdit = new QLineEdit(locationsTab);
    locationsGrid->addWidget(ascoEdit,3,1);
    QPushButton *ascoButt = new QPushButton("...");
    locationsGrid->addWidget(ascoButt, 3, 2);
    connect(ascoButt, SIGNAL(clicked()), SLOT(slotAscoDirBrowse()));

    locationsGrid->addWidget(new QLabel(tr("Octave Path:"), locationsTab) ,4,0);
    octaveEdit = new QLineEdit(locationsTab);
    locationsGrid->addWidget(octaveEdit,4,1);
    QPushButton *OctaveButt = new QPushButton("...");
    locationsGrid->addWidget(OctaveButt, 4, 2);
    connect(OctaveButt, SIGNAL(clicked()), SLOT(slotOctaveDirBrowse()));


    // the pathsTableWidget displays the path list
    pathsTableWidget = new QTableWidget(locationsTab);
    pathsTableWidget->setColumnCount(1);

    QTableWidgetItem *pitem1 = new QTableWidgetItem();

    pathsTableWidget->setHorizontalHeaderItem(0, pitem1);

    pitem1->setText(tr("Subcircuit Search Path List"));

    pathsTableWidget->horizontalHeader()->setStretchLastSection(true);
    pathsTableWidget->verticalHeader()->hide();
    // allow multiple items to be selected
    pathsTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(pathsTableWidget, SIGNAL(cellClicked(int,int)), SLOT(slotPathTableClicked(int,int)));
    locationsGrid->addWidget(pathsTableWidget,5,0,3,2);

    QPushButton *AddPathButt = new QPushButton("Add Path");
    locationsGrid->addWidget(AddPathButt, 5, 2);
    connect(AddPathButt, SIGNAL(clicked()), SLOT(slotAddPath()));

    QPushButton *AddPathSubFolButt = new QPushButton("Add Path With SubFolders");
    locationsGrid->addWidget(AddPathSubFolButt, 6, 2);
    connect(AddPathSubFolButt, SIGNAL(clicked()), SLOT(slotAddPathWithSubFolders()));

    QPushButton *RemovePathButt = new QPushButton("Remove Path");
    locationsGrid->addWidget(RemovePathButt , 7, 2);
    connect(RemovePathButt, SIGNAL(clicked()), SLOT(slotRemovePath()));

    // create a copy of the current global path list
    currentPaths = QStringList(swieePathList);
    makePathTable();

    t->addTab(locationsTab, tr("Locations"));

    // ...........................................................
    // buttons on the bottom of the dialog (independent of the TabWidget)

    QHBoxLayout *Butts = new QHBoxLayout();
    Butts->setSpacing(3);
    Butts->setMargin(3);
    all->addLayout(Butts);

    QPushButton *OkButt = new QPushButton(tr("OK"));
    Butts->addWidget(OkButt);
    connect(OkButt, SIGNAL(clicked()), SLOT(slotOK()));
    QPushButton *ApplyButt = new QPushButton(tr("Apply"));
    Butts->addWidget(ApplyButt);
    connect(ApplyButt, SIGNAL(clicked()), SLOT(slotApply()));
    QPushButton *CancelButt = new QPushButton(tr("Cancel"));
    Butts->addWidget(CancelButt);
    connect(CancelButt, SIGNAL(clicked()), SLOT(reject()));
    QPushButton *DefaultButt = new QPushButton(tr("Default Values"));
    Butts->addWidget(DefaultButt);
    connect(DefaultButt, SIGNAL(clicked()), SLOT(slotDefaultValues()));



    OkButt->setDefault(true);

    // ...........................................................
    // fill the fields with the Swiee-Properties
    Font  = SwieeSettings.font;
    FontButton->setText(Font.toString());
    BGColorButton->setPaletteBackgroundColor(SwieeSettings.BGColor);
    undoNumEdit->setText(QString::number(SwieeSettings.maxUndo));
    editorEdit->setText(SwieeSettings.Editor);
    checkWiring->setChecked(SwieeSettings.NodeWiring);

    for(int z=LanguageCombo->count()-1; z>=0; z--)
        if(LanguageCombo->text(z).section('(',1,1).remove(')') == SwieeSettings.Language)
            LanguageCombo->setCurrentItem(z);

    /*! Load paths from settings */
    homeEdit->setText(SwieeSettings.SwieeHomeDir.canonicalPath());
    admsXmlEdit->setText(SwieeSettings.AdmsXmlBinDir.canonicalPath());
    ascoEdit->setText(SwieeSettings.AscoBinDir.canonicalPath());
    octaveEdit->setText(SwieeSettings.OctaveBinDir.canonicalPath());


    resize(300, 200);
}

SwieeSettingsDialog::~SwieeSettingsDialog()
{
    delete all;
    delete val200;
    delete Validator;
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotAddFileType()
{
    QModelIndexList indexes = fileTypesTableWidget->selectionModel()->selection().indexes();
    if (indexes.count())
    {
        fileTypesTableWidget->item(indexes.at(0).row(),0)->setText(Input_Suffix->text());
        fileTypesTableWidget->item(indexes.at(0).row(),1)->setText(Input_Program->text());
        fileTypesTableWidget->selectionModel()->clear();
        return;
    }

    //check before append
    for(int r=0; r < fileTypesTableWidget->rowCount(); r++)
        if(fileTypesTableWidget->item(r,0)->text() == Input_Suffix->text())
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("This suffix is already registered!"));
            return;
        }

    int row = fileTypesTableWidget->rowCount();
    fileTypesTableWidget->setRowCount(row+1);

    QTableWidgetItem *newSuffix = new QTableWidgetItem(QString(Input_Suffix->text()));
    newSuffix->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    fileTypesTableWidget->setItem(row, 0, newSuffix);

    QTableWidgetItem *newProgram = new QTableWidgetItem(Input_Program->text());
    fileTypesTableWidget->setItem(row, 1, newProgram);
    newProgram->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    Input_Suffix->setFocus();
    Input_Suffix->clear();
    Input_Program->clear();
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotRemoveFileType()
{
    QModelIndexList indexes = fileTypesTableWidget->selectionModel()->selection().indexes();
    if (indexes.count())
    {
        fileTypesTableWidget->removeRow(indexes.at(0).row());
        fileTypesTableWidget->selectionModel()->clear();
        Input_Suffix->setText("");
        Input_Program->setText("");
        return;
    }
}

// -----------------------------------------------------------
// Applies any changed settings and closes the dialog
void SwieeSettingsDialog::slotOK()
{
    slotApply();
    accept();
}

// -----------------------------------------------------------
// Applies any changed settings
void SwieeSettingsDialog::slotApply()
{
    bool changed = false;

    if(SwieeSettings.BGColor != BGColorButton->paletteBackgroundColor())
    {
        SwieeSettings.BGColor = BGColorButton->paletteBackgroundColor();

        int No=0;
        QWidget *w;
        while((w=App->DocumentTab->page(No++)) != 0)
            if(w->inherits("QTextEdit"))
                ((TextDoc*)w)->viewport()->setPaletteBackgroundColor(
                    SwieeSettings.BGColor);
            else
                ((Schematic*)w)->viewport()->setPaletteBackgroundColor(
                    SwieeSettings.BGColor);
        changed = true;
    }

    SwieeSettings.font=Font;

    SwieeSettings.Language =
        LanguageCombo->currentText().section('(',1,1).remove(')');

    if(SwieeSettings.Comment != ColorComment->paletteForegroundColor())
    {
        SwieeSettings.Comment = ColorComment->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.String != ColorString->paletteForegroundColor())
    {
        SwieeSettings.String = ColorString->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Integer != ColorInteger->paletteForegroundColor())
    {
        SwieeSettings.Integer = ColorInteger->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Real != ColorReal->paletteForegroundColor())
    {
        SwieeSettings.Real = ColorReal->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Character != ColorCharacter->paletteForegroundColor())
    {
        SwieeSettings.Character = ColorCharacter->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Type != ColorDataType->paletteForegroundColor())
    {
        SwieeSettings.Type = ColorDataType->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Attribute != ColorAttribute->paletteForegroundColor())
    {
        SwieeSettings.Attribute = ColorAttribute->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Directive != ColorDirective->paletteForegroundColor())
    {
        SwieeSettings.Directive = ColorDirective->paletteForegroundColor();
        changed = true;
    }
    if(SwieeSettings.Task != ColorTask->paletteForegroundColor())
    {
        SwieeSettings.Task = ColorTask->paletteForegroundColor();
        changed = true;
    }

    bool ok;
    if(SwieeSettings.maxUndo != undoNumEdit->text().toUInt(&ok))
    {
        SwieeSettings.maxUndo = undoNumEdit->text().toInt(&ok);
        changed = true;
    }
    if(SwieeSettings.Editor != editorEdit->text())
    {
        SwieeSettings.Editor = editorEdit->text();
        changed = true;
    }
    if(SwieeSettings.NodeWiring != (unsigned)checkWiring->isChecked())
    {
        SwieeSettings.NodeWiring = checkWiring->isChecked();
        changed = true;
    }

    SwieeSettings.FileTypes.clear();
    for (int row=0; row < fileTypesTableWidget->rowCount(); row++)
    {
        SwieeSettings.FileTypes.append(fileTypesTableWidget->item(row,0)->text()
                                      +"/"+
                                      fileTypesTableWidget->item(row,1)->text());
    }

    /*! Update SwieeSettings, tool paths */
    SwieeSettings.SwieeHomeDir = homeEdit->text();
    SwieeSettings.AdmsXmlBinDir = admsXmlEdit->text();
    SwieeSettings.AscoBinDir = ascoEdit->text();
    SwieeSettings.OctaveBinDir = octaveEdit->text();

    SwieeSettings.IgnoreFutureVersion = checkLoadFromFutureVersions->isChecked();

    saveApplSettings(App);  // also sets the small and large font

    if(changed)
    {
        App->readProjects();
        App->readProjectFiles();
        App->repaint();
    }

    // update the schenatic filelist hash
    SwieeMain->updatePathList(currentPaths);
    SwieeMain->updateSchNameHash();
    SwieeMain->updateSpiceNameHash();

}


// -----------------------------------------------------------
void SwieeSettingsDialog::slotFontDialog()
{
    bool ok;
    QFont tmpFont = QFontDialog::getFont(&ok, Font, this);
    if(ok)
    {
        Font = tmpFont;
        FontButton->setText(Font.toString());
    }
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotBGColorDialog()
{
    QColor c = QColorDialog::getColor(
                   BGColorButton->paletteBackgroundColor(), this);
    if(c.isValid())
        BGColorButton->setPaletteBackgroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotDefaultValues()
{
    //Font = QFont("Helvetica", 12);
    FontButton->setText(Font.toString());
    LanguageCombo->setCurrentItem(0);
    BGColorButton->setPaletteBackgroundColor(QColor(255,250,225));

    ColorComment->setPaletteForegroundColor(Qt::gray);
    ColorString->setPaletteForegroundColor(Qt::red);
    ColorInteger->setPaletteForegroundColor(Qt::blue);
    ColorReal->setPaletteForegroundColor(Qt::darkMagenta);
    ColorCharacter->setPaletteForegroundColor(Qt::magenta);
    ColorDataType->setPaletteForegroundColor(Qt::darkRed);
    ColorAttribute->setPaletteForegroundColor(Qt::darkCyan);
    ColorDirective->setPaletteForegroundColor(Qt::darkCyan);
    ColorTask->setPaletteForegroundColor(Qt::darkRed);

    undoNumEdit->setText("20");
    editorEdit->setText(SwieeSettings.BinDir + "qucs");
    checkWiring->setChecked(false);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorComment()
{
    QColor c = QColorDialog::getColor(
                   ColorComment->paletteForegroundColor(), this);
    if(c.isValid())
        ColorComment->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorString()
{
    QColor c = QColorDialog::getColor(
                   ColorString->paletteForegroundColor(), this);
    if(c.isValid())
        ColorString->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorInteger()
{
    QColor c = QColorDialog::getColor(
                   ColorInteger->paletteForegroundColor(), this);
    if(c.isValid())
        ColorInteger->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorReal()
{
    QColor c = QColorDialog::getColor(
                   ColorReal->paletteForegroundColor(), this);
    if(c.isValid())
        ColorReal->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorCharacter()
{
    QColor c = QColorDialog::getColor(
                   ColorCharacter->paletteForegroundColor(), this);
    if(c.isValid())
        ColorCharacter->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorDataType()
{
    QColor c = QColorDialog::getColor(
                   ColorDataType->paletteForegroundColor(), this);
    if(c.isValid())
        ColorDataType->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorAttribute()
{
    QColor c = QColorDialog::getColor(
                   ColorAttribute->paletteForegroundColor(), this);
    if(c.isValid())
        ColorAttribute->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorDirective()
{
    QColor c = QColorDialog::getColor(
                   ColorDirective->paletteForegroundColor(), this);
    if(c.isValid())
        ColorDirective->setPaletteForegroundColor(c);
}

// -----------------------------------------------------------
void SwieeSettingsDialog::slotColorTask()
{
    QColor c = QColorDialog::getColor(
                   ColorTask->paletteForegroundColor(), this);
    if(c.isValid())
        ColorTask->setPaletteForegroundColor(c);
}

void SwieeSettingsDialog::slotTableClicked(int row, int col)
{
    Input_Suffix->setText(fileTypesTableWidget->item(row,0)->text());
    Input_Program->setText(fileTypesTableWidget->item(row,1)->text());
}

// -----------------------------------------------------------
// The locations tab slots

void SwieeSettingsDialog::slotHomeDirBrowse()
{
    QFileDialog fileDialog( this, tr("Select the home directory"), homeEdit->text() );
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.exec();
    homeEdit->setText(fileDialog.selectedFile());
}

void SwieeSettingsDialog::slotAdmsXmlDirBrowse()
{
    QFileDialog fileDialog( this, tr("Select the admsXml bin directory"), admsXmlEdit->text() );
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.exec();
    admsXmlEdit->setText(fileDialog.selectedFile());
}

void SwieeSettingsDialog::slotAscoDirBrowse()
{
    QFileDialog fileDialog( this, tr("Select the ASCO bin directory"), ascoEdit->text() );
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.exec();
    ascoEdit->setText(fileDialog.selectedFile());
}

void SwieeSettingsDialog::slotOctaveDirBrowse()
{
    QFileDialog fileDialog( this, tr("Select the octave bin directory"), octaveEdit->text() );
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.exec();
    octaveEdit->setText(fileDialog.selectedFile());
}

void SwieeSettingsDialog::slotPathTableClicked(int row, int col)
{
    //Input_Path->setText(fileTypesTableWidget->item(row,0)->text());
}

void SwieeSettingsDialog::slotAddPath()
{
    QFileDialog fileDialog( this, tr("Select a directory"), SwieeSettings.SwieeWorkDir.canonicalPath());
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);

    if (fileDialog.exec())
    {
        currentPaths.append(fileDialog.selectedFile());
        // reconstruct the table again
        makePathTable();
    }
    else
    {
        // user cancelled
    }
}

void SwieeSettingsDialog::slotAddPathWithSubFolders()
{
    // open a file dialog to select the top level directory
    QFileDialog fileDialog( this, tr("Select a directory"), SwieeSettings.SwieeWorkDir.canonicalPath());
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);

    QString path;
    QFileInfo pathfinfo;

    if (fileDialog.exec())
    {
        // Iterate through the directories
        QDirIterator pathIter(fileDialog.selectedFile(), QDirIterator::Subdirectories);
        while (pathIter.hasNext())
        {
            path = pathIter.next();
            pathfinfo = pathIter.fileInfo();

            if (pathfinfo.isDir() && !pathfinfo.isSymLink() && !path.endsWith("."))
            {
                QDir thispath(path);
                currentPaths.append(thispath.canonicalPath());
            }
        }
        makePathTable();
    }
    else
    {
        // user cancelled
    }


}

void SwieeSettingsDialog::slotRemovePath()
{
    //Input_Path->setText(fileTypesTableWidget->item(row,0)->text());
    // get the selected items from the table
    QList<QTableWidgetItem *> selectedPaths = pathsTableWidget->selectedItems();

    foreach (QTableWidgetItem * item, selectedPaths)
    {
        QString path = item->text();
        //removedPaths.append(path);
        int pathind = currentPaths.indexOf(path,0);
        if (pathind != -1)
        {
            currentPaths.removeAt(pathind);
        }
    }

    makePathTable();
}

// makePathTable()
//
// Reconstructs the table containing the list of search paths
// in the locations tab
void SwieeSettingsDialog::makePathTable()
{
    // remove all the paths from the table if present
    pathsTableWidget->clearContents();
    pathsTableWidget->setRowCount(0);

    // fill listview with the list of paths
    foreach (QString pathstr, currentPaths)
    {
        int row = pathsTableWidget->rowCount();
        pathsTableWidget->setRowCount(row+1);
        QTableWidgetItem *path = new QTableWidgetItem(pathstr);
        path->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        pathsTableWidget->setItem(row, 0, path);
    }
}
