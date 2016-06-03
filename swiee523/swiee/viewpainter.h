#ifndef VIEWPAINTER_H
#define VIEWPAINTER_H

#include <QPainter>

#define TO_INT(f)   (f > 0.0 ? int(f + 0.5) : int(f - 0.5))

/* This class is neccessary because scaled fonts looks very ungly.
   Avoiding this is unfortunately not easy.
   This class scales the font point size instead of the font glyphes.
   But this has the disadvantage, that the text size does not increase
   equally with the rest of the drawing. We therefore must compute
   some thing anew each time the paint function is called. The class
   "ViewPainter" supports this. */

class ViewPainter {
public:
  ViewPainter(QPainter *p=0);
 ~ViewPainter();

  void init(QPainter*, float, int, int, int, int, float FontScale_=0.0,
	    float PrintScale_=1.0);
  void map(int, int, int&, int&);
  void drawPoint(int, int);
  void drawLine (int, int, int, int);
  void drawLines(int, int, float*);
  void drawRect (int, int, int, int);
  void drawRectD(int, int, int, int);
  void drawRoundRect(int, int, int, int);
  void drawEllipse(int, int, int, int);
  void drawArc(int, int, int, int, int, int);
  int  drawText(const QString&, int, int, int *Height=0);
  int  drawTextMapped(const QString&, int, int, int *Height=0);
  void fillRect(int, int, int, int, const QColor&);
  void eraseRect(int, int, int, int);
  void drawResizeRect(int, int);
  void drawStarSymbols(int, int, float*);
  void drawCircleSymbols(int, int, float*);
  void drawArrowSymbols(int, int, float*);
  void drawOneCircleSymbol(int, int, float*, int);

  QPainter *Painter;
  float Scale, FontScale, PrintScale, DX , DY;
  int LineSpacing;   // updated by init(), just for info
};

#endif
