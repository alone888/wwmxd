#ifndef CURVEDIAGRAM_H
#define CURVEDIAGRAM_H

#include "diagram.h"


class CurveDiagram : public Diagram  {
public: 
  CurveDiagram(int _cx=0, int _cy=0);
 ~CurveDiagram();


  Diagram* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  int  calcDiagram();
  void calcLimits();
  void calcCoordinate(double* &, double* &, double* &, float*, float*, Axis*);
  bool insideDiagram(float, float);

protected:
  void clip(float* &);
};

#endif
