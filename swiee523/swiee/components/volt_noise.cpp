#include "volt_noise.h"


Volt_noise::Volt_noise()
{
  Description = QObject::tr("noise voltage source");

  Arcs.append(new Arc(-12,-12, 24, 24,     0, 16*360,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30,  0,-12,  0,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 30,  0, 12,  0,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-12,  1,  1,-12,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-10,  6,  6,-10,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -7, 10, 10, -7,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( -2, 12, 12, -2,QPen(Qt::darkBlue,2)));

  Ports.append(new Port( 30,  0));
  Ports.append(new Port(-30,  0));

  x1 = -30; y1 = -15;
  x2 =  30; y2 =  15;

  tx = x1+4;
  ty = y2+4;
  Model = "Vnoise";
  Name  = "V";

  Props.append(new Property("u", "1e-6", true,
		QObject::tr("voltage power spectral density in V^2/Hz")));
  Props.append(new Property("e", "0", false,
		QObject::tr("frequency exponent")));
  Props.append(new Property("c", "1", false,
		QObject::tr("frequency coefficient")));
  Props.append(new Property("a", "0", false,
		QObject::tr("additive frequency term")));

  rotate();  // fix historical flaw
}

Volt_noise::~Volt_noise()
{
}

Component* Volt_noise::newOne()
{
  return new Volt_noise();
}

Element* Volt_noise::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Noise Voltage Source");
  BitmapFile = (char *) "noise_volt";

  if(getNewOne)  return new Volt_noise();
  return 0;
}