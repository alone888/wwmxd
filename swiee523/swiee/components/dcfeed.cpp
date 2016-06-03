#include "dcfeed.h"


dcFeed::dcFeed()
{
  Description = QObject::tr("dc feed");

  Arcs.append(new Arc(-17, -6, 12, 12,  0, 16*180,QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc( -6, -6, 12, 12,  0, 16*180,QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(  5, -6, 12, 12,  0, 16*180,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30,  0,-17,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 17,  0, 30,  0,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-23,-13, 23,-13,QPen(Qt::darkBlue,1)));
  Lines.append(new Line(-23, 13, 23, 13,QPen(Qt::darkBlue,1)));
  Lines.append(new Line(-23,-13,-23, 13,QPen(Qt::darkBlue,1)));
  Lines.append(new Line( 23,-13, 23, 13,QPen(Qt::darkBlue,1)));

  Ports.append(new Port(-30,  0));
  Ports.append(new Port( 30,  0));

  x1 = -30; y1 = -15;
  x2 =  30; y2 =  16;

  tx = x1+4;
  ty = y2+4;
  Model = "DCFeed";
  Name  = "L";

  Props.append(new Property("L", "1 uH", false,
	QObject::tr("for transient simulation: inductance in Henry")));
}

dcFeed::~dcFeed()
{
}

Component* dcFeed::newOne()
{
  return new dcFeed();
}

Element* dcFeed::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("dc Feed");
  BitmapFile = (char *) "dcfeed";

  if(getNewOne)  return new dcFeed();
  return 0;
}
