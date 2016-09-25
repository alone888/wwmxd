#include "wDelay3.h"
#include "./module.h"

wDelay3::wDelay3()
{
	Description = QObject::tr("ideal amplifier");

	Lines.append(new Line(-30,-35,-30, 35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30,-35, 30, -35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30, 35, 30,  35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(30, -35, 30,  35,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-50,  0,-30,  0,QPen(Qt::darkBlue,2)));
	Lines.append(new Line( 30,  0, 50,  0,QPen(Qt::darkBlue,2)));


	Arcs.append(new Arc(-12, -12, 24,  24, 0,16*360,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-12*0.7,  -12*0.7, 12*0.7,  12*0.7,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(12*0.7,  -12*0.7, -12*0.7,  12*0.7,QPen(Qt::darkBlue,2)));


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

wDelay3::~wDelay3()
{
}

Component* wDelay3::newOne()
{
	return new wDelay3();
}

Element* wDelay3::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
	Name = GB2312("»ìºÏÆ÷");
	BitmapFile = (char *) "z4";

	if(getNewOne)  return new wDelay3();
	return 0;
}
