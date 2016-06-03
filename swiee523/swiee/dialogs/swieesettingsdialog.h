#ifndef SWIEESETTINGSDIALOG_H
#define SWIEESETTINGSDIALOG_H

#include "swiee.h"

#include <QDialog>
#include <QFont>
#include <QRegExp>
#include <QVBoxLayout>

class QListView;
//class Q3ListViewItem;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
class QPushButton;
class QComboBox;
class QIntValidator;
class QRegExpValidator;
class QTableView;
class QStandardItemModel;

class SwieeSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SwieeSettingsDialog(SwieeApp *parent=0, const char *name=0);
    ~SwieeSettingsDialog();

private slots:
    void slotOK();
    void slotApply();
    void slotFontDialog();
    void slotBGColorDialog();
    void slotDefaultValues();
    void slotAddFileType();
    void slotRemoveFileType();
    void slotColorComment();
    void slotColorString();
    void slotColorInteger();
    void slotColorReal();
    void slotColorCharacter();
    void slotColorDataType();
    void slotColorAttribute();
    void slotColorDirective();
    void slotColorTask();
    void slotTableClicked(int,int);
    void slotPathTableClicked(int,int);

    void slotHomeDirBrowse();
    void slotAdmsXmlDirBrowse();
    void slotAscoDirBrowse();
    void slotOctaveDirBrowse();

    void slotAddPath();
    void slotAddPathWithSubFolders();
    void slotRemovePath();


public:
    SwieeApp *App;

    QFont Font;
    QCheckBox *checkWiring, *checkLoadFromFutureVersions;
    QComboBox *LanguageCombo;
    QPushButton *FontButton, *BGColorButton;
    QLineEdit *undoNumEdit, *editorEdit, *Input_Suffix, *Input_Program,
              *homeEdit, *admsXmlEdit, *ascoEdit, *octaveEdit;
    QTableWidget *fileTypesTableWidget, *pathsTableWidget;
    QStandardItemModel *model;
    QPushButton *ColorComment, *ColorString, *ColorInteger,
                *ColorReal, *ColorCharacter, *ColorDataType, *ColorAttribute,
                *ColorDirective, *ColorTask;

    QVBoxLayout *all;
    QIntValidator *val200;
    QRegExp Expr;
    QRegExpValidator *Validator;

private:
    QStringList currentPaths;

private:
    void makePathTable();

};

#endif
