#ifndef NODE_H
#define NODE_H

#include "element.h"

#include <Q3PtrList>

class ViewPainter;
class WireLabel;
class QPainter;


class Node : public Conductor {
public:
  Node(int, int);
 ~Node();

  void  paint(ViewPainter*);
  bool  getSelected(int, int);
  void  setName(const QString&, const QString&, int x_=0, int y_=0);

  Q3PtrList<Element> Connections;
  QString Name;  // node name used by creation of netlist
  QString DType; // type of node (used by digital files)
  int State;	 // remember some things during some operations
};

#endif