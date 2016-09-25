#include "wDelay5.h"
#include "./module.h"

wDelay5::wDelay5()
{
	Description = QObject::tr("ideal amplifier");

	//Lines.append(new Line(-16,-20,-16,80,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-30,-35,-30, 35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30,-35, 30, -35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30, 35, 30,  35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(30, -35, 30,  35,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-50,  0,-30,  0,QPen(Qt::darkBlue,2)));
	Lines.append(new Line( 30,  0, 50,  0,QPen(Qt::darkBlue,2)));



	Texts.append(new Text(-17,-21, "(s-1)", Qt::darkBlue, 12.0));
	Lines.append(new Line(-20,  0, 20,  0,QPen(Qt::darkBlue,2)));
	Texts.append(new Text(-20, 4, "s(s+1)", Qt::darkBlue, 12.0));


	Ports.append(new Port(-50,  0));
	Ports.append(new Port( 50,  0));

	x1 = -50; y1 = -45;
	x2 =  50; y2 =  45;

	tx = x1+4;
	ty = y2+4;
	Model = "Amp";
	Name  = "X";

	Props.append(new Property("G", "10", true,
		QObject::tr("voltage gain")));
	Props.append(new Property("Z1", "50 Ohm", false,
		QObject::tr("reference impedance of input port")));
	Props.append(new Property("Z2", "50 Ohm", false,
		QObject::tr("reference impedance of output port")));
	Props.append(new Property("NF", "0 dB", false,
		QObject::tr("noise figure")));
}

wDelay5::~wDelay5()
{
}

Component* wDelay5::newOne()
{
	return new wDelay5();
}

Element* wDelay5::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
	Name = GB2312("��ʱ��");
	BitmapFile = (char *) "z6";

	if(getNewOne)  return new wDelay5();
	return 0;
}
