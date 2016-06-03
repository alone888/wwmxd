#ifndef IFILE_H
#define IFILE_H

#include "component.h"


class iFile : public Component  {
public:
  iFile();
  ~iFile();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

  QString getSubcircuitFile();

protected:
  QString netlist();
};

#endif
