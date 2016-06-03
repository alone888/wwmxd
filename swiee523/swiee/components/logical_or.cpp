#include "logical_or.h"

Logical_OR::Logical_OR()
{
  Description = QObject::tr("logical OR");
  Model = "OR";

  createSymbol();
  tx = x1+4;
  ty = y2+4;
}

Logical_OR::~Logical_OR()
{
}

Component* Logical_OR::newOne()
{
  Logical_OR* p = new Logical_OR();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->Props.getLast()->Value = Props.getLast()->Value;
  p->recreate(0);
  return p;
}

Element* Logical_OR::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port OR");
  BitmapFile = (char *) "or";

  if(getNewOne)  return new Logical_OR();
  return 0;
}
