#ifndef POLARDIAGRAM_H
#define POLARDIAGRAM_H

#include "diagram.h"


class PolarDiagram : public Diagram  {
public: 
  PolarDiagram(int _cx=0, int _cy=0);
 ~PolarDiagram();


  Diagram* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  int  calcDiagram();
  void calcLimits();
  void calcCoordinate(double* &, double* &, double* &, float*, float*, Axis*);
};

#endif
