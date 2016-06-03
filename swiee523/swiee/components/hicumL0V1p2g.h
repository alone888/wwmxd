#ifndef hicumL0V1p2g_H
#define hicumL0V1p2g_H

#include "component.h"

class hicumL0V1p2g : public MultiViewComponent
{
  public:
    hicumL0V1p2g();
    ~hicumL0V1p2g() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
    static Element* info_pnp(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString netlist();
};

#endif /* hicumL0V1p2g_H */
