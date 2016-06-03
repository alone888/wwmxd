#ifndef _LABEL_H_
#define _LABEL_H_
#include <qlabel.h>
#include <QWidget> 
#include "qcoreevent.h"
#include "qevent.h"
#include "qtimer.h"

class swieeLabel:public QLabel
{
	Q_OBJECT
public:
	explicit swieeLabel(QWidget *parent = 0);
	explicit swieeLabel(char *, QWidget *parent);
	~swieeLabel();
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	bool mouse_press; 
	int clicked_num;
	QTimer* timer;
private slots:
	void SlotTimerOut();
signals:  
	void swieeLabelClicked();  
	void swieeLabelDoubleClicked();  
};


#endif //_LABEL_H_