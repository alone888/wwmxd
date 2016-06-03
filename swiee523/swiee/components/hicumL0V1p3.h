#ifndef hicumL0V1p3_H
#define hicumL0V1p3_H

#include "component.h"

class hicumL0V1p3 : public MultiViewComponent
{
  public:
    hicumL0V1p3();
    ~hicumL0V1p3() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
    static Element* info_pnp(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString netlist();
};

#endif /* hicumL0V1p3_H */
