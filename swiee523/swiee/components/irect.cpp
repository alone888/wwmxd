#include "irect.h"


iRect::iRect()
{
  Description = QObject::tr("ideal rectangle current source");

  Arcs.append(new Arc(-12,-12, 24, 24,  0, 16*360,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30,  0,-12,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 30,  0, 12,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -7,  0,  7,  0,QPen(Qt::darkBlue,3)));
  Lines.append(new Line(  6,  0,  0, -4,QPen(Qt::darkBlue,3)));
  Lines.append(new Line(  6,  0,  0,  4,QPen(Qt::darkBlue,3)));

  // little rectangle symbol
  Lines.append(new Line( 19,  5, 19,  7,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 13,  7, 19,  7,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 13,  7, 13, 11,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 13, 11, 19, 11,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 19, 11, 19, 15,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 13, 15, 19, 15,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 13, 15, 13, 17,QPen(Qt::darkBlue,2)));

  Ports.append(new Port( 30,  0));
  Ports.append(new Port(-30,  0));

  x1 = -30; y1 = -14;
  x2 =  30; y2 =  20;

  tx = x1+4;
  ty = y2+4;
  Model = "Irect";
  Name  = "I";

  Props.append(new Property("I", "1 mA", true,
		QObject::tr("current at high pulse")));
  Props.append(new Property("TH", "1 ms", true,
		QObject::tr("duration of high pulses")));
  Props.append(new Property("TL", "1 ms", true,
		QObject::tr("duration of low pulses")));
  Props.append(new Property("Tr", "1 ns", false,
		QObject::tr("rise time of the leading edge")));
  Props.append(new Property("Tf", "1 ns", false,
		QObject::tr("fall time of the trailing edge")));
  Props.append(new Property("Td", "0 ns", false,
		QObject::tr("initial delay time")));

  rotate();  // fix historical flaw
}

iRect::~iRect()
{
}

Component* iRect::newOne()
{
  return new iRect();
}

Element* iRect::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Rectangle Current");
  BitmapFile = (char *) "irect";

  if(getNewOne)  return new iRect();
  return 0;
}
