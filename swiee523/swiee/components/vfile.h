#ifndef VFILE_H
#define VFILE_H

#include "component.h"


class vFile : public Component  {
public:
  vFile();
  ~vFile();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

  QString getSubcircuitFile();

protected:
  QString netlist();
};

#endif
