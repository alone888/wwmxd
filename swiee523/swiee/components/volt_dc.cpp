#include "volt_dc.h"


Volt_dc::Volt_dc()
{
  Description = QObject::tr("ideal dc voltage source");

  Lines.append(new Line(  4,-13,  4, 13,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -4, -6, -4,  6,QPen(Qt::darkBlue,4)));
  Lines.append(new Line( 30,  0,  4,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -4,  0,-30,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 11,  5, 11, 11,QPen(Qt::red,1)));
  Lines.append(new Line( 14,  8,  8,  8,QPen(Qt::red,1)));
  Lines.append(new Line(-11,  5,-11, 11,QPen(Qt::black,1)));

  Ports.append(new Port( 30,  0));
  Ports.append(new Port(-30,  0));

  x1 = -30; y1 = -14;
  x2 =  30; y2 =  14;

  tx = x1+4;
  ty = y2+4;
  Model = "Vdc";
  Name  = "V";

  Props.append(new Property("U", "1 V", true,
		QObject::tr("voltage in Volts")));

  rotate();  // fix historical flaw
}

Volt_dc::~Volt_dc()
{
}

Component* Volt_dc::newOne()
{
  return new Volt_dc();
}

Element* Volt_dc::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("dc Voltage Source");
  BitmapFile = (char *) "dc_voltage";

  if(getNewOne)  return new Volt_dc();
  return 0;
}
