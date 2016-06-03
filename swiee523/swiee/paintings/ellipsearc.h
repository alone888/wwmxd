#ifndef ELLIPSEARC_H
#define ELLIPSEARC_H

#include "painting.h"

#include <QPen>


class EllipseArc : public Painting  {
public:
  EllipseArc();
 ~EllipseArc();

  void paintScheme(Schematic*);
  void getCenter(int&, int&);
  void setCenter(int, int, bool relative=false);

  Painting* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  bool load(const QString&);
  QString save();
  QString saveCpp();
  QString saveJSON();
  void paint(ViewPainter*);
  void MouseMoving(Schematic*, int, int, int, int, Schematic*, int, int, bool);
  bool MousePressing();
  bool getSelected(float, float, float);
  bool resizeTouched(float, float, float);
  void MouseResizeMoving(int, int, Schematic*);

  void rotate();
  void mirrorX();
  void mirrorY();
  bool Dialog();

  QPen  Pen;
  int   Angle, ArcLen;   // start angle and arc length
};

#endif
