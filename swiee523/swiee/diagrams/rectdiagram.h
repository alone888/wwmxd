#ifndef RECTDIAGRAM_H
#define RECTDIAGRAM_H

#include "diagram.h"


class RectDiagram : public Diagram  {
public: 
  RectDiagram(int _cx=0, int _cy=0);
 ~RectDiagram();


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
