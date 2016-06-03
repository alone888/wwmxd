#ifndef ARROWDIALOG_H
#define ARROWDIALOG_H

#include <QDialog>

class QLineEdit;
class QGridLayout;
class QPushButton;
class QComboBox;
class QIntValidator;


class ArrowDialog : public QDialog  {
Q_OBJECT
public:
  ArrowDialog(QWidget *parent=0, const char *name=0);
 ~ArrowDialog();

  void SetComboBox(Qt::PenStyle);

private slots:
  void slotSetColor();
  void slotSetStyle(int);

public:
  // TODO: I'm not sure whether declaring all this as public is really a good idea :-(
  // data encapsulation => make this stuff private & provide getter & setter methods...
  QLineEdit    *LineWidth, *HeadWidth, *HeadLength;
  QPushButton  *ColorButt;
  QComboBox    *StyleBox, *ArrowStyleBox;
  Qt::PenStyle LineStyle;

  QGridLayout   *all;
  QIntValidator *val100;
};

#endif
