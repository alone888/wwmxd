#include "swieelabel.h"

swieeLabel::swieeLabel( QWidget *parent /*= 0*/ ):QLabel(parent)
{
	mouse_press = false;
	clicked_num = 0;
	timer = new QTimer(this);
	//connect(timer,SIGNAL(timeout()),this,SLOT(SlotTimerOut()));   
}

swieeLabel::swieeLabel( char *name, QWidget *parent /*= 0*/ ):QLabel(name, parent)
{
	mouse_press = false;
	clicked_num = 0;
	timer = new QTimer(this);
	//connect(timer,SIGNAL(timeout()),this,SLOT(SlotTimerOut()));   
}

swieeLabel::~swieeLabel()
{
	delete timer;
}

void swieeLabel::mousePressEvent( QMouseEvent *event )
{
	if (event->button() == Qt::LeftButton)
	{
		mouse_press = true;
	}
}

void swieeLabel::mouseReleaseEvent( QMouseEvent *event )
{
	if(mouse_press  && this->rect().contains(event->pos()))
	{
		clicked_num++;
		mouse_press = false;
		timer->start(300);
	}
}

void swieeLabel::SlotTimerOut()
{
	if (clicked_num == 1)
	{
		emit swieeLabelClicked();
	}
	if (clicked_num >= 2)
	{
		emit swieeLabelDoubleClicked();
	}
	clicked_num = 0;
	timer->stop();
}
