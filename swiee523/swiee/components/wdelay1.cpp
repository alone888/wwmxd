#include "wDelay1.h"
#include "./module.h"

wDelay1::wDelay1()
{
	Description = QObject::tr("ideal amplifier");

	//Lines.append(new Line(-16,-20,-16,80,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-30,-35,-30, 35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30,-35, 30, -35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-30, 35, 30,  35,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(30, -35, 30,  35,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-50,  0,-30,  0,QPen(Qt::darkBlue,2)));
	Lines.append(new Line( 30,  0, 50,  0,QPen(Qt::darkBlue,2)));

	

	Texts.append(new Text(-5,-22, "1", Qt::darkBlue, 16.0));
	Texts.append(new Text(-5,-10, "-", Qt::darkBlue, 16.0));
	Texts.append(new Text(-5, 0, "S", Qt::darkBlue, 16.0));


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

wDelay1::~wDelay1()
{
}

Component* wDelay1::newOne()
{
	return new wDelay1();
}

Element* wDelay1::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
	Name = GB2312("»ý·ÖÆ÷");
	BitmapFile = (char *) "z2";

	if(getNewOne)  return new wDelay1();
	return 0;
}
