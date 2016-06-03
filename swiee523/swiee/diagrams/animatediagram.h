#ifndef ANIMATEDIAGRAM_H
#define ANIMATEDIAGRAM_H

#include "diagram.h"

class AnimateDiagram : public Diagram  {
public: 
  AnimateDiagram(int _cx=0, int _cy=0);
 ~AnimateDiagram();
 
  Diagram* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  int  calcDiagram();
  void calcLimits();
  void calcCoordinate(double* &, double* &, double* &, float*, float*, Axis*);
  bool insideDiagram(float, float);
  void paintAD(ViewPainter *p); // xuliand added to reload Diagram;;paint(ViewPainter *p)

protected:
  void clip(float* &);
};

#endif
