#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

class SwieeApp;
class QLineEdit;
class QCheckBox;
class QGroupBox;
class QPushButton;


class SearchDialog : public QDialog  {
Q_OBJECT
public:
  SearchDialog(SwieeApp*);
 ~SearchDialog();

  void initSearch(bool replace=false);
  void searchText(bool, int);

private slots:
  void slotSearch();

private:
  SwieeApp *App;
  QLineEdit *SearchEdit, *ReplaceEdit;
  QCheckBox *PositionBox, *CaseBox, *WordBox, *BackwardBox, *AskBox;
  QGroupBox *ReplaceGroup;
};

#endif
