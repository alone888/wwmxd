#include "logical_xor.h"

Logical_XOR::Logical_XOR()
{
  Description = QObject::tr("logical XOR");
  Model = "XOR";

  createSymbol();
  tx = x1+4;
  ty = y2+4;
}

Logical_XOR::~Logical_XOR()
{
}

Component* Logical_XOR::newOne()
{
  Logical_XOR* p = new Logical_XOR();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->Props.getLast()->Value = Props.getLast()->Value;
  p->recreate(0);
  return p;
}

Element* Logical_XOR::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port XOR");
  BitmapFile = (char *) "xor";

  if(getNewOne)  return new Logical_XOR();
  return 0;
}
