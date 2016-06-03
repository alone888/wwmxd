#ifndef RICHDIAGRAM_H
#define RICHDIAGRAM_H

#include "diagram.h"


class RichDiagram : public Diagram  {
public: 
  RichDiagram(int _cx=0, int _cy=0);
 ~RichDiagram();

  virtual Diagram* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
  virtual void paint(ViewPainter*);
  virtual int calcDiagram();
  virtual int scroll(int);
  virtual bool scrollTo(int, int, int);

  void createAxisLabels() {};   // no labels in this diagram

protected:
  void calcData(Graph*) {};  // no graph data
};

#endif
