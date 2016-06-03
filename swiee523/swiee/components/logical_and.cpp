#include "logical_and.h"

Logical_AND::Logical_AND()
{
  Description = QObject::tr("logical AND");
  Model = "AND";

  createSymbol();
  tx = x1+4;
  ty = y2+4;
}

Logical_AND::~Logical_AND()
{
}

Component* Logical_AND::newOne()
{
  Logical_AND* p = new Logical_AND();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->Props.getLast()->Value = Props.getLast()->Value;
  p->recreate(0);
  return p;
}

Element* Logical_AND::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port AND");
  BitmapFile = (char *) "and";

  if(getNewOne)  return new Logical_AND();
  return 0;
}
