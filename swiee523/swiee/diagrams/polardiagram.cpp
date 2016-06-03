#if HAVE_CONFIG_H
# include <config.h>
#endif
#include <QtGui>
#include <math.h>
#include <float.h>
#if HAVE_IEEEFP_H
# include <ieeefp.h>
#endif

#ifndef NO_ISINFINITE
#define NO_ISINFINITE
#include <stdio.h>
#include <math.h>
#endif
#include "polardiagram.h"
#include "main.h"
#ifndef IS_FINITE
#define isfinite(x) _finite(x)
#endif

PolarDiagram::PolarDiagram(int _cx, int _cy) : Diagram(_cx, _cy)
{
  x1 = 10;     // position of label text
  y1 = 2;
  x2 = 200;    // initial size of diagram
  y2 = 200;
  x3 = 207;    // with some distance for right axes text
  Name = "Polar";

  Arcs.append(new struct Arc(0, y2, x2, y2, 0, 16*360, QPen(Qt::black,0)));
//  calcDiagram();
}

PolarDiagram::~PolarDiagram()
{
}

// ------------------------------------------------------------
void PolarDiagram::calcCoordinate(double* &, double* &yD, double* &,
				  float *px, float *py, Axis*)
{
  double yr = *(yD++);
  double yi = *(yD++);
  *px = float((yr/yAxis.up + 1.0)*double(x2)/2.0);
  *py = float((yi/yAxis.up + 1.0)*double(y2)/2.0);

  if(isfinite(*px))
    if(isfinite(*py))
      return;

  *px = *py = float(cx) / 2.0;
}

// --------------------------------------------------------------
void PolarDiagram::calcLimits()
{
  double a, b;
  calcPolarAxisScale(&yAxis, a, yAxis.step, b);
  yAxis.limit_min = 0.0;
  yAxis.limit_max = yAxis.up;
}

// --------------------------------------------------------------
int PolarDiagram::calcDiagram()
{
  Lines.clear();
  Texts.clear();
  Arcs.clear();

  // x line
  Lines.append(new Line(0, y2>>1, x2, y2>>1, GridPen));

  x3 = x2 + 7;
  createPolarDiagram(&yAxis);
  return 3;
}

// ------------------------------------------------------------
Diagram* PolarDiagram::newOne()
{
  return new PolarDiagram();
}

// ------------------------------------------------------------
Element* PolarDiagram::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Polar");
  BitmapFile = (char *) "polar";

  if(getNewOne)  return new PolarDiagram();
  return 0;
}
