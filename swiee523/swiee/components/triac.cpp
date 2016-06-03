#include "triac.h"


Triac::Triac()
{
  Description = QObject::tr("triac (bidirectional thyristor)");

  Lines.append(new Line(  0,-30,  0, -6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(  0, 30,  0,  6,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-18,  6, 18,  6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-18, -6, 18, -6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -9,  6,-18, -6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -9,  6,  0, -6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(  9, -6,  0,  6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(  9, -6, 18,  6,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-13, 10, -9,  6,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30, 10,-13, 10,QPen(Qt::darkBlue,2)));

  Ports.append(new Port(  0,-30));
  Ports.append(new Port(  0, 30));
  Ports.append(new Port(-30, 10));

  x1 = -30; y1 = -30;
  x2 =  20; y2 =  30;

  tx = x2+4;
  ty = y1+4;
  Model = "Triac";
  Name  = "D";

  Props.append(new Property("Vbo", "400 V", false,
	QObject::tr("(bidirectional) breakover voltage")));
  Props.append(new Property("Igt", "50 uA", true,
	QObject::tr("(bidirectional) gate trigger current")));
  Props.append(new Property("Cj0", "10 pF", false,
	QObject::tr("parasitic capacitance")));
  Props.append(new Property("Is", "1e-10 A", false,
	QObject::tr("saturation current")));
  Props.append(new Property("N", "2", false,
	QObject::tr("emission coefficient")));
  Props.append(new Property("Ri", "10 Ohm", false,
	QObject::tr("intrinsic junction resistance")));
  Props.append(new Property("Rg", "5 Ohm", false,
	QObject::tr("gate resistance")));
  Props.append(new Property("Temp", "26.85", false,
	QObject::tr("simulation temperature")));
}

Component* Triac::newOne()
{
  return new Triac();
}

Element* Triac::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Triac");
  BitmapFile = (char *) "triac";

  if(getNewOne)  return new Triac();
  return 0;
}
