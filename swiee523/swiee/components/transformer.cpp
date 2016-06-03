#include "transformer.h"


Transformer::Transformer()
{
  Description = QObject::tr("ideal transformer");

  Arcs.append(new Arc(-16,-18,12,12, 16*270,16*180, QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(-16, -6,12,12, 16*270,16*180, QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(-16,  6,12,12, 16*270,16*180, QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(  4,-18,12,12,  16*90,16*180, QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(  4, -6,12,12,  16*90,16*180, QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc(  4,  6,12,12,  16*90,16*180, QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-10,-18,-10,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-10,-30,-30,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 10,-18, 10,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 10,-30, 30,-30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-10, 18,-10, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-10, 30,-30, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 10, 18, 10, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 10, 30, 30, 30,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -1,-20, -1, 20,QPen(Qt::darkBlue,1)));
  Lines.append(new Line(  1,-20,  1, 20,QPen(Qt::darkBlue,1)));

  Texts.append(new Text(-21, -18,"T"));
  Arcs.append(new Arc(-21,-24,  5,  5,  0, 16*360,QPen(Qt::darkBlue,2)));
  Arcs.append(new Arc( 15,-24,  5,  5,  0, 16*360,QPen(Qt::darkBlue,2)));


  Ports.append(new Port(-30,-30));
  Ports.append(new Port( 30,-30));
  Ports.append(new Port( 30, 30));
  Ports.append(new Port(-30, 30));

  x1 = -33; y1 = -34;
  x2 =  33; y2 =  34;

  tx = x1+4;
  ty = y2+4;
  Model = "Tr";
  Name  = "Tr";

  Props.append(new Property("T", "1", true,
		QObject::tr("voltage transformation ratio")));
}

Transformer::~Transformer()
{
}

Component* Transformer::newOne()
{
  return new Transformer();
}

Element* Transformer::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Transformer");
  BitmapFile = (char *) "transformer";

  if(getNewOne)  return new Transformer();
  return 0;
}
