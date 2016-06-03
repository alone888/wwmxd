#ifndef EKV26MOS_H
#define EKV26MOS_H

#include "component.h"

class EKV26MOS : public MultiViewComponent
{
  public:
    EKV26MOS();
    ~EKV26MOS() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
    static Element* info_pmos(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
    QString netlist();
};

#endif /* EKV26MOS_H */
