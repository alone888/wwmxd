#include "dcblock.h"

dcBlock::dcBlock()
{
  Description = QObject::tr("dc block");

  Lines.append(new Line(- 4,-11, -4, 11,QPen(Qt::darkBlue,4)));
  Lines.append(new Line(  4,-11,  4, 11,QPen(Qt::darkBlue,4)));
  Lines.append(new Line(-30,  0, -4,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(  4,  0, 30,  0,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-23,-14, 23,-14,QPen(Qt::darkBlue,1)));
  Lines.append(new Line(-23, 14, 23, 14,QPen(Qt::darkBlue,1)));
  Lines.append(new Line(-23,-14,-23, 14,QPen(Qt::darkBlue,1)));
  Lines.append(new Line( 23,-14, 23, 14,QPen(Qt::darkBlue,1)));

  Ports.append(new Port(-30,  0));
  Ports.append(new Port( 30,  0));

  x1 = -30; y1 = -16;
  x2 =  30; y2 =  17;

  tx = x1+4;
  ty = y2+4;
  Model = "DCBlock";
  Name  = "C";

  Props.append(new Property("C", "1 uF", false,
	QObject::tr("for transient simulation: capacitance in Farad")));
}

dcBlock::~dcBlock()
{
}

Component* dcBlock::newOne()
{
  return new dcBlock();
}

Element* dcBlock::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("dc Block");
  BitmapFile = (char *) "dcblock";

  if(getNewOne)  return new dcBlock();
  return 0;
}
