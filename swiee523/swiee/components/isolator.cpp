#include "isolator.h"


Isolator::Isolator()
{
  Description = QObject::tr("isolator");

  Lines.append(new Line( -8,  0,  8,  0,QPen(Qt::darkBlue,3)));
  Lines.append(new Line(  8,  0,  0, -5,QPen(Qt::darkBlue,3)));
  Lines.append(new Line(  8,  0,  0,  5,QPen(Qt::darkBlue,3)));

  Lines.append(new Line(-14,-14, 14,-14,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-14, 14, 14, 14,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-14,-14,-14, 14,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 14,-14, 14, 14,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-30,  0,-14,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 14,  0, 30,  0,QPen(Qt::darkBlue,2)));

  Ports.append(new Port(-30,  0));
  Ports.append(new Port( 30,  0));

  x1 = -30; y1 = -17;
  x2 =  30; y2 =  17;

  tx = x1+4;
  ty = y2+4;
  Model = "Isolator";
  Name  = "X";

  Props.append(new Property("Z1", "50 Ohm", false,
		QObject::tr("reference impedance of input port")));
  Props.append(new Property("Z2", "50 Ohm", false,
		QObject::tr("reference impedance of output port")));
  Props.append(new Property("Temp", "26.85", false,
		QObject::tr("simulation temperature in degree Celsius")));
}

Isolator::~Isolator()
{
}

Component* Isolator::newOne()
{
  return new Isolator();
}

Element* Isolator::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Isolator");
  BitmapFile = (char *) "z10";

  if(getNewOne)  return new Isolator();
  return 0;
}
