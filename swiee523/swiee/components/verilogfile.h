#ifndef VERILOGFILE_H
#define VERILOGFILE_H

#include "component.h"
//Added by qt3to4:
#include <QTextStream>


class Verilog_File : public MultiViewComponent  {
public:
  Verilog_File();
 ~Verilog_File() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

  bool createSubNetlist(QTextStream *);
  QString getErrorText() { return ErrText; }
  QString getSubcircuitFile();

protected:
  QString verilogCode(int);
  void createSymbol();
  QString loadFile();

  QString ModuleName;
  QString ErrText;
};

class Verilog_File_Info {
public:
  Verilog_File_Info();
  Verilog_File_Info(QString, bool isfile = false);
 ~Verilog_File_Info() {};
  QString parsePorts(QString, int);

public:
  QString ModuleName;
  QString PortNames;
};

#endif
