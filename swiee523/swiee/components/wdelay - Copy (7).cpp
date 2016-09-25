#include "wDelay.h"
#include "./module.h"

wDelay::wDelay()
{
	Description = QObject::tr("ideal amplifier");

	//Lines.append(new Line(-16,-20,-16,80,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-16,-20,-16, 20,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-16,-20, 16, -20,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(-16, 20, 16,  20,QPen(Qt::darkBlue,2)));
	Lines.append(new Line(16, -20, 16,  20,QPen(Qt::darkBlue,2)));

	Lines.append(new Line(-30,  0,-16,  0,QPen(Qt::darkBlue,2)));
	Lines.append(new Line( 16,  0, 30,  0,QPen(Qt::darkBlue,2)));

	Texts.append(new Text(-10,-10, "Z", Qt::darkBlue, 16.0));

	Texts.append(new Text(1,-12, "-1", Qt::darkBlue, 8.0));


	Ports.append(new Port(-30,  0));
	Ports.append(new Port( 30,  0));

	x1 = -30; y1 = -23;
	x2 =  30; y2 =  23;

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

wDelay::~wDelay()
{
}

Component* wDelay::newOne()
{
	return new wDelay();
}

Element* wDelay::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
	Name = GB2312("��ʱ��");
	BitmapFile = (char *) "z1";

	if(getNewOne)  return new wDelay();
	return 0;
}
