#include "gyrator.h"


Gyrator::Gyrator()
{
  Description = QObject::tr("gyrator (impedance inverter)");

  Arcs.append(new Arc(  3, -9, 18, 18, 16*90, 16*180,QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(-21, -9, 18, 18,16*270, 16*180,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-30,-30,-12,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30, 30,-12, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 12,-30, 30,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 12, 30, 30, 30,QPen(Qt::darkBlue,2)));

  Lines.append(new Line( 12,-30, 12, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-12,-30,-12, 30,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-22,-22, 22,-22,QPen(Qt::darkGray,1)));
  Lines.append(new Line( 22,-22, 22, 22,QPen(Qt::darkGray,1)));
  Lines.append(new Line( 22, 22,-22, 22,QPen(Qt::darkGray,1)));
  Lines.append(new Line(-22, 22,-22,-22,QPen(Qt::darkGray,1)));

  Ports.append(new Port(-30,-30));
  Ports.append(new Port( 30,-30));
  Ports.append(new Port( 30, 30));
  Ports.append(new Port(-30, 30));

  x1 = -30; y1 = -30;
  x2 =  30; y2 =  30;

  tx = x1+4;
  ty = y2+4;
  Model = "Gyrator";
  Name  = "X";

  Props.append(new Property("R", "50 Ohm", true,
		QObject::tr("gyrator ratio")));
  Props.append(new Property("Zref", "50 Ohm", false,
		QObject::tr("reference impedance")));
}

Gyrator::~Gyrator()
{
}

Component* Gyrator::newOne()
{
  return new Gyrator();
}

Element* Gyrator::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Gyrator");
  BitmapFile = (char *) "gyrator";

  if(getNewOne)  return new Gyrator();
  return 0;
}
