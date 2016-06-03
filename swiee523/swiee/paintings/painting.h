#ifndef PAINTING_H
#define PAINTING_H

#include "element.h"
#include "viewpainter.h"
//class QPainter;
//class Schematic;


class Painting : public Element  {
public:
  Painting();
 ~Painting() {};

  virtual void getCenter(int&, int &) {};
  virtual bool getSelected(float, float, float) { return false; };

  virtual Painting* newOne();
  virtual bool load(const QString&) { return true; };
  virtual QString save();
  virtual QString saveCpp();
  virtual QString saveJSON();
  virtual void paint(ViewPainter*) {};
  virtual void MouseMoving(Schematic*, int, int, int, int,
                           Schematic*, int, int, bool) {};
  virtual bool MousePressing() { return false; };
  virtual void Bounding(int&, int&, int&, int&);
  virtual bool resizeTouched(float, float, float) { return false; };
  virtual void MouseResizeMoving(int, int, Schematic*) {};

  virtual void rotate() {};
  virtual void mirrorX() {};
  virtual void mirrorY() {};
  virtual bool Dialog() { return false; };

  QString toPenString (int);
  QString toBrushString (int);
  QString Name; // name of painting, e.g. for saving
  int  State;   // state for different mouse operations
};

#endif