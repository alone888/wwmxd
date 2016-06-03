#ifndef hic2_full_H
#define hic2_full_H

#include "component.h"

class hic2_full : public Component
{
  public:
    hic2_full();
    ~hic2_full() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* hic2_full_H */
