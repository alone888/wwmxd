#include "logical_nor.h"

Logical_NOR::Logical_NOR()
{
  Description = QObject::tr("logical NOR");
  Model = "NOR";

  createSymbol();
  tx = x1+4;
  ty = y2+4;
}

Logical_NOR::~Logical_NOR()
{
}

Component* Logical_NOR::newOne()
{
  Logical_NOR* p = new Logical_NOR();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->Props.getLast()->Value = Props.getLast()->Value;
  p->recreate(0);
  return p;
}

Element* Logical_NOR::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port NOR");
  BitmapFile = (char *) "nor";

  if(getNewOne)  return new Logical_NOR();
  return 0;
}
