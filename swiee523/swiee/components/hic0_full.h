#ifndef hic0_full_H
#define hic0_full_H

#include "component.h"

class hic0_full : public MultiViewComponent
{
  public:
    hic0_full();
    ~hic0_full() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
    static Element* info_pnp(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString netlist();
};

#endif /* hic0_full_H */
