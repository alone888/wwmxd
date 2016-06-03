#include "logical_nand.h"

Logical_NAND::Logical_NAND()
{
  Description = QObject::tr("logical NAND");
  Model = "NAND";

  createSymbol();
  tx = x1+4;
  ty = y2+4;
}

Logical_NAND::~Logical_NAND()
{
}

Component* Logical_NAND::newOne()
{
  Logical_NAND* p = new Logical_NAND();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->Props.getLast()->Value = Props.getLast()->Value;
  p->recreate(0);
  return p;
}

Element* Logical_NAND::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("n-port NAND");
  BitmapFile = (char *) "nand";

  if(getNewOne)  return new Logical_NAND();
  return 0;
}
