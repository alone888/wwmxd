#ifndef PSDIAGRAM_H
#define PSDIAGRAM_H

#include "diagram.h"


// mixed polar and smith diagram
class PSDiagram : public Diagram  {
public:
  PSDiagram(int _cx=0, int _cy=0, bool _polarUp=true);
  ~PSDiagram();


  Diagram* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  static Element* info_sp(QString&, char* &, bool getNewOne=false);
  int  calcDiagram();
  void calcLimits();
  void calcCoordinate(double* &, double* &, double* &, float*, float*, Axis*);
};

#endif
